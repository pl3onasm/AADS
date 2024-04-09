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
#include "../../lib/clib/clib.h"

#define RIGHT(i) (2 * i + 2)
#define LEFT(i) (2 * i + 1)
#define PARENT(i) ((i - 1) / 2)

//===================================================================
// Creates a new priority queue
bpqueue *bpqNew(size_t capacity, bpqType type, bpqCompKey compKey, 
                bpqCopyKey copyKey, bpqFreeKey freeKey, 
                bpqToString toString) {

  bpqueue *pq = safeCalloc(1, sizeof(bpqueue));
  pq->arr = safeCalloc(capacity, sizeof(bpqNode *));
  pq->map = sstMapNew(CASE_SENSITIVE, capacity);
    // make map (string -> idx) manage its own keys
  sstMapCopyKeys(pq->map);   
  pq->toString = toString;
  pq->capacity = capacity;
  pq->compKey = compKey;
  pq->copyKey = copyKey;
  pq->freeKey = freeKey;
  pq->type = type;
  pq->label = "BINARY PQ";
  pq->delim = ", ";
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
    if (pq->arr[i] && pq->freeData)
      pq->freeData(pq->arr[i]->data);
    pq->freeKey(pq->arr[i]->key);
    free(pq->arr[i]);
  }
  free(pq->arr);
  sstMapFree(pq->map);
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
// Sets the priority queue to own the input data
// freeing what is still left when the queue is freed
void bpqOwnData(bpqueue *pq, bpqFreeData freeData) {
  pq->freeData = freeData;
}

//===================================================================
// Makes the priority queue operate on copies of the data
void bpqCopyData(bpqueue *pq, bpqCpyData copyData, 
                 bpqFreeData freeData) {
  pq->copyData = copyData;
  pq->freeData = freeData;
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
  bpqNode *tmp = pq->arr[i];
  pq->arr[i] = pq->arr[j];
  pq->arr[j] = tmp;
    // update the map with the new indices
  sstMapAddKey(pq->map, pq->toString(pq->arr[i]->data), i);
  sstMapAddKey(pq->map, pq->toString(pq->arr[j]->data), j);
}

//===================================================================
// Heapifies the priority queue
static void bpqHeapify(bpqueue *pq, size_t idx) {
  size_t l = LEFT(idx);
  size_t r = RIGHT(idx);
  if (pq->type == MIN) {      // min heap
    size_t smallest = idx;
    if (l < pq->size && pq->compKey(pq->arr[l]->key, 
                                    pq->arr[smallest]->key) < 0)
      smallest = l;
    if (r < pq->size && pq->compKey(pq->arr[r]->key, 
                                    pq->arr[smallest]->key) < 0) 
      smallest = r;
    if (smallest != idx) {
      swapNodes(pq, idx, smallest);
      bpqHeapify(pq, smallest);
    }
  } else {                    // max heap
  size_t largest = idx;
    if (l < pq->size && pq->compKey(pq->arr[l]->key, 
                                    pq->arr[largest]->key) > 0) 
      largest = l;
    if (r < pq->size && pq->compKey(pq->arr[r]->key, 
                                    pq->arr[largest]->key) > 0) 
      largest = r;
    if (largest != idx) {
      swapNodes(pq, idx, largest);
      bpqHeapify(pq, largest);
    }
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
  sstMapDelKey(pq->map, pq->toString(top));
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
  sstMapAddKey(pq->map, pq->toString(pq->arr[0]->data), 0);
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
  sstMapAddKey(pq->map, pq->toString(data), idx);
    // restore the heap property
  if (pq->type == MIN) {      // min heap
    while (idx > 0 && pq->compKey(pq->arr[idx]->key, 
                                  pq->arr[PARENT(idx)]->key) < 0) {
      swapNodes(pq, idx, PARENT(idx));
      idx = PARENT(idx);
    }
  } else {                    // max heap
    while (idx > 0 && pq->compKey(pq->arr[idx]->key, 
                                  pq->arr[PARENT(idx)]->key) > 0) {
      swapNodes(pq, idx, PARENT(idx));
      idx = PARENT(idx);
    }
  }
    // increase the size of the queue
  pq->size++;
}

//===================================================================
// Checks if the data is in the priority queue
bool bpqContains(bpqueue *pq, void *data) {
  return sstMapHasKey(pq->map, pq->toString(data));
}

//===================================================================
// Returns the key associated with the data
void *bpqGetKey(bpqueue *pq, void *data) {
  size_t idx = 0;
  if (!sstMapHasKeyVal(pq->map, pq->toString(data), &idx) ||
      ! pq->arr[idx])
    return NULL;
  return pq->arr[idx]->key;
}

//===================================================================
// Updates the priority of a node in the queue by updating the key
// and reordering the heap if necessary
void bpqUpdateKey(bpqueue *pq, void *data, void *newKey) {
  
    // get the index of the node in the queue
  size_t idx = 0;
  if (!sstMapHasKeyVal(pq->map, pq->toString(data), &idx) ||
      ! pq->arr[idx])
    return;

    // min heap / decrease key
  if (pq->type == MIN) {
    if (pq->compKey(newKey, pq->arr[idx]->key) > 0) {
      fprintf(stderr, "New key is larger than current key\n"); 
      return;
    }
    pq->freeKey(pq->arr[idx]->key);
    pq->arr[idx]->key = pq->copyKey(newKey);
      // restore the heap property
    while (idx > 0 && pq->compKey(pq->arr[idx]->key, 
                                  pq->arr[PARENT(idx)]->key) < 0) {
      swapNodes(pq, idx, PARENT(idx));
      idx = PARENT(idx);
    }

    // max heap / increase key
  } else {          
    if (pq->compKey(newKey, pq->arr[idx]->key) < 0) {
      fprintf(stderr, "New key is smaller than current key\n"); 
      return;
    }
    pq->freeKey(pq->arr[idx]->key);
    pq->arr[idx]->key = pq->copyKey(newKey);
      // restore the heap property
    while (idx > 0 && pq->compKey(pq->arr[idx]->key, 
                                  pq->arr[PARENT(idx)]->key) > 0) {
      swapNodes(pq, idx, PARENT(idx));
      idx = PARENT(idx);
    }
  } 
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
  }
  printf("\n--------------------\n\n");
}

//===================================================================
// End of file
#undef RIGHT
#undef LEFT
#undef PARENT