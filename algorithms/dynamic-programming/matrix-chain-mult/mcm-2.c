/* 
  file: mcm-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description:
    Matrix chain multiplication using top-down DP approach.
    The input is stored in an array holding the dimensions of
    the matrices A₁, ..., Aₙ as follows: A₁ = dims[0] x dims[1],
    A₂ = dims[1] x dims[2], ..., Aₙ = dims[n-1] x dims[n].
    The output is the minimal cost of the matrix chain product
    A₁ * ... * Aₙ.
  Time complexity: O(n³)
*/

#include "../../../lib/clib/clib.h"
#include <stdint.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b))

//===================================================================
// Computes the minimum cost of multiplying a chain of matrices
// starting at index i and ending at index j
int computeMinCost(size_t *dims, size_t **minCosts, 
                   size_t i, size_t j) {

    // base case: the cost of multiplying a single matrix is 0
  if (i == j)
    return minCosts[i][j] = 0;
  
    // if available, return the minimum cost
  if (minCosts[i][j] != SIZE_MAX)
    return minCosts[i][j];

    // try all possible split points k and choose
    // the one that yields the minimum cost
  for (size_t k = i; k < j; k++) { 
    size_t cost = computeMinCost(dims, minCosts, i, k) 
                + computeMinCost(dims, minCosts, k + 1, j)  
                + dims[i - 1] * dims[k] * dims[j];
      // store the computed value for future use
    minCosts[i][j] = MIN(minCosts[i][j], cost);
  }
  return minCosts[i][j];
}

//===================================================================

int main() {
    
    // read the matrix dimensions
  READ(size_t, dims, "%zu", len);

    // create a table for memoization of subproblems
  CREATE_MATRIX(size_t, minCosts, len, len, SIZE_MAX);

  computeMinCost(dims, minCosts, 1, len - 1);

  printf("Min cost: %zu\n", minCosts[1][len - 1]);

  FREE_MATRIX(minCosts, len);
  free(dims);

  return 0;
}