/* file: heapsort.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: heapsort
   time complexity: O(nlogn)
*/

#include <stdlib.h>
#include <stdio.h>

#include "../../../datastructures/heaps/binheaps/binheap.h"
#include "../../../lib/clib/clib.h"

void printInt(const void *a) {
  // print function for integers
  printf("%d", *(int *)a);
}

int intCmp(const void *a, const void *b) {
  // compares two integers and
  // returns -1 if a < b, 0 if a == b, 1 if a > b
  return *(int *)a - *(int *)b;
}

void heapsort(binheap *H){
  /* sorts the heap */
  size_t size = H->size;
  for (size_t i = H->size - 1; i >= 1; --i) {
    // swap the root with the last element of the heap
   SWAP(H->arr[0], H->arr[i]);
    // remove the last element from the heap 
    // by decreasing its size
    H->size--;
    // restore the bin heap property
    heapifyBinHeap(H, 0);
  }
  // restore the original size of the heap
  H->size = size;
}

int main (int argc, char *argv[]){
  size_t size = 0;
  // read the input array, set the size
  READ(int, arr, "%d", size);

  // build a max heap, sorts in ascending order
  binheap *hp = buildBinHeap(arr, size, sizeof(int),
                             false, intCmp);
  heapsort(hp);
  
  showBinHeap (hp, ", ", printInt);
  printf("\n");
  freeBinHeap(hp);

  // build a min heap, sorts in descending order
  hp = buildBinHeap(arr, size, sizeof(int),
                    true, intCmp);

  heapsort(hp);

  showBinHeap (hp, ", ", printInt);
  freeBinHeap(hp);
  free(arr);
  return 0;
}