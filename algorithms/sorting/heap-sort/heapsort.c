/* file: heapsort.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: heapsort
   time complexity: O(nlogn)
*/

#include <stdlib.h>
#include <stdio.h>

#define LEFT(i)   (2*i + 1)
#define RIGHT(i)  (2*i + 2)

typedef struct Heap {
  int size; 
  int *array;
} Heap;

void *safeMalloc (int n) {
  /* allocates n bytes of memory and checks whether the allocation
     was successful */
  void *ptr = malloc(n);
  if (ptr == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

Heap *newHeap(int size) {
  /* creates a new heap of given size */
  Heap *hp = safeMalloc(sizeof(Heap)); 
  hp->size = size;
  hp->array = safeMalloc(size*sizeof(int));
  return hp;
}

void freeHeap(Heap *H) {
  /* frees the heap */
  free(H->array);
  free(H);
}

void showHeap(Heap *H){
  /* prints the heap */
  printf("[");
  for (int i = 0; i < H->size; i++) {
    printf("%d", H->array[i]);
    if (i < H->size-1) printf(", ");
  }
  printf("]\n");
}

void swap (Heap *H, int a, int b) {
  /* swaps the elements at indices a and b in the heap */
  int temp = H->array[a]; 
  H->array[a] = H->array[b]; 
  H->array[b] = temp;
}

void maxHeapify(Heap *H, int i){
  /* restores the max heap property */
  int max = i, l = LEFT(i), r = RIGHT(i);
  if (l < H->size && H->array[l] > H->array[max])
    max = l;
  if (r < H->size && H->array[r] > H->array[max])
    max = r;
  if (max != i) { // is the max heap property violated?
    // move the violating element down the heap
    swap(H, i, max); 
    maxHeapify(H, max);
  }
}

void initMaxHeap(Heap *H){
  /* initializes the max heap */
  for (int i = H->size/2; i >= 0; i--)
    maxHeapify(H, i);
}

void heapsort(Heap *H){
  /* sorts the heap */
  int size = H->size;
  initMaxHeap(H);
  for (int i = H->size - 1; i > 0; --i) {
    // swap the root with the last element of the heap
    swap(H, 0, i);
    // remove the last element from the heap 
    // by decreasing its size
    H->size--;
    // restore the max heap property
    maxHeapify(H, 0);
  }
  // restore the original size of the heap
  H->size = size;
}

Heap *readHeap (int arr[], int size) {
  /* reads a heap from an array */
  Heap *hp = newHeap(size);
  for (int i = 0; i < size; i++) hp->array[i] = arr[i];
  return hp;
}

int main (int argc, char *argv[]){
  int intExample[] = {5, 6, 7, 8, 9, 10, 2, -1, 3, 4, 1, 2, 
                     -35, 78, -10, 13, 7, -11, 20, 1, 15, 7, 
                     16, 0, 1, 2, 5, 6, 100, 23};
  Heap *hp = readHeap(intExample, 30);
  printf("Unsorted:\n");
  showHeap (hp);
  heapsort(hp);
  printf("Sorted:\n");
  showHeap(hp);
  freeHeap(hp);
  return 0;
}