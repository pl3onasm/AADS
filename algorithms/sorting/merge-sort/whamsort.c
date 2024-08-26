/* 
  file: whamsort.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: WHAM sort, an optimized version of merge sort
  time complexity: O(nlogn) 
*/

#include "../../../lib/clib.h"

//===================================================================
// Merges two sorted subarrays into one sorted array
void merge (int *arr, int left, int invIndex, int right) {
    // allocate memory for an auxiliary array to 
    // store the combined sorted array
  int *sorted = safeMalloc((right - left) * sizeof(int));
  size_t s = 0, l = left, r = invIndex;
    // merge the two subarrays into the auxiliary array
    // in sorted order until one of the subarrays is exhausted
  while (l < invIndex && r < right) {
    if (arr[l] < arr[r]) sorted[s++] = arr[l++];
    else sorted[s++] = arr[r++];
  }
    // copy the remaining elements into the end 
    // of the auxiliary array
  while (l < invIndex) sorted[s++] = arr[l++];
  while (r < right) sorted[s++] = arr[r++];
    // copy the merged array back to the input array
  for (int i = left; i < right; i++) 
    arr[i] = sorted[i - left];
  free(sorted);
}

//===================================================================
// Returns the index of the first inversion in the array
size_t getInvIndex (int *arr, size_t left, size_t right) {
  size_t idx = left + 1;
  while (idx < right && arr[idx - 1] <= arr[idx]) idx++;
  return idx;
}

//===================================================================
// Sorts an array of integers in ascending order in O(nlogn) time
void whamSort (int *arr, size_t left, size_t right) {

    // while the array has more than one element
  while (left + 1 < right) {
      // find the first inversion in the array
    size_t invIndex = getInvIndex (arr, left, right); 
      // if there are no inversions, the array is sorted
    if (invIndex >= right) break;
      // compute the new right bound based on the first inversion
    size_t newRight = MIN(2*invIndex - left, right);
      // sort the right subarray
    whamSort (arr, invIndex, newRight);
      // combine the sorted halves
    merge (arr, left, invIndex, newRight);
  }
}

//===================================================================

int main (){
  
  READ(int, arr, "%d", len);
  
  whamSort(arr, 0, len);
  PRINT_ARRAY(arr, "%d", len);
  
  free(arr);
  return 0;
}