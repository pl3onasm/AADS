/* 
  Generic binary heap implementation
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "binheap.h"
#include "../../../lib/clib/clib.h"

binheap *newBinHeap(size_t capacity, hpType hpType,
    int (*cmp)(const void *, const void *)) {
  binheap *h = safeCalloc(1, sizeof(binheap));
  h->capacity = capacity;
  h->size = 0;
  h->cmp = cmp;
  h->arr = safeCalloc(capacity, sizeof(void *));
  h->hpType = hpType;
  return h;
}

void freeBinHeap(binheap *H) {
  if (!H) return;
  free(H->arr);
  free(H);
}

void *peekAtBinHeap(binheap *H) {
  if (H->size == 0) return NULL;
  return H->arr[0];
}

void *popFromBinHeap(binheap *H) {
  if (H->size == 0) return NULL;
  void *top = H->arr[0];
  H->arr[0] = H->arr[H->size - 1];
  H->size--;
  heapifyBinHeap(H, 0);
  return top;
}

void pushToBinHeap(binheap *H, void *node) {
  if (H->size == H->capacity) {
    H->capacity *= 2;
    H->arr = safeRealloc(H->arr, H->capacity * sizeof(void *));
  }
  H->arr[H->size] = node;
  size_t i = H->size;
  H->size++;
  if (H->hpType == MIN) {     // min heap
    while (i > 0 && H->cmp(H->arr[i], H->arr[PARENT(i)]) < 0) {
      SWAP(H->arr[i], H->arr[PARENT(i)]);
      i = PARENT(i);
    }
  } else {                    // max heap
    while (i > 0 && H->cmp(H->arr[i], H->arr[PARENT(i)]) > 0) {
      SWAP(H->arr[i], H->arr[PARENT(i)]);
      i = PARENT(i);
    }
  }
}

void heapifyBinHeap(binheap *H, size_t idx) {
  size_t l = LEFT(idx);
  size_t r = RIGHT(idx);
  if (H->hpType == MIN) {     // min heap 
    size_t smallest = idx;
    if (l < H->size && H->cmp(H->arr[l], H->arr[smallest]) < 0)
      smallest = l;
    if (r < H->size && H->cmp(H->arr[r], H->arr[smallest]) < 0)
      smallest = r;
    if (smallest != idx) {
      SWAP(H->arr[idx], H->arr[smallest]);
      heapifyBinHeap(H, smallest);
    }
  } else {                    // max heap
    size_t largest = idx;
    if (l < H->size && H->cmp(H->arr[l], H->arr[largest]) > 0)
      largest = l;
    if (r < H->size && H->cmp(H->arr[r], H->arr[largest]) > 0)
      largest = r;
    if (largest != idx) {
      SWAP(H->arr[idx], H->arr[largest]);
      heapifyBinHeap(H, largest);
    }
  }
}

binheap *buildBinHeap(void *arr, size_t len, size_t elemSize, 
    hpType hpType, int (*cmp)(const void *, const void *)) {

  if (len == 0) {
    fprintf(stderr, "Error: cannot build a heap from an empty array\n");
    exit(EXIT_FAILURE);
  }

  binheap *H = newBinHeap(len, hpType, cmp);
  
  // copy the array into the heap 
  for (size_t i = 0; i < len; i++) 
    H->arr[i] = (char *)arr + i * elemSize;
 
  // heapify the heap, starting from the last non-leaf node
  H->size = len;
  for (size_t i = len / 2; i--; ) 
    heapifyBinHeap(H, i);
  
  return H;
}

void showBinHeap(binheap *H, char *delim, 
    void (*showData)(const void *)) {
  
  for (size_t i = 0; i < H->size; i++) {
    showData(H->arr[i]);
    printf(i < H->size - 1 ? "%s" : "", delim);
  }
  printf("\n");
}



