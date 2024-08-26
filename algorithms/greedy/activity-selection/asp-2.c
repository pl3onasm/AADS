/* 
  file: asp-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: activity selection problem (ASP)
    using a recursive greedy algorithm
  time complexity: Θ(n) for selecting activities,
                   O(n log n) for sorting
*/ 

#include "../../../lib/clib.h"

//===================================================================
// Definition of an activity, having a start and finish time, an id
// number based on order of input and a boolean flag for selection
typedef struct {
  size_t start, finish, id;
  bool selected;
} Act;

//===================================================================
// Reads the activities and adds a sentinel activity at the start
Act *readActs(size_t *len) {
  size_t cap = 100;
  Act *acts = safeCalloc(cap, sizeof(Act));

    // start with a sentinel activity
  acts[(*len)++] = (Act){0, 0, 0, false};   

  while (scanf(" [ %zu , %zu ) , ", &acts[*len].start, 
                                    &acts[*len].finish) == 2) {
    acts[*len].id = *len;
    acts[*len].selected = false;

    if (++*len == cap) {
      cap *= 2;
      acts = safeRealloc(acts, cap * sizeof(Act));
    }
  }

  return acts;
}

//===================================================================
// Comparison function for qsort
int cmpAct(void const *a, void const *b) {
  return ((Act *)a)->finish < ((Act *)b)->finish ? -1 : 1;
}

//===================================================================
// Prints the selected activities in sequence of execution
void printActs(Act *acts, size_t nActs) {
  for (size_t i = 0; i < nActs; i++) 
    if (acts[i].selected)
      printf("Activity %zu: [%zu, %zu)\n", 
              acts[i].id, acts[i].start, acts[i].finish);
}

//===================================================================
// Selects the maximum number of mutually compatible activities that
// can be performed; returns the number of activities selected
size_t selectActs(Act *acts, size_t i, size_t n) {

    // find index k of the first activity aₖ
    // that starts after aᵢ finishes
  size_t k = i + 1; 
  while (k < n && acts[k].start < acts[i].finish) ++k;
      
    // if such an activity exists, select it and continue
  if (k < n) {
    acts[k].selected = true;
    return 1 + selectActs(acts, k, n);
  }

  return 0;
}

//===================================================================

int main () {
  
  size_t nActs = 0;
  Act *acts = readActs(&nActs);

    // sort activities by finish time
  qsort(acts, nActs, sizeof(Act), cmpAct);

  printf("Maximum number of activities: %zu\n\n"
         "Activity sequence:\n\n", 
         selectActs(acts, 0, nActs));

  printActs(acts, nActs);
  
  free(acts);

  return 0; 
}