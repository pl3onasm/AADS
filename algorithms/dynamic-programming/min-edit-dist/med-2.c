/* 
  file: med-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: minimum edit distance
    using top-down DP with memoization, no path reconstruction
  time complexity: O(n*m)
*/

#include "../../../lib/clib/clib.h"
#include <stdint.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b))
typedef enum { COPY, INSERT, DELETE, REPLACE, SWAP, KILL } op;

//===================================================================
// Naive recursive approach to compute the minimum edit distance
size_t computeMED(string *src, string *tgt, size_t i, size_t j, 
                  size_t *costs, size_t **dp) {
    
    // return the value if it has already been computed
  if (dp[i][j] != SIZE_MAX) return dp[i][j];

  size_t med;
  
    // source is empty: insert remaining target chars
  if (i == 0) return dp[i][j] = j * costs[INSERT];

    // check if killing last remaining chars is cheaper
  if (i == strLen(src) && j == strLen(tgt))
    for (size_t k = 0; k < strLen(src); ++k) {
      med = computeMED(src, tgt, k, j, costs, dp);
      dp[i][j] = MIN(dp[i][j], med + costs[KILL]);
    }

    // target is empty: delete all remaining source chars 
    // or kill if it's the only and cheapest operation
  if (j == 0) {
    if (dp[i][j] == costs[KILL] && i * costs[DELETE] > costs[KILL])
      return dp[i][j];
    return dp[i][j] = i * costs[DELETE];
  }

  if (charAt(src, i - 1) == charAt(tgt, j - 1)) {
    med = computeMED(src, tgt, i - 1, j - 1, costs, dp);
    dp[i][j] = MIN(dp[i][j], med + costs[COPY]);
  }

  if (i > 1 && j > 1 && charAt(src, i - 1) == charAt(tgt, j - 2) 
      && charAt(src, i - 2) == charAt(tgt, j - 1)) {
    med = computeMED(src, tgt, i - 2, j - 2, costs, dp);
    dp[i][j] = MIN(dp[i][j], med + costs[SWAP]);
  }

  med = computeMED(src, tgt, i, j - 1, costs, dp);
  dp[i][j] = MIN(dp[i][j], med + costs[INSERT]);

  med = computeMED(src, tgt, i - 1, j, costs, dp);
  dp[i][j] = MIN(dp[i][j], med + costs[DELETE]);

  if (charAt(src, i - 1) != charAt(tgt, j - 1)) {
    med = computeMED(src, tgt, i - 1, j - 1, costs, dp);
    dp[i][j] = MIN(dp[i][j], med + costs[REPLACE]);
  }

  return dp[i][j];
}

//===================================================================

int main () {
  
    // read source and target strings
  READ_STRING(src, '\n');
  READ_STRING(tgt, '\n');

    // read operation costs 
  size_t costs[6];
  READ_ARRAY(costs, "%zu", 6);

    // create memoization table
  CREATE_MATRIX(size_t, dp, strLen(src) + 1, 
                strLen(tgt) + 1, SIZE_MAX);

    // compute minimum edit distance
  printf("Minimum edit distance: %zu\n", 
    computeMED(src, tgt, strLen(src), strLen(tgt), costs, dp));
  
    // deallocate memory
  FREE_MATRIX(dp, strLen(src) + 1);
  freeString(src);
  freeString(tgt);

  return 0;
}