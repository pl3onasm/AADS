/* 
  file: asp-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: activity selection problem (ASP)
    using an iterative greedy algorithm
  time complexity: Θ(n) for selecting activities,
                   O(n log n) for sorting
*/ 

#include "../../../lib/clib/clib.h"

//===================================================================
// Definition of an activity, having a start and finish time, an id
// number based on order of input and a boolean flag for selection
typedef struct {
  size_t start, finish, id;
  bool selected;
} Act;

//===================================================================
// Reads activities from stdin, no sentinel activities needed
Act *readActs(size_t *len) {
  size_t cap = 100;
  Act *acts = safeCalloc(cap, sizeof(Act));

  while (scanf(" [ %zu , %zu ) , ", &acts[*len].start, 
                                    &acts[*len].finish) == 2) {
    acts[*len].id = *len + 1;
    acts[*len].selected = false;

    if (++*len == cap) {
      cap *= 2;
      acts = safeRealloc(acts, cap * sizeof(Act));
    }
  }

  acts = safeRealloc(acts, *len * sizeof(Act));
  return acts;
}

//===================================================================
// Comparison function for qsort
int cmpAct(const void *a, const void *b) {
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
// can be performed
size_t selectActs(Act *acts, size_t nActs) {
  size_t k = 0, max = 1;
    // select a₁
  acts[0].selected = true;

  for (size_t i = 1; i < nActs; i++) 
    if (acts[i].start >= acts[k].finish) {  
        // if aᵢ starts after aₖ finishes, select aᵢ
      acts[i].selected = true;
        // update k to i to continue search for next 
        // activity, compatible with aᵢ
      k = i;                      
      ++max;
    }
  
  return max;
}

//===================================================================

int main () {
  
  size_t nActs = 0;
  Act *acts = readActs(&nActs);

    // sort the activities by finish time
  qsort(acts, nActs, sizeof(Act), cmpAct);

  printf("Maximum number of activities: %zu\n\n"
         "Activity sequence:\n\n", 
         selectActs(acts, nActs));

  printActs(acts, nActs);

  free(acts);

  return 0; 
}