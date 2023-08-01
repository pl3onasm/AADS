/* file: randomqsort.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: quicksort, using a random element as pivot
   time complexity: expected O(nlogn)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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

void printArray (int *arr, int n) {
  /* prints an array of size n */
  printf("[");
  for (int i = 0; i < n; i++) {
    printf("%d", (arr)[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

void swap(int *arr, int i, int j) {
  /* swaps elements at indices i and j in arr */
  int temp = arr[i];
  arr[i] = arr[j];
  arr[j] = temp;
}

int partition(int *arr, int left, int right) {
  /* partitions arr[left:right] around a random pivot */
  srand(time(0));
  int random = left + rand() % (right - left);
  swap(arr, random, right);
  int pivot = right;
  int i = left;
  // loop invariant: arr[left:i] < arr[pivot]
  for (int j = left; j < right; j++) {
    if (arr[j] < arr[pivot]) {
      swap(arr, i, j);
      i++;
    }
  }
  swap(arr, i, pivot); // put pivot in its rightful place
  return i;
}

void quickSort(int *arr, int left, int right){
  /* sorts arr[left:right] in place */
  if (left < right) {
    // partition arr[left:right] around a pivot
    int q = partition(arr, left, right);
    // recursively sort the two partitions
    quickSort(arr, left, q - 1);
    quickSort(arr, q + 1, right);
  }
}

int main (int argc, char *argv[]){
  int intExample[] = {10, 8, -9, 6, 7, 5, 2, 3, 4, -1, 2,
                      13, 7, 11, 20, 1, -15, 7, 16, 0};
  printf("Unsorted:\n");
  printArray(intExample, 20);
  quickSort(intExample, 0, 19);
  printf("Sorted:\n");
  printArray(intExample, 20);
  return 0;
}
