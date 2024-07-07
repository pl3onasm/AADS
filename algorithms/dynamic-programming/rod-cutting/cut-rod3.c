/* 
  file: cut-rod3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: bottom-up DP approach to rod cutting
  Time complexity: O(n²)
*/ 

#include "../../../lib/clib/clib.h"
#define MAX(a,b) ((a) > (b) ? (a) : (b))

//===================================================================
// Returns the maximum revenue for a rod of length n given a price
// list and an array to store the computed revenues
double cutRod (double *prices, size_t n, double *revenues){

    // base case: revenue for a rod of length 0 is 0
  revenues[0] = 0;

    // compute maximum revenue for each rod length
  for (size_t j = 1; j <= n; j++) 
    for (size_t i = 1; i <= j; i++) 
      revenues[j] = MAX(revenues[j], prices[i] + revenues[j - i]);

  return revenues[n];
}

//===================================================================

int main() {

    // read the requested rod length
  size_t n;
  assert(scanf("%zu", &n) == 1);

    // read the prices for each rod length
  READ(double, prices, "%lf", len);

  if (n > len) {
    fprintf(stderr, "Requested rod length is greater " 
                    "than the number of prices\n");
    free(prices);
    exit(EXIT_FAILURE);
  }

    // create an array to store the computed revenues 
  CREATE_ARRAY(double, revenues, n + 1, -1);

  printf("Rod length: %zu\n"
         "Maximum revenue: %.2lf\n", 
         n, cutRod(prices, n, revenues));

  free(prices);
  free(revenues);

  return 0;
}
