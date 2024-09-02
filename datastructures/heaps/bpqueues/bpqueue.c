/* 
  Generic priority queue interface, using binary heaps
  Supports updating priorities using a hash table
    mapping data to indices in the queue (str(data) -> idx)
    String representation of data should be unique for each 
    data item
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "bpqueue.h"
#include "../../../lib/clib.h"

#define RIGHT(i) (2 * i + 2)
#define LEFT(i) (2 * i + 1)
#define PARENT(i) ((i - 1) / 2)

//===================================================================
// Creates a new priority queue
bpqueue *bpqNew(size_t capacity, bpqType type, bpqCompKey compKey, 
                bpqCopyKey copyKey, bpqFreeKey freeKey, 
                bpqToString toString, void *sentinel) {

  bpqueue *pq = safeCalloc(1, sizeof(bpqueue));
  pq->arr = safeCalloc(capacity, sizeof(bpqNode *));
  pq->datamap = sstMapNew(CASE_SENSITIVE, capacity);
    // make map (string -> idx) manage its own keys
  sstMapCopyKeys(pq->datamap);   
  pq->toString = toString;
  pq->capacity = capacity;
  pq->compKey = compKey;
  pq->copyKey = copyKey;
  pq->freeKey = freeKey;
  pq->type = type;
  pq->fac = type == MIN ? 1 : -1;
  pq->label = "BINARY PQ";
  pq->delim = ", ";
  pq->sentinel = sentinel;
  return pq;
}

//===================================================================
// Creates a new pbqNode
bpqNode *bpqNodeNew(void *data, void *key) {
  bpqNode *node = safeCalloc(1, sizeof(bpqNode));
  node->data = data;
  node->key = key;
  return node;
}

//===================================================================
// Deallocates the priority queue
void bpqFree(bpqueue *pq) {
  if (!pq) return;
  for (size_t i = 0; i < pq->size; i++) {
    pq->freeKey(pq->arr[i]->key);
    free(pq->arr[i]);
  }
  free(pq->arr);
  sstMapFree(pq->datamap);
  free(pq);
}

//===================================================================
// Sets the show function for the priority queue
void bpqSetShow(bpqueue *pq, bpqShowKey showKey, 
                bpqShowData showData) {
  pq->showKey = showKey;
  pq->showData = showData;
}

//===================================================================
// Sets the label for the priority queue
void bpqSetLabel(bpqueue *pq, char *label) {
  pq->label = label;
}

//===================================================================
// Sets the delimiter for the show function
void bpqSetDelim(bpqueue *pq, char *delim) {
  pq->delim = delim;
}

//===================================================================
// Returns the top element of the priority queue without removing it
// from the queue; returns NULL if the queue is empty
void *bpqPeek(bpqueue *pq) {
  if (bpqIsEmpty(pq))
    return NULL;
  return pq->arr[0]->data;
}

//===================================================================
// Swaps two nodes in the priority queue
static void swapNodes(bpqueue *pq, size_t i, size_t j) {
  SWAP(pq->arr[i], pq->arr[j]);
    // update the map with the new indices
  sstMapAddKey(pq->datamap, pq->toString(pq->arr[i]->data), i);
  sstMapAddKey(pq->datamap, pq->toString(pq->arr[j]->data), j);
}

//===================================================================
// Heapifies the priority queue
static void bpqHeapify(bpqueue *pq, size_t idx) {
  size_t l = LEFT(idx);
  size_t r = RIGHT(idx);
    // find the node that is the best candidate for the top
  size_t best = idx;
  if (l < pq->size && (pq->fac * pq->compKey(pq->arr[l]->key, 
                                 pq->arr[best]->key) < 0))
    best = l;
  if (r < pq->size && (pq->fac * pq->compKey(pq->arr[r]->key, 
                                 pq->arr[best]->key) < 0))
    best = r;
  if (best != idx) {
    swapNodes(pq, idx, best);
    bpqHeapify(pq, best);
  }
}

//===================================================================
// Removes the top element from the priority queue
void *bpqPop(bpqueue *pq) {
  if (bpqIsEmpty(pq))
    return NULL;
    // get the top element
  void *top = pq->arr[0]->data;
    // remove the string(data) -> idx mapping
  sstMapDelKey(pq->datamap, pq->toString(top));
    // free the key
  pq->freeKey(pq->arr[0]->key);
    // free the bpqNode, obviously NOT the data
  free(pq->arr[0]); 
    // decrease the size of the queue
  pq->size--;

    // if this was the last node, we are done
  if (bpqIsEmpty(pq))
    return top;
      
    // move the last node to the top
  pq->arr[0] = pq->arr[pq->size];
    // avoid dangling pointers
  pq->arr[pq->size] = NULL;   
    // update the map
  sstMapAddKey(pq->datamap, pq->toString(pq->arr[0]->data), 0);
    // restore the heap property
  bpqHeapify(pq, 0);
  return top;
}

//===================================================================
// Adds a new node to the priority queue
void bpqPush(bpqueue *pq, void *data, void *key) {
    // increase the capacity of the queue if necessary
  if (pq->size == pq->capacity) {
    pq->capacity *= 2;
    pq->arr = safeRealloc(pq->arr, pq->capacity * sizeof(bpqNode *));
  }
    // get the index of the new node
  size_t idx = pq->size;
    // create a new node
  pq->arr[idx] = bpqNodeNew(data, pq->copyKey(key));
    // add a string(data) -> idx mapping 
  sstMapAddKey(pq->datamap, pq->toString(data), idx);
    // restore the heap property
  while (idx > 0 && (pq->fac * pq->compKey(pq->arr[idx]->key, 
                               pq->arr[PARENT(idx)]->key) < 0)) {
      swapNodes(pq, idx, PARENT(idx));
      idx = PARENT(idx);
  }
    // increase the size of the queue
  pq->size++;
}

//===================================================================
// Checks if the data is in the priority queue
bool bpqContains(bpqueue *pq, void *data) {
  return sstMapHasKey(pq->datamap, pq->toString(data));
}

//===================================================================
// Returns the key associated with the data
void *bpqGetKey(bpqueue *pq, void *data) {
  size_t idx = 0;
  if (!sstMapHasKeyVal(pq->datamap, pq->toString(data), &idx) ||
      ! pq->arr[idx])
    return NULL;
  return pq->arr[idx]->key;
}

//===================================================================
// Changes the key of the node containing the data to newKey
static bool bpqChangeKey(bpqueue *pq, void *data, void *newKey) {
  
    // get the index of the node in the queue
  size_t idx = 0;
  if (!sstMapHasKeyVal(pq->datamap, pq->toString(data), &idx) ||
      ! pq->arr[idx]) {
    fprintf(stderr, "bpqChangeKey: data not in the queue\n");
    return false;
  }

  if (pq->fac * pq->compKey(newKey, pq->arr[idx]->key) > 0) {
    fprintf(stderr, "bpqChangeKey: new key is %s than current key\n",
            pq->type == MIN ? "greater" : "less");
    return false;
  }

  pq->freeKey(pq->arr[idx]->key);
  pq->arr[idx]->key = pq->copyKey(newKey);
    // restore the heap property
  while (idx > 0 && (pq->fac * pq->compKey(pq->arr[idx]->key, 
                               pq->arr[PARENT(idx)]->key) < 0)) {
    swapNodes(pq, idx, PARENT(idx));
    idx = PARENT(idx);
  }

  return true;
}

//===================================================================
// Updates the priority of a node in the queue by updating the key
// and reordering the heap if necessary
bool bpqUpdateKey(bpqueue *pq, void *data, void *newKey) {
  if (!pq || ! data || ! newKey) {
    fprintf(stderr, "bpqUpdateKey: invalid arguments\n");
    return false;
  }

  if (pq->sentinel && 
      pq->fac * pq->compKey(newKey, pq->sentinel) == 0) {
    fprintf(stderr, "bpqUpdateKey: new key is the sentinel key\n");
    return false;
  }

  return bpqChangeKey(pq, data, newKey);
}

//===================================================================
// Shows the priority queue
void bpqShow(bpqueue *pq) {
  if (bpqIsEmpty(pq)) {
    printf("Priority queue is empty\n");
    return;
  }
  if (! pq->showKey || ! pq->showData) {
    fprintf(stderr, "No showKey or showData function set\n");
    return;
  }
  printf("--------------------\n"
          " %s\n"
          " Size: %zu\n"
          "--------------------\n",
          pq->label, pq->size);

  for (size_t i = 0; i < pq->size; i++) {
    printf ("[");
    pq->showKey(pq->arr[i]->key);
    printf ("]: ");
    pq->showData(pq->arr[i]->data);
    if (i < pq->size - 1) printf("%s", pq->delim);
    if ((i + 1) % 5 == 0) printf("\n");
  }
  printf("\n--------------------\n\n");
}

//===================================================================
// Deletes the node containing the data from the priority queue
bool bpqDelete(bpqueue *pq, void *data) {
  if (!pq || ! data) {
    fprintf(stderr, "bpqDelete: invalid arguments\n");
    return false;
  }

  if (! pq->sentinel) {
    fprintf(stderr, "bpqDelete: sentinel key not set\n");
    return false;
  }

  if (bpqChangeKey(pq, data, pq->sentinel)) {
    bpqPop(pq);
    return true;
  }

  return false;
}

//===================================================================
// End of file
#undef RIGHT
#undef LEFT
#undef PARENT