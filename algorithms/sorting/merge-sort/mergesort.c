/* file: mergesort.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: merge sort, using a temporary array to merge
   time complexity: O(nlogn) 
*/

#include <stdlib.h>
#include <stdio.h>

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
    printf("%d", arr[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

void merge(int *arr, int left, int mid, int right) {
  /* merges two sorted subarrays into one sorted array */
  int *sorted = safeMalloc((right - left + 1)*sizeof(int));
  int l = left, r = mid + 1, s = 0; 
  // merge while both subarrays are not empty  
  while (l <= mid && r <= right) {
    if (arr[l] < arr[r]) sorted[s++] = arr[l++];
    else sorted[s++] = arr[r++];
  }
  // copy the remaining elements
  while (l <= mid) sorted[s++] = arr[l++];
  while (r <= right) sorted[s++] = arr[r++];
  // copy the sorted array back to the original array
  for (int i = left; i <= right; i++) arr[i] = sorted[i - left];
  free(sorted);
}

void mergeSort(int *arr, int left, int right) { 
  /* sorts the int array in ascending order */
  if (left < right) {
    // divide the array in two subarrays
    int mid = left + (right - left)/2;
    // conquer the subarrays
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    // combine the sorted halves
    merge(arr, left, mid, right);
  }
}

int main (int argc, char *argv[]){
  int example[] = {10, -8, 9, 6, 7, -5, 2, 3, 4, -1, 2,
                   13, -7, 11, 20, 1, -15, 7, 16, 0};
  printf("Unsorted:\n");
  printArray(example, 20);
  mergeSort(example, 0, 19);
  printf("Sorted:\n");
  printArray(example, 20);
  return 0;
}