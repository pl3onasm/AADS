/* 
  file: lhs-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: lecture hall scheduling (LHS) 
    using brute force by counting the maximum number of overlapping
    activities at any given time 
  time complexity: O(n^2)
    where n is the number of activities
*/ 

#include "../../../lib/clib.h"

//===================================================================
// Defines an activity
typedef struct {
  size_t id;               // activity id based on input order
  size_t start;            // start time
  size_t end;              // end time
} activity;

//===================================================================
// Reads activities from stdin and determines the minimum start time
// and maximum end time
activity *readActivities(size_t *nActs) {

  size_t cap = 50, n = 0;
  activity *acts = safeCalloc(cap, sizeof(activity));
  
  while (scanf(" [ %zu , %zu ) , ", 
         &acts[n].start, &acts[n].end) == 2) {
    
    acts[n].id = n + 1;

    if (++n == cap) {
      cap *= 2;
      acts = safeRealloc(acts, cap * sizeof(activity));
    }
  }

  *nActs = n;
  return acts;
}

//===================================================================
// Finds the minimum number of halls needed to schedule all 
// activities without any overlap by counting for each activity 
// the number of overlapping activities, and taking the maximum as
// the number of halls needed
size_t minHalls(activity *acts, size_t nActs) {

  size_t nHalls = 0;

  for (size_t i = 0; i < nActs; i++) {
    size_t nConflicts = 0;
    for (size_t j = 0; j < nActs; j++) 
      if (acts[j].start <= acts[i].start 
          && acts[j].end > acts[i].start)
        nConflicts++;
    nHalls = MAX(nHalls, nConflicts);
  }

  return nHalls;
}

//===================================================================

int main() {

  size_t nActs = 0;
  activity *acts = readActivities(&nActs);

  size_t nHalls = minHalls(acts, nActs);

  printf("Number of halls: %zu\n", nHalls);
  
  free(acts);

  return 0;
}





