/* 
  file: inversioncount.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: 
    We want to find the number of inversions in the array. 
    For this we simply sort the array with mergesort, and count the
    number of inversions while merging. This approach gives a  
    solution in O(nlogn), since it's just mergesort with a counter.
*/

#include "../../../lib/clib/clib.h"

//===================================================================
// Merges two sorted subarrays into one sorted array while counting
// the number of inversions
size_t merge(int *arr, size_t left, size_t mid, size_t right) {
    // allocate memory for an auxiliary array to 
    // store the combined sorted array
  int *sorted = safeMalloc((right - left) * sizeof(int));
  size_t l = left, r = mid, s = 0, count = 0;
    // merge the two subarrays into the auxiliary array
    // in sorted order until one of the subarrays is exhausted
  while (l < mid && r < right) {
    if (arr[l] <= arr[r]) 
        // no inversions
      sorted[s++] = arr[l++];
    else {
        // total number of inversions is the number of 
        // remaining elements in the left subarray
      sorted[s++] = arr[r++];
      count += mid - l;
    }
  }
    // copy the remaining elements into the end 
    // of the auxiliary array
  while (l < mid) sorted[s++] = arr[l++];
  while (r < right) sorted[s++] = arr[r++];
    // copy the auxiliary array back to the input array
  for (size_t i = left; i < right; i++) 
    arr[i] = sorted[i - left];
  free(sorted);
  return count;
}

//===================================================================
// Counts the number of inversions in the array while sorting it
size_t inversionCount(int *arr, size_t left, size_t right) { 
  size_t count = 0; 

  // if the array has more than one element
  if (left + 1 < right) {
      // divide the array in two subarrays
      // around the midpoint
    size_t mid = left + (right - left)/2;
      // conquer the subarrays
    count += inversionCount(arr, left, mid);
    count += inversionCount(arr, mid, right);
      // combine the sorted halves
    count += merge(arr, left, mid, right);
  }
  return count;
}

//===================================================================

int main (){

  READ(int, arr, "%d", len);

  printf("%lu\n", inversionCount(arr, 0, len));

  free(arr); 
  return 0; 
}