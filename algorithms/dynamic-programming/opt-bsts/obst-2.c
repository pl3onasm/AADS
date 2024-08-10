/* 
  file: obst-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: optimal binary search tree problem,
    top-down dynamic programming approach
  time complexity: O(n^3)
*/

#include "../../../lib/clib/clib.h"
#include <float.h>
#include <stdint.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b))

//===================================================================
// Computes the expected search cost of an optimal binary search tree
// using a top-down dynamic programming approach
double computeOBST (size_t i, size_t j, double *probs, 
                    double **costs) {

  if (j == SIZE_MAX) 
    return 0;

  if (costs[i][j] != -1)
    return costs[i][j];

  if (j == i - 1) 
    return costs[i][j] = 0;

  double minCost = DBL_MAX, sum = 0;

  for (size_t r = i; r <= j; ++r) {
    sum += probs[r];
    double cost = computeOBST(i, r - 1, probs, costs) +
                  computeOBST(r + 1, j, probs, costs);
    minCost = MIN(minCost, cost);
  }

  return costs[i][j] = minCost + sum;
}

//===================================================================

int main () {
  
  READ(double, probs, "%lf", nProbs);

  CREATE_MATRIX(double, costs, nProbs + 1, nProbs, -1);
  
  printf("OBST cost: %.2lf\n", 
          computeOBST(0, nProbs - 1, probs, costs));

  FREE_MATRIX(costs, nProbs + 1);
  free(probs);

  return 0;
}