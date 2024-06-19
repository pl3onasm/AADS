/* 
  file: cows.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: 
    this an example of binary search, where we are looking  
    for the upper bound on the distance between two cows 
    such that all cows can be placed at least that far apart.
  time complexity: 
    O(n log(n))
*/

#include "../../../../lib/clib/clib.h"

//===================================================================
// Comparison function for qsort
int cmpPositions (const void * a, const void * b) {
  return ( *(size_t*)a - *(size_t*)b );
}

//===================================================================
// Checks whether it is possible to place the cows in the stalls
// such that the distance between any two of them is at least minDist
bool arePlaceable (size_t *positions, size_t nStalls, size_t nCows, 
                   size_t minDist) {
  size_t prev = positions[0];
  size_t count = 1;
  for (size_t i = 1; i < nStalls; i++) {
    if (positions[i] - prev >= minDist) {
      prev = positions[i];
      count++;
    }
    if (count == nCows) 
      return true;
  }
  return false;
}

//===================================================================
// Returns the maximum minimum distance between any two cows
size_t maxMinDist (size_t *positions, size_t nStalls, size_t nCows) {
  size_t left = 0, right = positions[nStalls - 1] - positions[0];
  while (left <= right) {
    size_t mid = left + (right - left)/2;
    if (arePlaceable(positions, nStalls, nCows, mid)) 
      left = mid + 1;
    else right = mid - 1;
  }
  return left - 1;
}

//===================================================================
// Prints the stall positions where the cows should be placed such 
// that the cows are at least minDist apart
void printSolution (size_t *positions, size_t nStalls, size_t nCows,
                    size_t minDist) {
  size_t prev = positions[0];
  size_t count = 1;
  printf("Minimum distance: %zu\n", minDist);
  printf("Stall positions: %zu ", positions[0]);
  for (size_t i = 0; i < nStalls; i++) {
    if (positions[i] - prev >= minDist) {
      printf("%zu ", positions[i]);
      prev = positions[i];
      if (++count == nCows) 
        break;
    }
  }
  printf("\n");
}

//===================================================================

int main () {

  size_t nCows;
  assert(scanf("%zu", &nCows) == 1);
  
  READ(size_t, positions, "%zu", nStalls);

  qsort(positions, nStalls, sizeof(size_t), cmpPositions);

  printSolution(positions, nStalls, nCows, 
                maxMinDist(positions, nStalls, nCows));

  free(positions);
  return 0;
}