/* 
  file: cut-rod2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: top-down DP approach to rod cutting
  Time complexity: O(n²)
*/ 

#include "../../../lib/clib/clib.h"
#include <float.h>
#define MAX(a,b) ((a) > (b) ? (a) : (b))

//===================================================================
// Returns the maximum revenue for a rod of length n given a price
// list and an array to store the computed revenues
double cutRod (double *prices, size_t n, double *revenues){

    // if available, return the stored value
  if (revenues[n] >= 0)  
    return revenues[n];

    // base case: revenue for a rod of length 0 is 0
  if (n == 0)
    return revenues[n] = 0;

    // compute the maximum revenue for a rod of length n
  double rev = -1;
  for (size_t i = 1; i <= n; i++) 
    rev = MAX(rev, prices[i] + cutRod(prices, n - i, revenues));
    
    // store the computed value for future use and return it
  return revenues[n] = rev;  
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