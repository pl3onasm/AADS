/* 
  file: obst-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: optimal binary search tree problem,
    bottom-up dynamic programming approach with reconstruction
    of the tree structure
  time complexity: O(n^3)
*/

#include "../../../lib/clib/clib.h"
#include <float.h>

//===================================================================
// Computes the expected search cost of an optimal binary search tree
// and stores the optimal roots of the subtrees
void computeOBST (size_t nProbs, double *probs, double **costs, 
                    size_t **roots) {

  for (size_t i = 0; i < nProbs; ++i) {
    costs[i][i] = probs[i];
    roots[i][i] = i;
  }

  for (size_t l = 2; l <= nProbs; ++l) {
    for (size_t i = 0; i <= nProbs - l; ++i) {
      size_t j = i + l - 1;
      double sum = 0;
      for (size_t r = i; r <= j; ++r) {
        sum += probs[r];
        double cost = (r > i ? costs[i][r - 1] : 0) +
                      (r < j ? costs[r + 1][j] : 0);
        if (cost < costs[i][j]) {
          costs[i][j] = cost;
          roots[i][j] = r;
        }
      }
      costs[i][j] += sum;
    }
  }
}

//===================================================================
// Reconstructs the optimal binary search tree structure
void constructOBST (size_t i, size_t j, size_t level,
                    size_t **roots) {

  if (j == i - 1) {
    return;
  } else {
    constructOBST(i, roots[i][j] - 1, level + 1, roots);
    for (size_t k = 0; k < level; ++k)
      printf("-");
    printf(level ? "| K%zu\n" : "K%zu\n", roots[i][j]);
    constructOBST(roots[i][j] + 1, j, level + 1, roots);
  }

}

//===================================================================

int main () {
  
  READ(double, probs, "%lf", nProbs);

  CREATE_MATRIX(double, costs, nProbs + 1, nProbs, DBL_MAX);

  CREATE_MATRIX(size_t, roots, nProbs, nProbs, 0);

  computeOBST(nProbs, probs, costs, roots);
  
  printf("OBST cost: %.2lf\n\n"
         "OBST structure:\n\n", costs[0][nProbs - 1]);

  constructOBST(0, nProbs - 1, 0, roots);

  FREE_MATRIX(costs, nProbs + 1);
  FREE_MATRIX(roots, nProbs);
  free(probs);

  return 0;
}