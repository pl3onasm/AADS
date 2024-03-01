/* file: bubblesort.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: bubble sort
   time complexity: O(n^2)
*/

#include "../../../lib/clib/clib.h"

void bubbleSort(int *arr, size_t n) {
  /* sorts an array of size n using bubble sort */
  bool swapped = true;
  for (size_t i = 0; i < n; i++) {
    // if no swaps were made in the previous iteration, 
    // the array is sorted
    if (!swapped) break;
    swapped = false;
    // bubble up smaller elements to the front of the array
    for (size_t j = n - 1; j > i; j--) {
      if (arr[j] < arr[j - 1]) {
        SWAP(arr[j], arr[j - 1]);
        swapped = true;
      }
    }
  }
}

int main (){
  size_t len;
  READ(int, arr, "%d", len);
  bubbleSort(arr, len);
  PRINT_ARRAY(arr, "%d", len);
  free(arr);
  return 0;
}