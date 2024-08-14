/* 
  file: asp-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: activity selection problem (ASP)
               using bottom-up dynamic programming
  time complexity: O(n³)
  note: the total number of selected activities is unique
        but the selection itself is not, so the output
        may differ from the other ASP implementations
*/ 

#include "../../../lib/clib/clib.h"
#include <stdint.h>

//===================================================================
// Definition of an activity, having a start and finish time and an
// id number based on the order of input
typedef struct {
  size_t start, finish, id;
} Act;

//===================================================================
// Reads activities from stdin and adds two sentinel activities
Act *readActs(size_t *len) {
  size_t cap = 100;
  Act *acts = safeCalloc(cap, sizeof(Act));

    // start with a sentinel activity
  acts[(*len)++] = (Act){0, 0, 0};

  while (scanf(" [ %zu , %zu ) , ", &acts[*len].start, 
                                    &acts[*len].finish) == 2) {
    acts[*len].id = *len;

    if (++*len == cap) {
      cap *= 2;
      acts = safeRealloc(acts, cap * sizeof(Act));
    }
  }
    // end with a sentinel activity
  acts[(*len)++] = (Act){SIZE_MAX, SIZE_MAX, *len};

  return acts;
}

//===================================================================
// Comparison function for qsort
int cmpAct(void const *a, void const *b) {
  return ((Act *)a)->finish < ((Act *)b)->finish ? -1 : 1;
}

//===================================================================
// Prints the selected activities in sequence of execution
void printActs(Act *acts, size_t **maxSet, size_t i, size_t j) {

  size_t k = maxSet[i][j];

  if (k) {
    printActs(acts, maxSet, i, k);
    printf("Activity %zu: [%zu, %zu)\n", 
            acts[k].id, acts[k].start, acts[k].finish);
    printActs(acts, maxSet, k, j);
  }
}

//===================================================================
// Selects the maximum number of activities such that no two overlap
// and stores the selected activities in maxSet 
void selectActs(Act *acts, size_t n, size_t **maxSet, size_t **dp) {
  
    // work by increasing subproblem size l
  for (size_t l = 2; l <= n; ++l) {
    for (size_t i = 0; i <= n - l; ++i) {
      size_t j = i + l - 1;
      size_t k = j - 1;
        // select the optimal activity aₖ that maximizes the
        // number of activities in the set [i, j] of size l
      while (acts[i].finish < acts[k].finish) {
        if (acts[i].finish <= acts[k].start && 
            acts[k].finish <= acts[j].start &&
            dp[i][k] + dp[k][j] + 1 > dp[i][j]) {
          dp[i][j] = dp[i][k] + dp[k][j] + 1;
          maxSet[i][j] = k;
        }
        --k;
      }
    }
  }
}

//===================================================================

int main() {
  
  size_t n = 0;
  Act *acts = readActs(&n);

    // sort the activities by finish time
  qsort(acts, n, sizeof(Act), cmpAct);

    // matrix to store the maximum number of activities
  CREATE_MATRIX(size_t, dp, n + 1, n + 1, 0);

    // matrix to store the selected activities
  CREATE_MATRIX(size_t, maxSet, n + 1, n + 1, 0);

  selectActs (acts, n, maxSet, dp);

  printf("Max number of activities: %zu\n\n"
         "Activity sequence:\n\n", dp[0][n - 1]);
  
  printActs(acts, maxSet, 0, n - 1);
  
  FREE_MATRIX(dp, n + 1);
  FREE_MATRIX(maxSet, n + 1);
  free(acts);

  return 0;
}