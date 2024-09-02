/* 
  Generic binary heap implementation
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "binheap.h"
#include "../../../lib/clib.h"

#define RIGHT(i) (2 * i + 2)
#define LEFT(i) (2 * i + 1)
#define PARENT(i) ((i - 1) / 2)

//===================================================================
// Creates a new binary heap
binheap *bhpNew(size_t capacity, bhpType hpType,
                bhpCompData cmp) {
  binheap *h = safeCalloc(1, sizeof(binheap));
  h->capacity = capacity;
  h->cmp = cmp;
  h->arr = safeCalloc(capacity, sizeof(*(h->arr)));
  h->hpType = hpType;
  h->fac = hpType == MIN ? 1 : -1;
  h->label = "BINARY HEAP";
  h->delim = ", ";
  return h;
}

//===================================================================
// Deallocates the binary heap
void bhpFree(binheap *H) {
  if (!H) return;
  free(H->arr);
  free(H);
}

//===================================================================
// Sets the show function for the heap
void bhpSetShow(binheap *H, bhpShowData show) {
  H->show = show;
}

//===================================================================
// Sets the label for the heap
void bhpSetLabel(binheap *H, char *label) {
  H->label = label;
}

//===================================================================
// Sets the delimiter for the show function
void bhpSetDelim(binheap *H, char *delim) {
  H->delim = delim;
}

//===================================================================
// Returns the top element of the heap without removing it
void *bhpPeek(binheap *H) {
  if (H->size == 0) return NULL;
  return H->arr[0];
}

//===================================================================
// Removes the top element from the heap
void *bhpPop(binheap *H) {
  if (H->size == 0) return NULL;
  void *top = H->arr[0];
  H->arr[0] = H->arr[H->size - 1];
  H->size--;
  bhpHeapify(H, 0);
  return top;
}

//===================================================================
// Adds a new node to the heap
void bhpPush(binheap *H, void *node) {
  if (H->size == H->capacity) {
    H->capacity *= 2;
    H->arr = safeRealloc(H->arr, H->capacity * sizeof(void *));
  }
  size_t idx = H->size;
  H->arr[idx] = node;
    // restore the heap property
  while (idx > 0 && 
        (H->fac * H->cmp(H->arr[idx], H->arr[PARENT(idx)]) < 0)) {
    SWAP(H->arr[idx], H->arr[PARENT(idx)]);
    idx = PARENT(idx);
  }
  H->size++;
}

//===================================================================
// Heapifies the binary heap starting from the given index
void bhpHeapify(binheap *H, size_t idx) {
  size_t l = LEFT(idx);
  size_t r = RIGHT(idx);
    // find the node that is the best candidate for the top
  size_t best = idx;
  if (l < H->size && (H->fac * H->cmp(H->arr[l], H->arr[best]) < 0))
    best = l;
  if (r < H->size && (H->fac * H->cmp(H->arr[r], H->arr[best]) < 0))
    best = r;
  if (best != idx) {
    SWAP(H->arr[idx], H->arr[best]);
    bhpHeapify(H, best);
  }
}

//===================================================================
// Builds a binary heap from an array
binheap *bhpBuild(void *arr, size_t len, size_t elemSize,
                  bhpType hpType, bhpCompData cmp) {

  if (len == 0) {
    fprintf(stderr, "Error: cannot build a heap "
                    "from an empty array\n");
    exit(EXIT_FAILURE);
  }

  binheap *H = bhpNew(len, hpType, cmp);

    // copy the array into the heap 
  for (size_t i = 0; i < len; i++) {
    void *node = (char *)arr + i * elemSize;
    H->arr[i] = node;
  }
 
    // heapify the heap, 
    // starting from the last non-leaf node
  H->size = len;
  for (size_t i = len / 2; i--; ) 
    bhpHeapify(H, i);
  
  return H;
}

//===================================================================
// Shows the binary heap
void bhpShow(binheap *H) {
  if (!H->show) {
    fprintf(stderr, "Error: no show function set for the heap\n");
    return;
  }
  printf("--------------------\n"
         "%s\n"
         "Type: %s\n"
         "Size: %zu\n"
         "--------------------\n",
          H->label, 
          H->hpType == MIN ? "MIN" : "MAX", 
          H->size);

  for (size_t i = 0; i < H->size; i++) {
    H->show(H->arr[i]);
    printf(i < H->size - 1 ? "%s" : "\n", H->delim);
    if ((i + 1) % 10 == 0) printf("\n");
  }

  printf("--------------------\n\n");
}

//===================================================================
// End of file
#undef RIGHT
#undef LEFT
#undef PARENT