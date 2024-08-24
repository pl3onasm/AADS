/* 
  file: lhs-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: lecture hall scheduling (LHS) 
    using brute force by counting conflicts at each point in time
    and taking the maximum number of conflicts as the number of halls
  time complexity: O((max - min) * n) 
    where n is the number of activities
*/ 

#include "../../../lib/clib/clib.h"
#include <stdint.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

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
activity *readActivities(size_t *nActs, size_t *min, size_t *max) {

  size_t cap = 50, n = 0;
  activity *acts = safeCalloc(cap, sizeof(activity));
  
  while (scanf(" [ %zu , %zu ) , ", 
         &acts[n].start, &acts[n].end) == 2) {
    
    acts[n].id = n + 1;
    *min = MIN(*min, acts[n].start);
    *max = MAX(*max, acts[n].end);

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
// activities without any overlap by counting the maximum number of
// conflicting (overlapping) activities at any given time between
// the minimum start time and maximum end time
size_t minHalls(activity *acts, size_t nActs, size_t min, 
                size_t max) {

  size_t nHalls = 0;

  for (size_t t = min; t < max; t++) {
    size_t nConflicts = 0;
    for (size_t i = 0; i < nActs; i++) 
      if (acts[i].start <= t && acts[i].end > t)
        nConflicts++;
    nHalls = MAX(nHalls, nConflicts);
  }

  return nHalls;
}

//===================================================================

int main() {

  size_t nActs = 0, min = SIZE_MAX, max = 0;
  activity *acts = readActivities(&nActs, &min, &max);

  size_t nHalls = minHalls(acts, nActs, min, max);

  printf("Number of halls: %zu\n", nHalls);
  
  free(acts);

  return 0;
}