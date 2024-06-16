/* 
  file: randomqsort.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: quicksort, using a random element as pivot
  time complexity: expected O(nlogn)
*/

#include "../../../lib/clib/clib.h"
#include <time.h>

//===================================================================
// Partitions an array around a random pivot
size_t partition(int *arr, size_t left, size_t right) {
  
  size_t i = left;
    // put random pivot at the end of the array
  SWAP(arr[left + rand() % (right - left)], arr[right - 1]);

    // keep swapping elements smaller than the pivot
    // loop invariant: arr[left:i-1] < pivot
  for (size_t j = left; j < right - 1; j++) {
    if (arr[j] < arr[right - 1]) {
      SWAP(arr[i], arr[j]);
      i++;
    }
  }
    // put pivot in its final sorted position
  SWAP(arr[i], arr[right - 1]);
  return i;
}

//===================================================================
// Sorts an array of integers in place in ascending order
void quickSort(int *arr, size_t left, size_t right){

    // if the array has more than one element
  if (left + 1 < right) {
      // partition the array around a random pivot
    size_t q = partition(arr, left, right);
      // recursively sort the two partitions
    quickSort(arr, left, q);
    quickSort(arr, q + 1, right);
  }
}

//===================================================================

int main (){
  srand(time(NULL));

  READ(int, arr, "%d", len);
  
  quickSort(arr, 0, len);
  PRINT_ARRAY(arr, "%d", len);
  
  free(arr);
  return 0;
}