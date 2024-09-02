#include "fibheap.h"
#include "../../../lib/clib.h"
#include <math.h>

//===================================================================
// FNV-1a hash function
// (http://www.isthe.com/chongo/tech/comp/fnv/index.html)
static uint64_t fibHash(void *key, uint64_t seed) {

  if (! key) {
    fprintf(stderr, "fibHash: key is NULL\n");
    exit(EXIT_FAILURE);
  }
  char *str = (char *)key;
  char ch;
    // FNV offset basis and magic seed
  uint64_t hash = 14695981039346656037ULL + seed;  
  while ((ch = *str++)) {
    hash ^= ch;
    hash *= 1099511628211ULL;  // FNV prime
  }
  return hash;
}

//===================================================================
// Case sensitive comparison for keys
static int cmpStrCS(void const *str1, void const *str2) {
  return strcmp((char *)str1, (char *)str2);
}

//===================================================================
// Creates a new Fibonacci heap
fibheap *fibNew(fibType type, fibCompKey compKey, 
                 fibCopyKey copyKey, fibFreeKey freeKey,
                 fibToString toString, void *sentinel) {
  
  fibheap *F = safeCalloc(1, sizeof(fibheap));
  F->datamap = mapNew(fibHash, 30, cmpStrCS);
  F->label = "FIB HEAP";
  F->copyKey = copyKey;
  F->freeKey = freeKey;
  F->toString = toString;
  F->compKey = compKey;
  F->type = type;
  F->fac = type == MIN ? 1 : -1;
  F->sentinel = sentinel;
  return F;
}

//===================================================================
// Creates a new Fibonnacci node with data and key
static fibnode *newFibnode(fibheap *F, void *data, void *key) {
  fibnode *u = safeCalloc(1, sizeof(fibnode));
  u->data = data;
  u->key = key;
  u->mark = false;
  u->degree = 0;
    // add a new key-value pair to the datamap
  mapAddKey(F->datamap, F->toString(data), u);
  return u;
}

//===================================================================
// Deallocates a Fibonacci node
static void freeFibnode(fibheap *F, fibnode *u) {
  F->freeKey(u->key);
    // remove the key-value pair from the datamap
  mapDelKey(F->datamap, F->toString(u->data));
  free(u);
}

//===================================================================
// Deallocates the Fibonacci heap
void fibFree(fibheap *F) {
  while (F->size) 
    fibPop(F);
  
  if (F->datamap) mapFree(F->datamap);
  free(F);
}

//===================================================================
// Sets the show function for the Fibonacci heap
void fibSetShow(fibheap *F, fibShowKey showKey, 
                fibShowData showData) {
  F->showKey = showKey;
  F->showData = showData;
}

//===================================================================
// Sets the label for the Fibonacci heap
void fibSetLabel(fibheap *F, char *label) {
  F->label = label;
}

//===================================================================
// Insertes node u before node v in the circular doubly linked list
// containing v
static void cListInsert (fibnode *u, fibnode *v) {
  u->next = v;
  u->prev = v->prev;
  v->prev->next = u;
  v->prev = u;
}

//===================================================================
// Removes node u from the circular doubly linked list containing u
static void cListRemove (fibnode *u) {
    // adjust parent's child pointer if necessary
  if (u->parent && u->parent->child == u) {
    if (u->next != u) 
      u->parent->child = u->next;
    else
      u->parent->child = NULL;
  }
    // remove u from the list
  u->prev->next = u->next;
  u->next->prev = u->prev;
    // turn u into a circular list of one node
  u->next = u->prev = u;        
  u->parent = NULL;             
}

//===================================================================
// Makes u the only node in the root list
static void makeCircularRoot (fibheap *F, fibnode *u) {
  u->next = u->prev = u;
  F->top = u;
}

//===================================================================
// Removes u from the root list and makes it a child of v
static void link(fibheap *H, fibnode *u, fibnode *v) {
  cListRemove(u);               
  if (! v->child)      
      // if v is childless, make u its only child
    v->child = u; 
  else 
      // else insert u into the child list of v
    cListInsert(u, v->child); 
  u->parent = v;
  v->degree++;  
  u->mark = false; 
}

//===================================================================
// Inserts a node with given data and key into the Fibonacci heap
void fibPush(fibheap *F, void *data, void *key) {
  
  if (! F || ! data || ! key) return;

  if (F->sentinel && F->fac * F->compKey(key, F->sentinel) == 0) {
    fprintf(stderr, "fibPush: key is the sentinel key\n");
    return;
  }

  if (mapGetVal(F->datamap, F->toString(data))) {
    fprintf(stderr, "fibPush: data already in the heap\n");
    return;
  }

  fibnode *u = newFibnode(F, data, F->copyKey(key));

  if (! F->top)                  
      // if the heap is empty, make a root list of one node
    makeCircularRoot(F, u);     
  else {
      // else insert u into the existing root list
      // and update the top pointer if necessary
    cListInsert(u, F->top);     
    if (F->fac * F->compKey(u->key, F->top->key) < 0) 
      F->top = u;              
  }
  F->size++;                   
}

