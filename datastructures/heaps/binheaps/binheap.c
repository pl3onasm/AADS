#include "binheap.h"
#include "../../../lib/clib/clib.h"

binheap *newBinHeap(size_t capacity, bool isMin,
    int (*cmp)(const void *, const void *)) {
  binheap *h = safeCalloc(1, sizeof(binheap));
  h->capacity = capacity;
  h->size = 0;
  h->cmp = cmp;
  h->arr = safeCalloc(capacity, sizeof(void *));
  h->isMin = isMin;
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

void insertIntoBinHeap(binheap *H, void *node) {
  if (H->size == H->capacity) {
    H->capacity *= 2;
    H->arr = safeRealloc(H->arr, H->capacity * sizeof(void *));
  }
  H->arr[H->size] = node;
  size_t i = H->size;
  H->size++;
  if (H->isMin) {
    while (i > 0 && H->cmp(H->arr[i], H->arr[PARENT(i)]) < 0) {
      SWAP(H->arr[i], H->arr[PARENT(i)]);
      i = PARENT(i);
    }
  } else {
    while (i > 0 && H->cmp(H->arr[i], H->arr[PARENT(i)]) > 0) {
      SWAP(H->arr[i], H->arr[PARENT(i)]);
      i = PARENT(i);
    }
  }
}

void heapifyBinHeap(binheap *H, size_t idx) {
  size_t l = LEFT(idx);
  size_t r = RIGHT(idx);
  if (H->isMin) {             // min heap 
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




