/* file: msp-3.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Maximum subarray problem using 
    an iterative linear approach
   time complexity: O(n)
*/

#include "../../../lib/clib/clib.h"

void printResult (int *arr, int l, int h, int max) {
  /* prints the result */
  printf("Maximum sum: %d\n", max);
  printf("Maximum subarray: \n[");
  for (int i = l; i <= h; ++i) {
    printf("%d", arr[i]);
    if (i < h) printf(", ");
  }
  printf("]\n");
}

int maxSubarray(int *arr, int n, int *l, int *h) {
  /* computes the maximum subarray */
  int max = INT_MIN, sum = 0, low = 0;
  for (int i = 0; i < n; ++i) {
    sum += arr[i];
    if (sum > max) { // update max
      max = sum;
      *h = i;
      *l = low;
    }
    if (sum < 0) {   // reset sum
      sum = 0;
      low = i+1;
    }
  }
  return max;
}

int main(int argc, char *argv[]) {
  int n;
  (void)! scanf("%d", &n);

  CREATE_ARRAY(int, arr, n);
  READ_ARRAY(arr, "%d", n);

  int l = 0, h = n-1;  // low and high indices
  
  int max = maxSubarray(arr, n, &l, &h);
  printResult(arr, l, h, max);

  free(arr);
  return 0; 
}