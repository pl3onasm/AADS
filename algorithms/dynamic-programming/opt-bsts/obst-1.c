/* 
  file: obst-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: optimal binary search tree problem,
    naive recursive approach
  time complexity: exponential
  caution: may explode for large inputs
*/

#include "../../../lib/clib/clib.h"
#include <float.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b))

//===================================================================
// Computes the expected search cost of an optimal binary search tree
// using a naive recursive approach
double computeOBST (size_t i, size_t j, double *probs) {

  if (j == i - 1) 
    return 0;

  double minCost = DBL_MAX, sum = 0;

  for (size_t r = i; r <= j; ++r) {
    sum += probs[r];
    double cost = computeOBST(i, r - 1, probs) +
                  computeOBST(r + 1, j, probs);
    minCost = MIN(minCost, cost);
  }

  return minCost + sum;
}

//===================================================================

int main () {
  
  READ(double, probs, "%lf", nProbs);

  printf("OBST cost: %.2lf\n", computeOBST(0, nProbs - 1, probs));
  
  free(probs);

  return 0;
}