/* file: cows.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: 
     this an example of binary search, where we are looking  
     for the upper bound on the distance between two cows 
     such that they are at least that far apart.
   time complexity: 
      O(n log(n))
*/

#include "../../../../lib/clib/clib.h"

int compare (const void * a, const void * b) {
  return ( *(int*)a - *(int*)b );
}

bool isSolvable (int *stalls, int n, int c, int minDist) {
  /* checks whether it is possible to place c cows in n stalls
     such that the minimum distance between any two of them is
     at least minDist */
  int prev = stalls[0], i, count = 1;
  for (i = 1; i < n; i++) {
    if (stalls[i] - prev >= minDist) {
      prev = stalls[i];
      count++;
    }
    if (count == c) return true;
  }
  return false;
}

int maxMinDist (int *stalls, int n, int c) {
  /* returns the maximum minimum distance between any 
     two cows, given c cows and n stalls */
  int left = 0, right = stalls[n-1] - stalls[0], mid;
  while (left <= right) {
    mid = left + (right - left)/2;
    if (isSolvable(stalls, n, c, mid)) left = mid + 1;
    else right = mid - 1;
  }
  return left - 1;
}

void printSolution (int *stalls, int n, int c, int minDist) {
  /* prints the stalls where the cows should be placed */
  int prev = stalls[0], i, count = 1;
  printf("Minimum distance: %d\n", minDist);
  printf("Stalls: %d ", stalls[0]);
  for (i = 0; i < n; i++) {
    if (stalls[i] - prev >= minDist) {
      printf("%d ", stalls[i]);
      prev = stalls[i];
      count++;
      if (count == c) break;
    }
  }
  printf("\n");
}

int main () {
  int n, c;
  (void)! scanf("%d %d", &n, &c);
  
  CREATE_ARRAY(int, stalls, n);
  READ_ARRAY(stalls, "%d", n);

  qsort(stalls, n, sizeof(int), compare);

  printSolution(stalls, n, c, maxMinDist(stalls, n, c));

  free(stalls);
  return 0;
}