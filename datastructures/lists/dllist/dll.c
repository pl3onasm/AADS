/* 
  Generic Doubly Linked List (DLL) implementation
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "../../../lib/clib/clib.h"
#include "dll.h"

//=================================================================
// Creates a new DLL
dll *dllNew () {
  dll *L = safeCalloc(1, sizeof(dll));
  L->NIL = safeCalloc(1, sizeof(dllNode));
  L->size = 0;
  L->NIL->next = L->NIL;
  L->NIL->prev = L->NIL;
  L->NIL->dllData = NULL;
  L->iter = L->NIL;
  L->delim = ", ";
  return L;
}

//=================================================================
// SETTERS
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

//=================================================================
// Makes a copy of the DLL
dll *dllCopy (dll *L, dllCpyData copyData, dllFreeData freeData) {
  if (! L) 
    return NULL;
  if (! copyData) {
    fprintf(stderr, "dllCopy: copyData function not set\n");
    return NULL;
  }
  if (! freeData) {
    fprintf(stderr, "dllCopy: freeData function not set\n");
    return NULL;
  }
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

//=================================================================
// Creates a new DLL node
dllNode *dllNewNode () {
  dllNode *node = safeCalloc(1, sizeof(dllNode));
  node->dllData = NULL;
  return node;
}

//=================================================================
// Deallocates a DLL node
static void dllFreeNode (dll *L, dllNode *node) {
  if (! node || ! L)
    return;
  if (L->freeData) 
    L->freeData(node->dllData);
  free(node);
}

//=================================================================
// Makes the DLL empty
void dllEmpty (dll *L) {
  if (! L) 
    return;
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

//=================================================================
// Frees the entire DLL
void dllFree (dll *L) {
  if (! L) 
    return;
  dllNode *node = L->NIL->next;
  while (node != L->NIL) {
    dllNode *next = node->next;
    dllFreeNode(L, node);
    node = next;
  }
  free (L->NIL);
  free(L);
}

//=================================================================
// Prepends a node to the DLL 
void dllPush (dll *L, void *data) {
  if (! L) 
    return;
    // create a new first node
  dllNode *n = dllNewNode();
    // current first node becomes the second node
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

//=================================================================
// Inserts a node in a sorted DLL
void dllInsert (dll *L, void *data) {
  if (! L) 
    return;
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

//=================================================================
// Appends a node to the DLL
void dllPushBack (dll *L, void *data) {
  if (! L) 
    return;
    // create a new node (last node
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

//=================================================================
// Pops the first node from the DLL
// Returns the data of the node; NULL if the DLL is empty
void *dllPop (dll *L) {
  if (! L) 
    return NULL;
  if (dllIsEmpty(L)) 
    return NULL;
  dllNode *n = L->NIL->next;
  void *data = n->dllData;
    // free the node, NOT the data
  n->prev->next = n->next;
  n->next->prev = n->prev;
  free(n);
  L->size--;
  return data;
}

//=================================================================
// Pops the last node from the DLL
// Returns the data of the node; NULL if the DLL is empty
void *dllPopBack (dll *L) {
  if (! L) 
    return NULL;
  if (dllIsEmpty(L)) 
    return NULL;
  dllNode *n = L->NIL->prev;
  void *data = n->dllData;
    // free the node, NOT the data
  n->prev->next = n->next;
  n->next->prev = n->prev;
  free(n);
  L->size--;
  return data;
}

//=================================================================
// Deletes a node from the DLL
static void dllDeleteNode (dll *L, dllNode *node) {
  if (! L) 
    return;
  if (node == L->NIL) 
    return;
  node->prev->next = node->next;
  node->next->prev = node->prev;
  dllFreeNode(L, node);
  L->size--;
}

//=================================================================
// Deletes node at current iterator position and
// moves the iterator to the next node
void dllDeleteCurr (dll *L) {
  if (! L) 
    return;
  if (L->iter == L->NIL) 
    return;
  dllNode *node = L->iter;
  L->iter = L->iter->next;
  dllDeleteNode(L, node);
}

//=================================================================
// Deletes the first node with a given key
// Returns true if the node was found and deleted
// Returns false otherwise
bool dllDeleteData (dll *L, void *data) {
  if (! L) 
    return false;
  if (! L->cmp) {
    fprintf(stderr, "dllDeleteData: comparison function not set\n");
    return false;
  }
  if (dllIsEmpty(L)) 
    return false;
  L->NIL->dllData = data;
  dllNode *n = L->NIL->next;
  while (L->cmp(n->dllData, data))
    n = n->next;
  if (n == L->NIL)
    return false;
  dllDeleteNode(L, n);
  return true;
}

//=================================================================
// Searches for the first node with a given key
// returns the data of the node if found, NULL otherwise
void *dllFind (dll *L, void *key) {
  if (! L) 
    return NULL;
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

//=================================================================
// Shows the DLL
void dllShow (dll *L) {
  if (!L || dllIsEmpty(L)) 
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

//=================================================================
// Peeks at the data of the first node
// NULL is returned if the DLL is empty
void *dllPeek (dll *L) {
  if (! L) 
    return NULL;
  if (dllIsEmpty(L)) 
    return NULL;
  return L->NIL->next->dllData;
}

//=================================================================
// Peeks at the data of the last node
// NULL is returned if the DLL is empty
void *dllPeekBack (dll *L) {
  if (! L) 
    return NULL;
  if (dllIsEmpty(L)) 
    return NULL;
  return L->NIL->prev->dllData;
}

//=================================================================
// Peeks at the data of the next node
// NULL if the end of the DLL is reached
void *dllPeekNext (dll *L) {
  if (! L) 
    return NULL;
  if (L->iter == L->NIL)
    return NULL;
  return L->iter->next->dllData;
}

//=================================================================
// Peeks at the data of the previous node
// NULL is the start of the DLL is reached
void *dllPeekPrev (dll *L) {
  if (! L) 
    return NULL;
  if (L->iter == L->NIL)
    return NULL;
  return L->iter->prev->dllData;
}

//=================================================================
// Peeks at the data of the current node
// NULL if the iterator is at the end of the DLL
void *dllPeekCurr (dll *L) {
  if (! L) 
    return NULL;
  if (L->iter == L->NIL)
    return NULL;
  return L->iter->dllData;
}

//=================================================================
// Resets the list iterator
void dllResetIter (dll *L) {
  if (! L) 
    return;
  L->iter = L->NIL;
}

//=================================================================
// Sets the iterator to the first node
void dllSetIterFirst (dll *L) {
  if (! L) 
    return;
  L->iter = L->NIL->next;
}

//=================================================================
// Sets the iterator to the last node
void dllSetIterLast (dll *L) {
  if (! L) 
    return;
  L->iter = L->NIL->prev;
}

//=================================================================
// Sets the iterator the first node of the DLL and returns the 
// data of that node
void *dllFirst(dll *L) {
  if (! L) 
    return NULL;
  L->iter = L->NIL->next;
  if (L->iter == L->NIL) 
    return NULL;
  return L->iter->dllData;
}

//=================================================================
// Sets the iterator to the last node of the DLL and returns the
// data of that node
void *dllLast(dll *L) {
  if (! L) 
    return NULL;
  L->iter = L->NIL->prev;
  if (L->iter == L->NIL) 
    return NULL;
  return L->iter->dllData;
}

//=================================================================
// Returns true if the next 
// node is the end of the DLL
bool dllEnd(dll *L) {
  if (! L) 
    return true;
  return L->iter->next == L->NIL;
}

//=================================================================
// Returns true if the previous
// node is the beginning of the DLL
bool dllStart(dll *L) {
  if (! L) 
    return true;
  return L->iter->prev == L->NIL;
}

//=================================================================
// Updates the list iterator to the next node
// and returns the data of that node
// value is NULL if the end of the DLL is reached
void *dllNext(dll *L) {
  if (! L) 
    return NULL;
  L->iter = L->iter->next;
  if (L->iter == L->NIL) 
    return NULL;
  return L->iter->dllData;
}

//=================================================================
// Updates the list iterator to the previous node
// and returns the data of that node
// value is NULL if the beginning of the DLL is reached
void *dllPrev(dll *L) {
  if (! L) 
    return NULL;
  L->iter = L->iter->prev;
  if (L->iter == L->NIL) 
    return NULL;
  return L->iter->dllData;
}

