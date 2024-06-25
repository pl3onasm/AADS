/* 
  file: cut-rod4.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: extended bottom-up version to also print 
    an optimal solution
  Time complexity: O(n²)
*/ 

#include "../../../lib/clib/clib.h"

//===================================================================
// Returns the maximum revenue for a rod of length n given a price
// list and an array to store the computed revenues
double cutRod (double *prices, size_t n, 
               double *revenues, size_t *cuts){

    // if available, return the stored value
  if (revenues[n] >= 0)  
    return revenues[n];

    // otherwise compute the maximum revenue in
    // a bottom-up fashion
  for (size_t j = 1; j <= n; j++) {
    double rev = -1;
    for (size_t i = 1; i <= j; i++) {
      if (rev < prices[i] + revenues[j - i]) {
        rev = prices[i] + revenues[j - i];
        cuts[j] = i;
      }
    }
    
      // store the computed value for future use
    revenues[j] = rev;  
  }

  return revenues[n];
}

//===================================================================
// Print the optimal solution for a rod of length n
void printSolution (size_t *cuts, size_t n) {

  printf("Optimal cuts: ");
  while (n > 0) {
    printf(n - cuts[n] == 0 ? "%zu" : "%zu, ", cuts[n]);
    n = n - cuts[n];    
  }
  printf("\n");
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

    // create an array to store the computed revenues and
    // an array to store the optimal cuts
    // initialize the revenues with -1 except for the first element
    // since the revenue for a rod of length 0 is 0
  CREATE_ARRAY(double, revenues, n + 1);
  CREATE_ARRAY(size_t, cuts, n + 1);
  memset(revenues + 1, -1, sizeof(double) * n);

  printf("Rod length: %zu\n"
         "Maximum revenue: %.2lf\n", 
         n, cutRod(prices, n, revenues, cuts));

  printSolution(cuts, n);

  free(prices);
  free(revenues);
  free(cuts);

  return 0;
}
