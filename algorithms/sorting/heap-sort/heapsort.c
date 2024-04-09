/* file: heapsort.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: heapsort
   time complexity: O(nlogn)
*/

#include "../../../datastructures/heaps/binheaps/binheap.h"
#include "../../../lib/clib/clib.h"

void showInt(void const *a) {
  // print function for integers
  printf("%d", *(int *)a);
}

int cmpInts(void const *a, void const *b) {
  // compares two integers and
  // returns < 0 if a < b, 0 if a == b, > 0 if a > b
  return *(int *)a - *(int *)b;
}

void heapsort(binheap *H){
  // sorts the heap
  size_t size = H->size;
  for (size_t i = H->size - 1; i >= 1; --i) {
    // swap the root with the heap's last element
   SWAP(H->arr[0], H->arr[i]);
    // remove the last element from the heap 
    // by decreasing its size
    H->size--;
    // restore the bin heap property
    bhpHeapify(H, 0);
  }
    // restore the original size of the heap
  H->size = size;
}

int main (){
  size_t size = 0;
  
    // reads the input array, set the size
  READ(int, arr, "%d", size);

    // builds a max heap, sorts in ascending order
  binheap *H = bhpBuild(arr, size, sizeof(int),
                        MAX, cmpInts);
  heapsort(H);
  
  bhpSetShow(H, showInt);
  bhpShow(H);
  printf("\n");
  bhpFree(H);

    // builds a min heap, sorts in descending order
  H = bhpBuild(arr, size, sizeof(int),
               MIN, cmpInts);

  heapsort(H);

  bhpSetShow(H, showInt);
  bhpShow(H);
  bhpFree(H);
  free(arr);
  return 0;
}