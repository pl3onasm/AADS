/* 
  file: lhs-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: lecture hall scheduling (LHS) 
               using greedy algorithm and a stack
  time complexity: O(n log n) where n is the number of activities
  note: this version uses a stack to keep track of available halls,
        which will schedule activities differently than the lhs-4.c
        version, which uses a binary heap, but the schedules are, 
        of course, still optimal, using the minimum number of halls
*/ 

#include "../../../lib/clib.h"
#include "../../../datastructures/stacks/stack.h"

//===================================================================
// Defines an activity
typedef struct activity {
  size_t id;                // activity id based on input order
  size_t start;             // start time
  size_t end;               // end time
  size_t hallId;            // hall where the activity is scheduled
} activity;

//===================================================================
// Defines an event: the start or end of an activity
typedef struct event {
  activity *act;            // activity associated with the event
  size_t time;              // time of the event
  enum { START, END } type; // start or end event
} event;

//===================================================================
// Defines a lecture hall 
typedef struct hall {
  size_t id;                // hall id
  activity *sch;            // schedule of activities
  size_t schSize;           // number of activities in the schedules
} hall;

//===================================================================
// Compares two events by their time in ascending order: if a start
// event and an end event have the same time, the end event comes
// first in the sorted order so that the hall is freed up first
int cmpEvents(void const *a, void const *b) {
  event *e1 = (event *)a;
  event *e2 = (event *)b;
  if (e1->type == START && e2->type == END && e1->time == e2->time)
    return 1;
  return e1->time - e2->time;
}

//===================================================================
// Shows an activity 
void showActivity(activity act) {
  printf("Act %zu: [ %zu, %zu )\n", act.id, act.start, act.end);
}

//===================================================================
// Creates events from activities
event *createEvents(activity *acts, size_t nActs) {
  event *events = safeCalloc(nActs * 2, sizeof(event));
  for (size_t i = 0; i < nActs; i++) {
    events[i * 2].act = &acts[i];
    events[i * 2].time = acts[i].start;
    events[i * 2].type = START;
    events[i * 2 + 1].act = &acts[i];
    events[i * 2 + 1].time = acts[i].end;
    events[i * 2 + 1].type = END;
  }
  return events;
}

//===================================================================
// Reads activities from stdin
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
// Creates a new lecture hall
hall *newHall(size_t id, size_t schSize) {
  hall *h = safeCalloc(1, sizeof(hall));
  h->sch = safeCalloc(schSize, sizeof(event));
  h->id = id;
  return h;
}

//===================================================================
// Schedules activities in lecture halls by scanning the events as
// they appear in the time line 
size_t scheduleActivities(activity *acts, size_t nActs, hall **halls,
                          event *events) {
  
  size_t nHalls = 0;
  qsort(events, nActs * 2, sizeof(event), cmpEvents);

    // create a stack of available halls
  stack *availHalls = newStack(nActs);
  
    // scan the events
  for (size_t i = 0; i < nActs * 2; i++) {
    event e = events[i];
    if (e.type == START) {
      if (isEmptyStack(availHalls)) {
          // create a new hall if no halls are available
        halls[nHalls] = newHall(nHalls + 1, nActs);
        stackPush(availHalls, halls[nHalls++]);
      } 
        // assign activity to the last hall that became available
      hall *h = stackPop(availHalls);
      h->sch[h->schSize++] = *e.act;
      e.act->hallId = h->id;
    } else 
        // if an activity ends, its hall becomes available again
      stackPush(availHalls, halls[e.act->hallId - 1]);
  }

  freeStack(availHalls);
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

  event *events = createEvents(acts, nActs);

  hall **halls = safeCalloc(nActs, sizeof(hall*));

  size_t nHalls = scheduleActivities(acts, nActs, halls, events);
  showSchedules(halls, nHalls);

  free(events);
  free(acts);
  free(halls);

  return 0;
}