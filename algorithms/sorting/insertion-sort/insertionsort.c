/* 
  file: insertionsort.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: insertion sort
  time complexity: O(n^2)
*/

#include "../../../lib/clib/clib.h"

//===================================================================
// Sorts an array of integers in ascending order in place 
// in O(n^2) time
void insertionSort(int *arr, size_t n) {
  for (size_t i = 1; i < n; i++) {
    int key = arr[i];
    size_t j = i;
      // shift elements of the sorted subarray arr[0..i-1] to
      // the right until the correct position for key is found
    while (j > 0 && arr[j - 1] > key) {
      arr[j] = arr[j - 1];
      j--;
    }
      // insert key into its sorted position
    arr[j] = key;
  }
}

//===================================================================

int main (){
  size_t len;
  READ(int, arr, "%d", len);
  
  insertionSort(arr, len);
  PRINT_ARRAY(arr, "%d", len);
  
  free(arr);
  return 0;
}