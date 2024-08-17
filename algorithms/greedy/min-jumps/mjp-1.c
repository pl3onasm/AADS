/* 
  file: mjp-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: minimum jumps problem (MJP)
    using naive recursion 
  caution: may explode for large inputs    
*/ 

#include "../../../lib/clib/clib.h"
#include <stdint.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

//===================================================================
// Returns the minimum number of jumps needed to reach the
// destination, or SIZE_MAX if the destination is unreachable
size_t jump(size_t *jumps, size_t idx, size_t dest) {
    
    // base case: destination reached
  if (idx >= dest) 
    return 0;

    // recursive case: try all possible jumps from current 
    // position and return the minimum number of jumps
  size_t minJumps = SIZE_MAX;
  for (size_t i = 1; i <= jumps[idx]; i++) {
    size_t numJumps = jump(jumps, idx + i, dest);
    if (numJumps != SIZE_MAX)
      minJumps = MIN(minJumps, numJumps + 1);
  }

  return minJumps;
}

//===================================================================

int main() {

  READ(size_t, jumps, "%zu", n);

  size_t minJumps = jump(jumps, 0, n-1);

  printf(minJumps == SIZE_MAX ? "Not reachable.\n" 
                              : "%zu\n", minJumps);

  free(jumps);

  return 0;
}