/* 
  file: quicksort.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: quicksort, using last element as pivot
  time complexity: worst case O(n^2), average case O(nlogn)
*/

#include "../../../lib/clib.h"

//===================================================================
// Partitions an array around a pivot element (last element) and
// returns the sorted position of this pivot
size_t partition(int *arr, size_t left, size_t right) {
  
  size_t pivotIdx = right - 1, i = left;

    // keep swapping elements smaller than the pivot
    // loop invariant: arr[left:i-1] < pivot
  for (size_t j = left; j < right - 1; j++)
    if (arr[j] < arr[pivotIdx]) 
      SWAP(arr[i++], arr[j]);

    // put pivot in its final sorted position
  SWAP(arr[i], arr[pivotIdx]);
  return i;
}

//===================================================================
// Sorts an array of integers in place in ascending order
void quickSort(int *arr, size_t left, size_t right){
  
    // if the array has more than one element
  if (left + 1 < right) {
      // partition the array around the last element as pivot
    size_t pivotIdx = partition(arr, left, right);
      // recursively sort the two partitions
    quickSort(arr, left, pivotIdx);
    quickSort(arr, pivotIdx + 1, right);
  }
}

//===================================================================

int main (){

  READ(int, arr, "%d", len);
  
  quickSort(arr, 0, len);
  PRINT_ARRAY(arr, "%d", len);
  
  free(arr);
  return 0;
}
