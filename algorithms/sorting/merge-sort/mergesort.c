/* 
  file: mergesort.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: merge sort
  time complexity: O(nlogn) 
*/

#include "../../../lib/clib.h"

//===================================================================
// Merges two sorted subarrays into one sorted array
void merge(int *arr, size_t left, size_t mid, size_t right) {
    // allocate memory for an auxiliary array to 
    // store the combined sorted array
  int *sorted = safeMalloc((right - left) * sizeof(int));
  size_t l = left, r = mid, s = 0; 
    // merge the two subarrays into the auxiliary array
    // in sorted order until one of the subarrays is exhausted
  while (l < mid && r < right) {
    if (arr[l] < arr[r]) sorted[s++] = arr[l++];
    else sorted[s++] = arr[r++];
  }
    // copy the remaining elements into the end 
    // of the auxiliary array
  while (l < mid) sorted[s++] = arr[l++];
  while (r < right) sorted[s++] = arr[r++];
    // copy the auxiliary array back to the input array
  for (size_t i = left; i < right; i++) 
    arr[i] = sorted[i - left];
  free(sorted);
}

//===================================================================
// Sorts an array of integers in ascending order in O(nlogn) time
void mergeSort(int *arr, size_t left, size_t right) { 

    // if the array has more than one element
  if (left + 1 < right) {
      // divide the array in two subarrays
      // around the midpoint
    size_t mid = left + (right - left)/2;
      // conquer the subarrays
    mergeSort(arr, left, mid);
    mergeSort(arr, mid, right);
      // combine the sorted halves
    merge(arr, left, mid, right);
  }
}

//===================================================================

int main (){

  READ(int, arr, "%d", len);
  
  mergeSort(arr, 0, len);
  PRINT_ARRAY(arr, "%d", len);
  
  free(arr);
  return 0;
}