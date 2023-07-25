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

#include <stdio.h>
#include <stdlib.h>

typedef enum {
  false = 0,
  true = 1
} bool;

void *safeCalloc (int n, int size) {
  /* allocates n elements of size size, initializing them to 0, and
     checks whether the allocation was successful */
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

int compare (const void * a, const void * b) {
  return ( *(int*)a - *(int*)b );
}

bool isSolvable (int * stalls, int n, int c, int minDist) {
  /* checks whether it is possible to place c cows in the stalls
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

int maxMinDist (int * stalls, int n, int c) {
  /* returns the maximum minimum distance between any 
     two cows, given c cows and n stalls */
  int l = 0, r = stalls[n-1] - stalls[0], m;
  while (l <= r) {
    m = l + (r - l)/2;
    if (isSolvable(stalls, n, c, m)) l = m + 1;
    else r = m - 1;
  }
  return l - 1;
}

void printSolution (int * stalls, int n, int c, int minDist) {
  /* prints the stalls where the cows should be placed */
  int prev = stalls[0], i, count = 1;
  printf("Minimum distance: %d\n", minDist);
  printf("Stalls: %d ", stalls[0]);
  for (i = 0; i < n; i++) {
    if (stalls[i] - prev >= minDist) {
      printf("%d ", stalls[i]);
      prev = stalls[i];
      count++;
    }
  }
  printf("\n");
}

int main () {
  int n, c;
  scanf("%d %d", &n, &c);
  int *stalls = safeCalloc(n, sizeof(int));
  for (int i = 0; i < n; i++) scanf("%d", &stalls[i]);
  qsort(stalls, n, sizeof(int), compare);
  printSolution(stalls, n, c, maxMinDist(stalls, n, c));
  free(stalls);
  return 0;
}