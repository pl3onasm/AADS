/* 
  file: mjp-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: minimum jumps problem (MJP)
    using a bottom-up DP approach
  time complexity: O(n^2)
*/ 

#include "../../../lib/clib.h"

//===================================================================
// Returns the minimum number of jumps needed to reach the
// destination, or SIZE_MAX if the destination is unreachable.
// The moves array is filled with the indices from which the
// maximum jumps were taken
size_t jump(size_t *jumps, size_t n, size_t *moves) {
    
  CREATE_ARRAY(size_t, dp, n, SIZE_MAX);

    // base case: no jumps needed to reach the first index
  dp[0] = 0;

    // gradually increase the subproblem size by 
    // increasing the destination index by 1
  for (size_t i = 1; i < n; i++) {
      // try all possible jumps from previous indices
    for (size_t j = 0; j < i; j++) 
      if (j + jumps[j] >= i && dp[j] + 1 < dp[i]) {
        dp[i] = dp[j] + 1;
        moves[i] = j;
      }
    
    if (dp[i] == SIZE_MAX) 
      // can't reach destination
      break;
  }

  size_t minJumps = dp[n - 1];

  free(dp);
  return minJumps;
}

//===================================================================
// Reconstructs the path to the destination by following the indices
// in the moves array
void recPath(size_t *moves, size_t idx, size_t dest) {

    // base case: reached the first index
  if (idx == 0) {
    printf("0, ");
    return;
  }

  recPath(moves, moves[idx], dest);
  printf(idx == dest ? "%zu\n" : "%zu, ", idx);
}

//===================================================================
// Shows the minimum number of jumps and the path to the destination
// if the destination is reachable
void showJumps(size_t *moves, size_t n, size_t minJumps) {
  
  if (minJumps == SIZE_MAX) {
    printf("Destination not reachable.\n");
    return;
  }

  printf("Min jumps: %zu\nPath:\n  ", minJumps);
  recPath(moves, n - 1, n - 1);
}

//===================================================================

int main() {

  READ(size_t, jumps, "%zu", n);

  if (n == 1 || n == 0) {
    printf("Min jumps: 0\n");
    free(jumps);
    return 0;
  }

  CREATE_ARRAY(size_t, moves, n, 0);

  size_t minJumps = jump(jumps, n, moves);

  showJumps(moves, n, minJumps);

  free(jumps);
  free(moves);

  return 0;
}