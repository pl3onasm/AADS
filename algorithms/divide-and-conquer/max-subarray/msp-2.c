/* file: msp-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Maximum subarray problem using 
    a divide and conquer approach
   time complexity: O(nlogn)
*/

#include "../../../lib/clib/clib.h"

typedef struct {
  int low, high, sum;
} Sub;

void printResult (int *arr, Sub max) {
  /* prints the result */
  printf("Maximum sum: %d\n", max.sum);
  printf("Maximum subarray: \n["); 
  for (int i = max.low; i <= max.high; ++i) {
    printf("%d", arr[i]);
    if (i < max.high) printf(", ");
  }
  printf("]\n");
}

void getMaxCrossSub (int *arr, int mid, Sub *max) {
  /* determines the maximum subarray crossing the midpoint */
  int sum = 0, leftSum = INT_MIN, rightSum = INT_MIN;
  int left = max->low, right = max->high;
  // get maximum subarray on the left side of the midpoint
  for (int i = mid; i >= left; --i) {
    sum += arr[i];
    if (sum > leftSum) {
      leftSum = sum;
      max->low = i;
    }
  }
  // get maximum subarray on the right side of the midpoint
  sum = 0;
  for (int j = mid + 1; j <= right; ++j) {
    sum += arr[j];
    if (sum > rightSum) {
      rightSum = sum;
      max->high = j;
    }
  }
  max->sum = leftSum + rightSum; 
}

void getMaxSub (int *arr, Sub *max) {
  /* computes the maximum subarray */
  int low = max->low, high = max->high;
  if (low == high) max->sum = arr[low];
  else {
    int mid = low + (high - low) / 2;
    Sub left = {low, mid, 0};
    getMaxSub(arr, &left);
    Sub right = {mid + 1, high, 0};
    getMaxSub(arr, &right);
    Sub cross = {low, high, 0};
    getMaxCrossSub(arr, mid, &cross);

    if (left.sum >= right.sum && left.sum >= cross.sum) *max = left;
    else if (right.sum >= left.sum && right.sum >= cross.sum) *max = right;
    else *max = cross;
  }
}

int main(int argc, char *argv[]) {
  int n;
  (void)! scanf("%d", &n);

  CREATE_ARRAY(int, arr, n);
  READ_ARRAY(arr, "%d", n);

  Sub max = {0, n-1, 0};
  
  getMaxSub(arr, &max);
  printResult(arr, max);
  
  free(arr);
  return 0;
}