/* file: whamsort.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: WHAM sort, an optimized version of merge sort
   time complexity: O(nlogn) 
*/

#include <stdio.h>
#include <stdlib.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b)); 

void *safeMalloc (int n) {
  /* checks if memory has been allocated successfully */
  void *p = malloc(n);
  if (p == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);
  }
  return p;
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

void merge (int left, int invIndex, int right, int arr[]) {
  /* merges two sorted subarrays */
  int s = 0, l = left, r = invIndex;
  int *sorted = safeMalloc((right-left)*sizeof(int));
  // merge the two subarrays until one of them is exhausted
  while (l < invIndex && r < right) {
    if (arr[l] < arr[r]) sorted[s++] = arr[l++];
    else sorted[s++] = arr[r++];
  }
  // copy the remaining elements
  while (l < invIndex) sorted[s++] = arr[l++];
  while (r < right) sorted[s++] = arr[r++];
  // copy the merged subarray back to the original array
  for (int i = left; i < right; i++) arr[i] = sorted[i-left];
  free(sorted);
}

int getInvIndex (int left, int right, int *arr) {
  /*  returns the index of the first inversion in arr[left..right] */
  int idx = left + 1;
  while (idx < right && arr[idx - 1] <= arr[idx]) idx++;
  return idx;
}

void whamSort (int left, int right, int arr[]) {
  /* sorts arr[left..right] in ascending order */
  while (left < right) {
    // find the first inversion in arr[left..right]
    int invIndex = getInvIndex (left, right, arr); 
    // if there are no inversions, the array is sorted
    if (invIndex >= right) break;
    // compute the new right bound based on the first inversion
    int newRight = MIN(2*invIndex - left, right);
    // sort the right subarray
    whamSort (invIndex, newRight, arr);
    // merge the two sorted subarrays
    merge (left, invIndex, right, arr);
  }
}

int main(int argc, char *argv[]) {
  int intExample[] = {5, 6, 7, 8, -9, 10, 2, 3, 4, 1, 2,
                      13, 7, -11, 20, 1, 15, -7, 16, 0};
  printf("Unsorted\n");
  printArray(intExample, 20);
  whamSort (0, 20, intExample);
  printf("Sorted\n");
  printArray (intExample, 20);
  return 0;
}