//===================================================================
// Consolidates the heap by linking nodes of equal degree until
// no two nodes in the root list have the same degree
static void consolidate(fibheap *F) {
  
    // maxDegree of a node in the Fibonacci heap of size n is
    // maxD(n) = floor(log_phi(n)), where phi is the golden ratio
    // constant given by (1 + sqrt(5)) / 2
  size_t maxD = floor(log(F->size) / log((1 + sqrt(5)) / 2)) + 1;
  
    // A is an auxiliary array of pointers to nodes in the root list
  fibnode **A = safeCalloc(maxD, sizeof(fibnode*));  
  fibnode *u = F->top, *end = F->top->prev;
  bool last = false;
  
    // iterate over the root list while there are unprocessed nodes
    // or until we reach the last node
  while (!last && u != u->next) {
    if (u == end) last = true;  
      // save the next node in the root list
    fibnode *next = u->next;    
    size_t d = u->degree;       
    while (A[d]) {      
        // get the node v with the same degree as u
      fibnode *v = A[d];        
      if (F->fac * F->compKey(u->key, v->key) > 0) 
        SWAP(u, v);
      link(F, v, u);            
      A[d++] = NULL;            
    }
      // u is now the node with unique 
      // degree d in the root list
    A[d] = u; 
    u = next;
  }            
  
    // rebuild the root list from the array A
  F->top = NULL;
  for (size_t i = 0; i < maxD; i++) {
    if (A[i]) {         
      fibnode *w = A[i];
      if (!F->top)
          // if the heap is empty, make a root list of one node 
        makeCircularRoot(F, w); 
      else {
          // else insert w into the root list 
          // and update the top pointer if necessary
        cListInsert(w, F->top);
        if (F->fac * F->compKey(w->key, F->top->key) < 0)
          F->top = w;  
      }
    }
  }
  free(A); 
}

//===================================================================
// Extracts the top node from the Fibonacci heap
void *fibPop(fibheap *F) {
  
  fibnode *z = F->top;
  
  if (z) {
    fibnode *u = z->child;
    for (size_t i = 0; i < z->degree; i++) {
      fibnode *w = u->next; 
        // remove u from z's child list       
      cListRemove(u);           
        // insert u into the root list
      cListInsert(u, F->top); 
      u = w;
    }

    if (z == z->next)      
        // if z is the only node in the root list,
        // make the heap empty
      F->top = NULL;   
    else {
        // else set top pointer to z's sibling and
        // remove z from the root list
      F->top = z->next;         
      cListRemove(z); 
        // consolidate the heap if necessary  
      if (F->top != F->top->next)
        consolidate(F); 
    }

    F->size--; 
    void *data = z->data;
    freeFibnode(F, z);
    return data; 
  }

  return NULL;
}

//===================================================================
// Removes u from the child list of v, and makes it a root node
static void cut(fibheap *F, fibnode *u, fibnode *v){
  
  v->degree--;   
    // update v's child pointer if necessary
  if (v->child == u)   
    v->child = u->next;         
  if (! v->degree)       
    v->child = NULL;            
    // remove u from v's child list
  cListRemove(u);     
    // insert u into the root list
  cListInsert(u, F->top); 
  u->mark = false;    
}

//===================================================================
// Recursively cuts u from its parent until it reaches a root node
// or an unmarked node
static void cascadingCut(fibheap *F, fibnode *u) {
  
  fibnode *z = u->parent;
  if (z) {
    if (! u->mark) u->mark = true;
    else {
      cut(F, u, z);
      cascadingCut(F, z);
    }
  }
}

//===================================================================
// Changes the key of the node containing the data to newKey
static void fibChangeKey(fibheap *F, fibnode *u, void *newKey) {
  
  F->freeKey(u->key);
  u->key = F->copyKey(newKey);    
        
  fibnode *v = u->parent;
  if (v && (F->fac * F->compKey(u->key, v->key) < 0)) {
      // if u is not a root and its key is less than 
      // its parent's key, cut u from its parent  
    cut(F, u, v);
    cascadingCut(F, v);
  }
    // update top pointer if necessary
  if (F->fac * F->compKey(u->key, F->top->key) < 0) 
    F->top = u;
}

