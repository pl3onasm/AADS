/* 
  Generic Doubly Linked List (DLL) implementation
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "../../../lib/clib/clib.h"
#include "dll.h"

  // Creates a new DLL
dll *dllNew () {
  dll *L = safeCalloc(1, sizeof(dll));
  L->NIL = safeCalloc(1, sizeof(dllNode));
  L->size = 0;
  L->NIL->next = L->NIL;
  L->NIL->prev = L->NIL;
  L->NIL->dllData = NULL;
  L->iter = L->NIL;
  L->cmp = NULL;
  L->showData = NULL;
  L->delim = ", ";
  L->label = NULL;
  L->freeData = NULL;
  L->copyData = NULL;
  return L;
}

  // Sets the DLL to own the input data,
  // freeing it when the DLL is freed
void dllOwnData (dll *L, dllFreeData freeData) {
  L->freeData = freeData;
}

  // Sets the DLL to make copies of the data
  // If set, the DLL will only free the copies
void dllCopyData (dll *L, dllCpyData copyData, 
                     dllFreeData freeData) {
  L->freeData = freeData;
  L->copyData = copyData;
}

  // Sets the delimiter function for the DLL
void dllSetDelim (dll *L, char* delim) {
  L->delim = delim;
}

  // Sets the showData function for the DLL
void dllSetShow (dll *L, dllShowData showData) {
  L->showData = showData;
}
  
  // Sets the comparison function for the DLL
void dllSetCmp (dll *L, dllCmpData cmp) {
  L->cmp = cmp;
}

  // Sets the label for the DLL
void dllSetLabel (dll *L, char *label) {
  L->label = label;
}

  // Makes a copy of the DLL
dll *dllCopy (dll *L, dllCpyData copyData, dllFreeData freeData) {
  dll *new = dllNew();
  new->cmp = L->cmp;
  new->showData = L->showData;
  new->delim = L->delim;
  new->freeData = freeData;
  new->copyData = copyData;
  dllNode *node = L->NIL->next;
  while (node != L->NIL) {
    dllPush(new, node->dllData);
    node = node->next;
  }
  return new;
}

  // Creates a new DLL node
dllNode *dllNewNode () {
  dllNode *node = safeCalloc(1, sizeof(dllNode));
  node->dllData = NULL;
  return node;
}

  // Deallocates a DLL node
void dllFreeNode (dll *L, dllNode *node) {
  if (L->freeData) 
    L->freeData(node->dllData);
  free(node);
}

  // Makes the DLL empty
void dllEmpty (dll *L) {
  dllNode *node = L->NIL->next; 
  while (node != L->NIL) {
    dllNode *next = node->next;
    dllFreeNode(L, node);
    node = next;
  }
  L->NIL->next = L->NIL;
  L->NIL->prev = L->NIL;
  L->size = 0;
}

  // Frees the entire DLL
void dllFree (dll *L) {
  dllNode *node = L->NIL->next;
  while (node != L->NIL) {
    dllNode *next = node->next;
    dllFreeNode(L, node);
    node = next;
  }
  free (L->NIL);
  free(L);
}

  // Prepends a node to the DLL 
void dllPush (dll *L, void *data) {
  dllNode *n = dllNewNode();
    // first node becomes the second node
  n->next = L->NIL->next;
  L->NIL->next->prev = n;
    // the new node becomes the first node
  L->NIL->next = n;
  n->prev = L->NIL;
    // update the size and data
  L->size++;
  if (L->copyData) 
    n->dllData = L->copyData(data);
  else
    n->dllData = data;
}

  // Inserts a node in a sorted DLL
void dllInsert (dll *L, void *data) {
  if (dllIsEmpty(L)) {
    dllPush(L, data);
    return;
  }
  if (! L->cmp) {
    fprintf(stderr, "dllInsert: comparison function not set\n");
    return;
  }

  dllNode *new = dllNewNode();
  new->dllData = data;

    // find the node to insert after
    // the node is the first node that is not 
    // smaller than the new node
  dllNode *n = L->NIL->next;
  
  while (n != L->NIL && L->cmp(n->dllData, data) < 0) 
    n = n->next;

    // insert the new node
  new->next = n;
  new->prev = n->prev;
    // update pointers of the adjacent nodes
  n->prev->next = new;
  n->prev = new;
    // update the data
  if (L->copyData) 
    new->dllData = L->copyData(data);
  else
    new->dllData = data;
    // update the size
  L->size++;
}

  // Appends a node to the DLL
void dllPushBack (dll *L, void *data) {
  dllNode *n = dllNewNode();
    // the last node becomes the penultimate node
  n->prev = L->NIL->prev;
  L->NIL->prev->next = n;
    // the new node becomes the last node
  L->NIL->prev = n;
  n->next = L->NIL;
    // update the size and data
  L->size++;
  if (L->copyData) 
    n->dllData = L->copyData(data);
  else
    n->dllData = data;
}

  // Deletes a node from the DLL
void dllDelete (dll *L, dllNode *node) {
  if (node == L->NIL) 
    return;
  node->prev->next = node->next;
  node->next->prev = node->prev;
  dllFreeNode(L, node);
  L->size--;
}

  // Deletes the first node with a given key
void dllDeleteData (dll *L, void *data) {
  if (! L->cmp) {
    fprintf(stderr, "dllDeleteData: comparison function not set\n");
    return;
  }
  L->NIL->dllData = data;
  dllNode *n = L->NIL->next;
  while (L->cmp(n->dllData, data))
    n = n->next;
  if (n == L->NIL)
    return;
  dllDelete(L, n);
}

  // Searches for the first node with a given key
  // returns the data of the node if found, NULL otherwise
void *dllFind (dll *L, void *key) {
  if (! L->cmp) {
    fprintf(stderr, "dllFind: comparison function not set\n");
    return NULL;
  }
  L->NIL->dllData = key;
  dllNode *n = L->NIL->next;
  while (L->cmp(n->dllData, key))
    n = n->next;
  if (n == L->NIL) 
    return NULL;
  return n->dllData;
}

  // Shows the DLL, prints delimiter (default: ", ")
void dllShow (dll *L) {
  if (dllIsEmpty(L)) 
    return;
  if (! L->showData){
    fprintf(stderr, "dllShow: showData function not set\n");  
    return;
  }

  // if a label is set, print it along with the DLL size
  if (L->label) 
    printf("%s[%zu]: ", L->label, L->size);

  dllNode *node = L->NIL->next;
  while (node != L->NIL) {
    L->showData(node->dllData);
    node = node->next;
    if (node != L->NIL) 
      printf("%s", L->delim);
  }
  printf("\n");
}

  // Sets the list iterator to the data of the first node
void dllFirst(dll *L) {
  L->iter = L->NIL->next;
}

  // Sets the list iterator to the data of the last node  
void dllLast(dll *L) {
  L->iter = L->NIL->prev;
}

  // Returns true if the iterator has reached
  // the start or end of the DLL
bool dllEnd(dll *L) {
  return L->iter == L->NIL;
}

  // Returns the data of the current node
  // and updates the list iterator to the next node
  // value is NULL if the end of the DLL is reached
void *dllNext(dll *L) {
  if (L->iter == L->NIL) 
    return NULL;
  void *data = L->iter->dllData;
  L->iter = L->iter->next;
  return data;
}

  // Returns the data of the current node
  // and updates the list iterator to the previous node
  // value is NULL if the beginning of the DLL is reached
void *dllPrev(dll *L) {
  if (L->iter == L->NIL) 
    return NULL;
  void *data = L->iter->dllData;
  L->iter = L->iter->prev;
  return data;
}
