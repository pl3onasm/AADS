/* 
  file: msp-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: Maximum subarray problem, naive approach
  time complexity: Θ(n²)
*/

#include "../../../lib/clib.h"
#include <limits.h>

//===================================================================
// Prints the maximum sum and a subarray having this sum
void printMaxSub (int *arr, size_t low, size_t high, int max) {
  printf("Maximum sum: %d\n"
         "A subarray with the maximum sum:\n  ", max);
  for (size_t i = low; i <= high; ++i) 
    printf(i == high ? "%d\n" : "%d, ", arr[i]);
}

//===================================================================
// Returns the maximum subarray sum and determines the bounds of
// a subarray with this sum
int getMaxSub(int *arr, size_t len, size_t *low, size_t *high) {
  int maxSum = INT_MIN;
  for (size_t i = 0; i < len; ++i) {
    int tempSum = 0;
    for (size_t j = i; j < len; ++j) {
      tempSum += arr[j];
      if (tempSum > maxSum) {
        maxSum = tempSum;
        *low = i;
        *high = j;
      }
    }
  }
  return maxSum;
}

//===================================================================

int main() {
  
  READ(int, arr, "%d", len);

  size_t low, high;
  
  int max = getMaxSub(arr, len, &low, &high);

  printMaxSub(arr, low, high, max);

  free(arr);
  return 0; 
}
  

  