/* 
  file: lhs.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: lecture hall scheduling (LHS) 
               using greedy algorithm and a binary heap
  time complexity: O(n log n) where n is the number of activities
  note: this version uses a binary heap to keep track of occupied
        halls, which will schedule activities differently than the
        lhs-2.c version, which uses a stack, but the schedules are,
        of course, still optimal
*/ 

#include "../../../lib/clib/clib.h"
#include "../../../datastructures/heaps/binheaps/binheap.h"

//===================================================================
// Defines an activity
typedef struct {
  size_t id;               // activity id based on input order
  size_t start;            // start time
  size_t end;              // end time
} activity;

//===================================================================
// Defines a lecture hall 
typedef struct {
  size_t id;               // hall id
  activity *sch;           // schedule of activities
  size_t schSize;          // number of activities in the schedule
} hall;

//===================================================================
// Compares two activities by their start time
int cmpActivities(void const *a, void const *b) {
  return ((activity *)a)->start - ((activity *)b)->start;
}

//===================================================================
// Compares two halls by the end time of the last activity in their
// schedules
int cmpHalls(void const *a, void const *b) {
  hall *h1 = (hall *)a;
  hall *h2 = (hall *)b;
  return h1->sch[h1->schSize-1].end - h2->sch[h2->schSize-1].end;
}

//===================================================================
// Shows an activity
void showActivity(activity act) {
  printf("Act %zu: [ %zu, %zu )\n", act.id, act.start, act.end);
}

//===================================================================
// Reads activities from stdin
activity *readActivities(size_t *nActs) {

  size_t cap = 50;
  activity *acts = safeCalloc(cap, sizeof(activity));
  
  while (scanf(" [ %zu , %zu ) , ", 
         &acts[*nActs].start, &acts[*nActs].end) == 2) {
    
    acts[*nActs].id = *nActs + 1;
    if (++*nActs == cap) {
      cap *= 2;
      acts = safeRealloc(acts, cap * sizeof(activity));
    }
  }
  return acts;
}

//===================================================================
// Creates a new lecture hall
hall *newHall(size_t id, activity act, size_t schSize) {
  hall *h = safeCalloc(1, sizeof(hall));
  h->sch = safeCalloc(schSize, sizeof(activity));
  h->id = id;
  h->sch[h->schSize++] = act;
  return h;
}

//===================================================================
// Schedules activities in lecture halls by scanning the activities
// in order of increasing start time and assigning each activity to
// the earliest available lecture hall at the time of its start
// Returns the number of lecture halls used
size_t scheduleActivities(activity *acts, size_t nActs, 
                          hall **halls) {

    // sort activities by start time
  qsort(acts, nActs, sizeof(activity), cmpActivities);

    // create a binary heap for keeping track of occupied halls;
    // the min heap is ordered by the end time of the last 
    // activity in each hall's schedule
  binheap *occupied = bhpNew(nActs, MIN, cmpHalls);
  hall *h; size_t nHalls = 0;

    // assign each activity to the earliest available hall
  for (size_t i = 0; i < nActs; i++) {
    if ((h = bhpPeek(occupied))
        && acts[i].start >= h->sch[h->schSize-1].end) {
        // assign activity to hall at the top of the heap
      h->sch[h->schSize++] = acts[i];
      bhpHeapify(occupied, 0);
    } else {
        // assign activity to a new hall
      halls[nHalls] = newHall(nHalls + 1, acts[i], nActs);
      bhpPush(occupied, halls[nHalls++]);
    }
  }
  bhpFree(occupied);
  return nHalls;
}

//===================================================================
// Shows the number of halls and their schedules
void showSchedules(hall **halls, size_t nHalls) {
  printf("Number of halls: %zu\n\n"
         "SCHEDULES\n", nHalls);
  for (size_t i = 0; i < nHalls; i++) {
    printf("\n---------\n"
           " Hall %zu"
           "\n---------\n", halls[i]->id);
    for (size_t j = 0; j < halls[i]->schSize; j++) 
      showActivity(halls[i]->sch[j]);
    free(halls[i]->sch);
    free(halls[i]);
  }
  printf("\n");
}

//===================================================================

int main() {

  size_t nActs = 0;
  activity *acts = readActivities(&nActs);

  hall **halls = safeCalloc(nActs, sizeof(hall*));

  size_t nHalls = scheduleActivities(acts, nActs, halls);
  showSchedules(halls, nHalls);

  free(acts);
  free(halls);

  return 0;
}