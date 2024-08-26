/* 
  file: selectionsort.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: selection sort
  time complexity: O(n^2)
*/

#include "../../../lib/clib.h"

//===================================================================
// Sorts an array of integers in ascending order in place 
// in O(n^2) time
void selectionSort(int *arr, size_t n) {
  for (size_t i = 0; i < n - 1; i++) {
    size_t minIdx = i;
      // find the minimum element in the unsorted subarray
      // by performing a linear search
    for (size_t j = i + 1; j < n; j++) 
      if (arr[j] < arr[minIdx]) 
        minIdx = j;
      // swap minimum with 1st element of unsorted subarray
      // to extend the sorted subarray
    SWAP(arr[i], arr[minIdx]);
  }
}

//===================================================================

int main (){
  
  READ(int, arr, "%d", len);

  selectionSort(arr, len);
  PRINT_ARRAY(arr, "%d", len);
  
  free(arr);
  return 0;
}