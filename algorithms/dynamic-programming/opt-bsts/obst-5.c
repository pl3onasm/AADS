/* 
  file: obst-5.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: optimal binary search tree problem,
    bottom-up dynamic programming approach with Knuth's
    optimization to reduce time complexity to O(n^2)
  time complexity: O(n^2)
*/

#include "../../../lib/clib/clib.h"
#include <float.h>

//===================================================================
// Computes the expected search cost of an optimal binary search tree
// and stores the optimal roots of the subtrees
// Knuth's optimization reduces time complexity from O(n^3) to O(n^2)
void computeOBST (size_t nProbs, double *probs, double **costs, 
                  size_t **roots, double **sums) {

  for (size_t i = 0; i < nProbs; ++i) {
    costs[i][i] = probs[i];
    roots[i][i] = i;
  }

  for (size_t l = 2; l <= nProbs; ++l) {
    for (size_t i = 0; i <= nProbs - l; ++i) {
      size_t j = i + l - 1;
      for (size_t r = roots[i][j - 1]; r <= roots[i + 1][j]; ++r) {
        double leftCost = r == i ? 0 : costs[i][r - 1];
        double rightCost = r == j ? 0 : costs[r + 1][j];
        double minCost = leftCost + rightCost + sums[i][j];
        if (minCost < costs[i][j]) {
          costs[i][j] = minCost;
          roots[i][j] = r;
        }
      }
    }
  }
}

//===================================================================
// Reconstructs the optimal binary search tree structure from the
// roots of the subtrees
void constructOBST (size_t i, size_t j, size_t level,
                    size_t **roots) {

  if (j == i - 1) {
    return;
  } else {
    constructOBST(i, roots[i][j] - 1, level + 1, roots);
    for (size_t k = 0; k < level; ++k)
      printf("-");
    printf(level ? "| K%zu (%zu)\n" : "K%zu (%zu)\n", 
           roots[i][j], level);
    constructOBST(roots[i][j] + 1, j, level + 1, roots);
  }
}

//===================================================================
// Precomputes the sums of the probabilities of the keys over each
// possible range of keys
void computeSums (size_t nProbs, double *probs, double **sums) {

  for (size_t i = 0; i < nProbs; ++i) {
    sums[i][i] = probs[i];
    for (size_t j = i + 1; j < nProbs; ++j) 
      sums[i][j] = sums[i][j - 1] + probs[j];
  }
}

//===================================================================

int main () {
  
  READ(double, probs, "%lf", nProbs);

  CREATE_MATRIX(double, costs, nProbs + 1, nProbs, DBL_MAX);

  CREATE_MATRIX(size_t, roots, nProbs, nProbs, 0);

  CREATE_MATRIX(double, sums, nProbs + 1, nProbs, 0);

  computeSums(nProbs, probs, sums);

  computeOBST(nProbs, probs, costs, roots, sums);
  
  printf("OBST cost: %.2lf\n\n"
         "OBST structure:\n\n", costs[0][nProbs - 1]);

  constructOBST(0, nProbs - 1, 0, roots);

  FREE_MATRIX(sums, nProbs + 1);
  FREE_MATRIX(costs, nProbs + 1);
  FREE_MATRIX(roots, nProbs);
  free(probs);

  return 0;
}