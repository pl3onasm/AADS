/* file: countingsort-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: counting sort
     This is an adaptation so that it also works in cases where
     the array contains negative integers or is more efficient
     if the minimum value is much larger than 0.
   time complexity: O(n) provided that k = O(n)
*/

#include "../../../lib/clib.h"

//===================================================================
// Sorts an array of integers in ascending order
int *countingSort(int *arr, size_t n) {
    // find the minimum and maximum values to determine the range
  int min = arr[0], max = arr[0];
  for (size_t i = 1; i < n; i++) {
    min = MIN(min, arr[i]);
    max = MAX(max, arr[i]); 
  }
  int range = max - min;
    // count the number of occurences of each item and subtract
    // min from each item so that the counting array starts at 0
  size_t *counts = safeCalloc(range + 1, sizeof(size_t));
  for (size_t i = 0; i < n; i++) 
    counts[arr[i] - min]++;
    // compute the cumulative sums of the counts
  for (size_t i = 1; i <= range; i++) 
    counts[i] += counts[i - 1];
    // place the elements in their sorted positions
  int *sorted = safeCalloc(n, sizeof(int));
  for (size_t i = n; i--; ) {
    sorted[counts[arr[i] - min] - 1] = arr[i];
    counts[arr[i] - min]--;
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