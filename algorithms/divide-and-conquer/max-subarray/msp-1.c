/* file: msp-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Maximum subarray problem, naive approach
   time complexity: Θ(n²)
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

int getMaxSub(int *arr, int n, int *l, int *h) {
  /* determines the maximum subarray */
  int max = INT_MIN;
  for (int i = 0; i < n; ++i) {
    int sum = 0;
    for (int j = i; j < n; ++j) {
      sum += arr[j];
      if (sum > max) {
        max = sum;
        *l = i;
        *h = j;
      }
    }
  }
  return max;
}

int main(int argc, char *argv[]) {
  int n; 
  (void)! scanf("%d", &n);
  
  CREATE_ARRAY(int, arr, n);
  READ_ARRAY(arr, "%d", n);

  int l = 0, h = n - 1;  // low and high indices
  
  int max = getMaxSub(arr, n, &l, &h);
  printResult(arr, l, h, max);

  free(arr);
  return 0; 
}
  

  