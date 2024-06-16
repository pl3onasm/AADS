/* file: msp-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Maximum subarray problem, naive approach
   time complexity: Θ(n²)
*/

#include "../../../lib/clib/clib.h"
#include <limits.h>

void printResult (int *arr, size_t l, size_t h, int max) {
  /* prints the result */
  printf("Maximum sum: %d\n", max);
  printf("Maximum subarray: \n[");
  for (size_t i = l; i <= h; ++i) {
    printf("%d", arr[i]);
    if (i < h) printf(", ");
  }
  printf("]\n");
}

int getMaxSub(int *arr, size_t n, size_t *l, size_t *h) {
  /* determines the maximum subarray */
  int max = INT_MIN;
  for (size_t i = 0; i < n; ++i) {
    int sum = 0;
    for (size_t j = i; j < n; ++j) {
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
  size_t n; 
  (void)! scanf("%zu", &n);
  
  CREATE_ARRAY(int, arr, n);
  READ_ARRAY(arr, "%d", n);

  size_t l = 0, h = n - 1;  // low and high indices
  
  int max = getMaxSub(arr, n, &l, &h);
  printResult(arr, l, h, max);

  free(arr);
  return 0; 
}
  

  