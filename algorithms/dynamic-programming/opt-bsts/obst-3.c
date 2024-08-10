/* 
  file: obst-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: optimal binary search tree problem,
    bottom-up dynamic programming approach
  time complexity: O(n^3)
*/

#include "../../../lib/clib/clib.h"
#include <float.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b))

//===================================================================
// Computes the expected search cost of an optimal binary search tree
// using a bottom-up dynamic programming approach
double computeOBST (size_t nProbs, double *probs, double **costs) {

  for (size_t i = 0; i < nProbs; ++i)
    costs[i][i] = probs[i];

  for (size_t l = 2; l <= nProbs; ++l) {
    for (size_t i = 0; i <= nProbs - l; ++i) {
      size_t j = i + l - 1;
      double sum = 0;
      for (size_t r = i; r <= j; ++r) {
        sum += probs[r];
        double cost = (r > i ? costs[i][r - 1] : 0) +
                      (r < j ? costs[r + 1][j] : 0);
        costs[i][j] = MIN(costs[i][j], cost);
      }
      costs[i][j] += sum;
    }
  }

  return costs[0][nProbs - 1];
}

//===================================================================

int main () {
  
  READ(double, probs, "%lf", nProbs);

  CREATE_MATRIX(double, costs, nProbs + 1, nProbs, DBL_MAX);
  
  printf("OBST cost: %.2lf\n", computeOBST(nProbs, probs, costs));

  FREE_MATRIX(costs, nProbs + 1);
  free(probs);

  return 0;
}