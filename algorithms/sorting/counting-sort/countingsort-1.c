/* 
  file: countingsort-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: counting sort
    This version can only be used to sort arrays
    containing non-negative integers.
  time complexity: O(n) provided that k = O(n)
*/

#include "../../../lib/clib.h"

//===================================================================
// Sorts an array of non-negative integers in ascending order 
int *countingSort(int *arr, size_t n) {
    // find the maximum value k
  int max = arr[0];
  for (size_t i = 1; i < n; i++) 
    max = MAX(max, arr[i]);
    // count the number of occurences of each item
  size_t *counts = safeCalloc(max + 1, sizeof(size_t));
  for (size_t i = 0; i < n; i++) 
    counts[arr[i]]++;
    // compute the cumulative sums of the counts
  for (size_t i = 1; i <= max; i++) 
    counts[i] += counts[i - 1];
    // place the items in their correct positions
  int *sorted = safeCalloc(n, sizeof(int));
  for (size_t i = n; i--; ) {
    sorted[counts[arr[i]] - 1] = arr[i];
    counts[arr[i]]--;
  }
  free(counts);
  return sorted;
}

//===================================================================

int main (){
  
  READ(int, arr, "%d", len);

  int *sorted = countingSort(arr, len);
  PRINT_ARRAY(sorted, "%d", len);
  
  free(sorted);
  free(arr);
  return 0;
}