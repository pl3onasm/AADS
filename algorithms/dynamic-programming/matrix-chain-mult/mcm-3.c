/* 
  file: mcm-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description:
    Matrix chain multiplication using bottom-up DP approach
    The input is stored in an array holding the dimensions of
    the matrices A₁, ..., Aₙ as follows: A₁ = dims[0] x dims[1],
    A₂ = dims[1] x dims[2], ..., Aₙ = dims[n-1] x dims[n].
    The output is the minimal cost of the matrix chain product
    A₁ * ... * Aₙ and an optimal parenthesization.
  Time complexity: O(n³)
*/

#include "../../../lib/clib/clib.h"
#include <stdint.h>

//===================================================================
// Computes the minimum costs and optimal split points for all
// subproblems in bottom-up fashion
void computeMinCost (size_t *dims, size_t **minCosts, 
                     size_t **splitPoints, size_t len) {
  
    // fill the table in order of increasing chain length l
  for (size_t l = 2; l <= len; l++) { 
    for (size_t i = 1; i < len - l + 1; i++) {   
        // chain starts at i and ends at j
      size_t j = i + l - 1;                    
      minCosts[i][j] = SIZE_MAX; 
        // try all possible split points k and choose
        // the one that yields the minimum cost          
      for (size_t k = i; k < j; k++) { 
        size_t cost = minCosts[i][k] + minCosts[k + 1][j]  
                    + dims[i-1] * dims[k] * dims[j];
        if (cost < minCosts[i][j]) { 
          minCosts[i][j] = cost;
          splitPoints[i][j] = k;
        }
      }
    }
  }
}

//===================================================================
// Prints the optimal parenthesization of the matrix chain
// starting at i and ending at j
void printOptimalParens (size_t **splitPoints, size_t i, size_t j) {
  if (i == j) {
    printf("A%zu", i);
  } else {
    printf("(");
    printOptimalParens(splitPoints, i, splitPoints[i][j]);
    printf(" x ");
    printOptimalParens(splitPoints, splitPoints[i][j] + 1, j);
    printf(")");
  }
}

//===================================================================

int main () {
    // read the matrix dimensions
  READ(size_t, dims, "%zu", len);

    // create a table for memoization of subproblems
    // and a table for storing the optimal split points
  CREATE_MATRIX(size_t, minCosts, len, len);
  CREATE_MATRIX(size_t, splitPoints, len, len);

  computeMinCost(dims, minCosts, splitPoints, len);

  printf("Min cost: %zu\n\n", minCosts[1][len - 1]);

  printf("An optimal parenthesization:\n");
  printOptimalParens(splitPoints, 1, len - 1);
  printf("\n");

  FREE_MATRIX(minCosts, len);
  FREE_MATRIX(splitPoints, len);
  free(dims);

  return 0;
}