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

    // if available, return the stored value
  if (revenues[n] >= 0)  
    return revenues[n];

    // otherwise compute the maximum revenue in
    // a bottom-up fashion
  for (size_t j = 1; j <= n; j++) {
    double rev = -1;
    for (size_t i = 1; i <= j; i++) 
      rev = MAX(rev, prices[i] + revenues[j - i]);  
    
      // store the computed value for future use
    revenues[j] = rev;  
  }
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
    exit(EXIT_FAILURE);
  }

    // create an array to store the computed revenues 
  CREATE_ARRAY(double, revenues, n + 1);

    // initialize the revenues to -1, except for
    // the first element which is 0 as the revenue
    // for a rod of length 0 is 0
  memset(revenues + 1, -1, sizeof(double) * n);

  printf("Rod length: %zu\n"
         "Maximum revenue: %.2lf\n", 
         n, cutRod(prices, n, revenues));

  free(prices);
  free(revenues);

  return 0;
}
