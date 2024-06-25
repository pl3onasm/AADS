/* 
  file: mcm-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description:
    Matrix chain multiplication using naive recursion.
    The input is stored in an array holding the dimensions of
    the matrices A₁, ..., Aₙ as follows: A₁ = dims[0] x dims[1],
    A₂ = dims[1] x dims[2], ..., Aₙ = dims[n-1] x dims[n].
    The output is the minimal cost of the matrix chain product
    A₁ * ... * Aₙ.
  Time complexity: O(2ⁿ)

  Use at your own risk for large values of n (n > 20).
  The author is not responsible for any damage, be it 
  physical, mental, or financial, that may result from
  using this code.
*/

#include "../../../lib/clib/clib.h"
#include <stdint.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b));

//===================================================================
// Compute the minimum cost of multiplying a chain of 
// matrices starting at index i and ending at index j.
size_t computeMinCost (size_t *dims, size_t i, size_t j) {
  
  if (i == j)
      // cost of multiplying a single matrix is 0 
    return 0;   
  else {
    size_t minCost = SIZE_MAX;
      // try all possible split points k and choose 
      // the one that yields the minimum cost
    for (size_t k = i; k < j; k++) { 
      size_t cost = computeMinCost(dims, i, k) 
                  + computeMinCost(dims, k + 1, j) 
                  + dims[i - 1] * dims[k] * dims[j];
      minCost = MIN(minCost, cost);
    }
    return minCost;
  }
}

//===================================================================

int main() {
  
    // read the matrix dimensions
  READ(size_t, dims, "%zu", len);

  printf("Min cost: %zu\n", computeMinCost(dims, 1, len - 1));

  free(dims);

  return 0;
}
