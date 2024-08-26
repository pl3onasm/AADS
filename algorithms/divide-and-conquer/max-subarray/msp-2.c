/* 
  file: msp-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: Maximum subarray problem using 
    a divide and conquer approach
  time complexity: O(nlogn)
*/

#include "../../../lib/clib.h"
#include <limits.h>

//===================================================================
// Subarray data structure
typedef struct {
  size_t low, high;     // bounds of the subarray
  int sum;              // sum of the subarray
} Sub;

//===================================================================
// Prints the maximum sum and a subarray having this sum
void printMaxSub (int *arr, Sub max) {
  printf("Maximum sum: %d\n"
         "A subarray with the maximum sum:\n  ", max.sum);
  for (size_t i = max.low; i <= max.high; ++i) 
    printf(i == max.high ? "%d\n" : "%d, ", arr[i]);
}

//===================================================================
// Computes the maximum subarray crossing the midpoint
void getMaxCrossSub (int *arr, size_t mid, Sub *max) {
  int sum = 0, leftSum = INT_MIN, rightSum = INT_MIN;
  size_t left = max->low, right = max->high;
    // get maximum subarray on the left side of the midpoint
  for (size_t i = mid + 1; i >= left + 1 && i--; ) {
    sum += arr[i];
    if (sum > leftSum) {
      leftSum = sum;
      max->low = i;
    }
  }
    // get maximum subarray on the right side of the midpoint
  sum = 0;
  for (size_t j = mid + 1; j <= right; ++j) {
    sum += arr[j];
    if (sum > rightSum) {
      rightSum = sum;
      max->high = j;
    }
  }
    // sum of the maximum subarray crossing the midpoint
  max->sum = leftSum + rightSum; 
}

//===================================================================
// Computes the maximum subarray sum and determines the bounds of
// a subarray with this sum
void getMaxSub (int *arr, Sub *max) {
  size_t low = max->low, high = max->high;
  if (low == high) 
    max->sum = arr[low];
  else {
      // DIVIDE the array in two halves
    size_t mid = low + (high - low) / 2;
      // CONQUER the left and right subarrays
    Sub left = {low, mid, 0};
    getMaxSub(arr, &left);
    Sub right = {mid + 1, high, 0};
    getMaxSub(arr, &right);
      // COMBINE the solutions
    Sub cross = {low, high, 0};
    getMaxCrossSub(arr, mid, &cross);

    if (left.sum >= right.sum && left.sum >= cross.sum) 
      *max = left;
    else if (right.sum >= left.sum && right.sum >= cross.sum) 
      *max = right;
    else 
      *max = cross;
  }
}

//===================================================================

int main() {
 
  READ(int, arr, "%d", len);

  Sub max = {0, len - 1, 0};
  
  getMaxSub(arr, &max);
  printMaxSub(arr, max);
  
  free(arr);
  return 0;
}