//===================================================================
// Updates the key of the node containing the data to newKey
// Returns true if the update was successful
bool fibUpdateKey(fibheap *F, void *data, void *newKey) {

  if (!F || ! data || ! newKey) return false;
  
  if (F->sentinel && F->fac * F->compKey(newKey, F->sentinel) == 0) {
    fprintf(stderr, "fibUpdateKey: new key is the sentinel key\n");
    return false;
  }

  fibnode *u = mapGetVal(F->datamap, F->toString(data));
  if (! u) {
    fprintf(stderr, "fibUpdateKey: data not in the heap\n");
    return false;
  }

  if (F->fac * F->compKey(newKey, u->key) > 0) {
    fprintf(stderr, "fibUpdateKey: new key is %s than current key\n",
           F->type == MIN ? "greater" : "less");
    return false;
  }

  fibChangeKey(F, u, newKey);

  return true;
}

//===================================================================
// Returns the key of the data in the Fibonacci heap
void *fibGetKey(fibheap *F, void *data) {
  if (! data || ! F) return NULL;
  fibnode *u = mapGetVal(F->datamap, F->toString(data));
  if (! u) {
    fprintf(stderr, "fibGetKey: data not in the heap\n");
    return NULL;
  }
  return u->key;
}

//===================================================================
// Shows all data in the Fibonacci heap starting from node u
// at given level
static void showAllData(fibheap *F, fibnode *u, size_t level) {
  
  if (! u) return;
  fibnode *v = u;
  do {
    for (size_t i = 0; i < level; i++) printf("   ");
    printf(" |-> ");
    F->showData(v->data);
    printf(" (key: ");
    F->showKey(v->key);
    printf(")\n");
    if (v->child) showAllData(F, v->child, level + 1);
    v = v->next;
  } while (v != u);
}

//===================================================================
// Shows the Fibonacci heap
void fibShow(fibheap *F) {

  if (! F->showData || ! F->showKey) {
    fprintf(stderr, "fibShow: showData or "
                    "showKey function not set\n");
    return;
  }

  printf("--------------------\n"
          " %s\n"
          " Size: %zu\n"
          "--------------------\n",
          F->label, F->size);
  
  if (! F->top) {
    printf("< Empty >\n");
    printf("--------------------\n\n");
    return;
  }
  showAllData(F, F->top, 0);
  printf("--------------------\n\n");
}

//===================================================================
// Takes a peek at the top node in the Fibonacci heap
void *fibPeek(fibheap *F) {
  if (! F->top) return NULL;
  return F->top->data;
}

//===================================================================
// Returns true if the data is in the Fibonacci heap
bool fibContains(fibheap *F, void *data) {
  if (! data) return false;
  return mapHasKey(F->datamap, F->toString(data));
}

//===================================================================
// Deletes a node from the Fibonacci heap by updating its key
// to a given sentinel key. The node is then removed from the heap
// Returns true if deletion was successful
bool fibDelete(fibheap *F, void *data) {
  if (! F || ! data) {
    printf("fibDelete: invalid arguments\n");
    return false;
  }

  if (! F->sentinel) {
    fprintf(stderr, "fibDelete: sentinel key not set\n");
    return false;
  }

  fibnode *u = mapGetVal(F->datamap, F->toString(data));
  if (! u) {
    fprintf(stderr, "fibDelete: following data not in the heap\n  ");
    F->showData(data);
    return false;
  }
  
  fibChangeKey(F, u, F->sentinel);
  fibPop(F);
  return true;
}

//===================================================================
// Takes the union of two Fibonacci heaps and returns the resulting
// Fibonacci heap
fibheap *fibUnion(fibheap *F1, fibheap *F2) {
  
  if (! F1) return F2;
  if (! F2) return F1;

  if (F1->type != F2->type) {
    fprintf(stderr, "fibUnion: heaps have different types\n");
    return NULL;
  }

  if (F1->sentinel != F2->sentinel) {
    fprintf(stderr, "fibUnion: heaps have different sentinels\n");
    return NULL;
  }

  fibheap *F = fibNew(F1->type, F1->compKey, F1->copyKey, 
                      F1->freeKey, F1->toString, F1->sentinel);

  F->size = F1->size + F2->size;
  F->top = F1->top;

    // concatenate the root lists of F1 and F2
  if (F1->top && F2->top) {
    F1->top->prev->next = F2->top;
    F2->top->prev->next = F1->top;
    SWAP(F1->top->prev, F2->top->prev);
  } 

  if (F2->top && (! F1->top || 
      F1->fac * F1->compKey(F2->top->key, F1->top->key) < 0))
    F->top = F2->top;
  
  mapFree(F->datamap);
  F->datamap = mapMerge(F1->datamap, F2->datamap);
  F1->datamap = NULL;
  
  free(F1);
  free(F2);
  return F;
}