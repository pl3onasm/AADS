/* file: bubblesort.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: bubble sort
   time complexity: O(n^2)
*/

#include <stdlib.h>
#include <stdio.h>

typedef enum {               
  false = 0,
  true = 1
} bool;

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

void bubbleSort(int *arr, int n) {
  /* sorts an array of size n using bubble sort */
  bool swapped = true;
  for (int i = 0; i < n; i++) {
    // if no swaps were made in the previous iteration, 
    // the array is sorted
    if (!swapped) break;
    swapped = false;
    // bubble up smaller elements to the front of the array
    for (int j = n - 1; j > i; j--) {
      if (arr[j] < arr[j - 1]) {
        swap(arr, j, j - 1);
        swapped = true;
      }
    }
  }
}

int main (int argc, char *argv[]){
  int intExample[] = {10, 8, -9, 6, 7, 5, 2, 3, 4, -1, 2,
                      13, 7, 11, 20, 1, -15, 7, 16, 0};
  printf("Unsorted:\n");
  printArray(intExample, 20);
  bubbleSort(intExample, 20);
  printf("Sorted:\n");
  printArray(intExample, 20);
  return 0;
}