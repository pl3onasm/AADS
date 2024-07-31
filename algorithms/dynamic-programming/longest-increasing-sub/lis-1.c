/* 
  file: lis-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: longest increasing subsequence
    naive recursive implementation
  time complexity: O(2^n)
  Use at your own risk for n > 20
*/ 

#include "../../../lib/clib/clib.h"
#include <stdint.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))

//===================================================================
// Returns the length of the longest increasing subsequence
size_t computeLis (int *arr, size_t i, size_t j) {
  
  if (i == 0)
      // base case: no more elements to consider
    return 0;

  if (j == SIZE_MAX || arr[i - 1] <= arr[j])
      // take maximum of including or excluding current element
    return MAX(1 + computeLis(arr, i - 1, i - 1),
               computeLis(arr, i - 1, j));
  else 
      // exclude current element
    return computeLis(arr, i - 1, j);
}

//===================================================================

int main () {

  READ(int, arr, "%d", len);

  printf("Max length: %zu\n", computeLis(arr, len, SIZE_MAX));

  free(arr);
  
  return 0;
}

