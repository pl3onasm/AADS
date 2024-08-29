/* 
  file: mjp-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: minimum jumps problem (MJP)
               using a greedy approach
  time complexity: O(n)
*/ 

#include "../../../lib/clib.h"

//===================================================================
// Returns the minimum number of jumps needed to reach the 
// destination, or SIZE_MAX if the destination is unreachable.
// The moves array is filled with the indices from which the
// maximum jumps were taken
size_t jump(size_t *jumps, size_t n, size_t *moves) {

  size_t nJumps = 0, lastJump = 0, newRange = 0, oldRange = 0;

    // keep taking the maximum jump from the range opened 
    // by the last jump until we reach the destination
  for (size_t idx = 0; idx < n; idx++) {

      // update the maximum range we can reach from the
      // current index
    if (idx + jumps[idx] > newRange) {
      newRange = idx + jumps[idx];
      lastJump = idx;
    }

    if (idx == oldRange) {
      if (newRange == oldRange) 
        // if we didn't move forward from the last jump
        // then the destination is unreachable
        return SIZE_MAX;
      
      oldRange = newRange;
      nJumps++;

        // record the index from which the 
        // last max jump was taken
      moves[idx] = lastJump;

        // have we reached the destination?
      if (newRange >= n - 1) 
        return nJumps;
    }
  }

  return nJumps;
}

//===================================================================
// Shows the minimum number of jumps and the path to the destination
// by following the indices in the moves array if the destination
// is reachable
void showJumps(size_t *moves, size_t n, size_t minJumps) {
  
  if (minJumps == SIZE_MAX) {
    printf("Destination not reachable.\n");
    return;
  }

  printf("Min jumps: %zu\nPath:\n  ", minJumps);
  printf("0");
  for (size_t idx = 0; idx < n; idx++) 
    if (moves[idx]) printf(", %zu", moves[idx]);
  printf(", %zu\n", n - 1);
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