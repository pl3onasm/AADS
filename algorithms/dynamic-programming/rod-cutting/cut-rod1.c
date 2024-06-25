/* 
  file: cut-rod1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description:
    This is a naive recursive implementation of 
    rod cutting. By induction we can prove T(n) = 2ⁿ, 
    where n is the length of the rod.
    The time complexity is therefore in O(2ⁿ).
    
  Use at your own risk for large values of n (n > 20).
  The author is not responsible for any damage, be it 
  physical, mental, or financial, that may result from
  using this code.
*/ 

#include "../../../lib/clib/clib.h"
#define MAX(a,b) ((a) > (b) ? (a) : (b))

//===================================================================
// Returns the maximum revenue that can be obtained by cutting 
// a rod of length n given the prices of each piece of the rod 
double cutRod(double *prices, size_t n) {
  
  if (n == 0)
    return 0; 

  double rev = -1;
  for (size_t i = 1; i <= n; i++) 
    rev = MAX(rev, prices[i] + cutRod(prices, n - i));

  return rev;
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

  printf("Rod length: %zu\n"
         "Maximum revenue: %.2lf\n", 
         n, cutRod(prices, n));

  free(prices);
  return 0;
}
