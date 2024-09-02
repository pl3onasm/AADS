/* 
  Generic priority queue, using a binary heap
  Some tests with a priority queue of tasks having 
    a name, and a duration
  Author: David De Potter
*/

#include "../fibheap.h"  
#include <string.h>
#include <time.h>
#include "../../../../lib/clib.h"
#include <limits.h>

//===================================================================
// task structure
typedef struct {
  char *name;
  double duration;
} task;

//===================================================================
// comparison function for int keys
int compKeys(void const *a, void const *b) {
  int x = *(int *)a;
  int y = *(int *)b;
  return x < y ? -1 : x > y;
}

//===================================================================
// make a copy of an int key
void *copyKey(void const *key) {
  int *copy = safeCalloc(1, sizeof(int));
  *copy = *(int *)key;
  return copy;
}

//===================================================================
// show function for a task
void showTask(void const *data) {
  task *t = (task *)data;
  printf("%s - %.2f min", t->name, t->duration);
}

//===================================================================
// show function for an int key
void showKey(void const *key) {
  printf("%d", *(int *)key);
}

//===================================================================
// creates a new random task
task *newTask(size_t n) {
  char *name = safeCalloc(50, sizeof(char));
  sprintf(name, "Task %zu", n);
  char duration[5];
  sprintf(duration, "%d", rand() % 60);
  sprintf(duration + strlen(duration), ".%02d", rand() % 60);
  task *t = safeCalloc(1, sizeof(task));
  t->name = name;
  t->duration = atof(duration);
  return t;
}

//===================================================================
// deallocates a task
void freeTask(void *data) {
  task *t = (task *)data;
  free(t->name);
  free(t);
}

//===================================================================
// string representation of a task 
char *taskToString(void const *data) {
  task *t = (task *)data;
  return t->name;
}

//===================================================================

int main () {
  srand(time(NULL));

    // randomize the type of the heap
  fibType type = rand() % 2;

    // bound sets the non-inclusive upper limit for a max heap
    // or the non-inclusive lower limit for a min heap and
    // is used to initialize the sentinel key for deletion
  int bound = type == MIN ? INT_MIN : INT_MAX;

  fibheap *F = fibNew(type, compKeys, copyKey,  
                      free, taskToString, &bound);
  fibSetShow(F, showKey, showTask);
  fibSetLabel(F, "Task queue");
  
  size_t size = rand() % 30 + 20;

  printf("TESTING %s FIBONACCI HEAP\n\n", F->type == MIN ? "MIN" : "MAX");
  printf("Creating %zu tasks for the %s Fibonacci heap...\n\n", 
         size, F->type == MIN ? "MIN" : "MAX");

  task **tasks = safeCalloc(size, sizeof(task *));

  for (size_t i = 0; i < size; i++) {
    tasks[i] = newTask(i);
    printf("  Created task %s with duration %.2f min\n", 
           tasks[i]->name, tasks[i]->duration);
  }

  printf("\nPushing the tasks with random priorities...\n\n");

  for (size_t i = 0; i < size; i++) {
    int key = rand() % size;

    fibPush(F, tasks[i], &key);
    printf("  Pushed %s with priority %d\n", tasks[i]->name, key);
  }

  printf("\nAfter pushing all tasks, we have a single root list\n"
         "in which all %zu nodes have the same degree 0\n", size);
  
  fibShow(F);
  
  printf("Deleting 7 tasks ...\n\n");
  size_t deletions = 0;
  while (deletions < 7) {
    size_t idx = rand() % size;
    if (fibDelete(F, tasks[idx])) {
      printf("  Deleted task %zu\n", idx);
      freeTask(tasks[idx]);
      tasks[idx] = NULL;
      deletions++;
    }
  }

  printf("\nAfter deleting some tasks, the necessary housekeeping has been done:\n"
         "the root list has been rebuilt and nodes have been linked\n");

  fibShow(F);
  
  printf("\nRandomly updating 10 priorities ...\n\n");
  size_t updates = 0;
  while (updates < 10) {
    size_t idx = rand() % size;
    int val = F->type == MIN ? rand() % 10 + 1 : rand() % 100 + 150;
    if (!tasks[idx]) 
      continue;
    int oldKey = *(int *)fibGetKey(F, tasks[idx]);
    if (fibUpdateKey(F, tasks[idx], &val)) {
      printf ("  Updated priority of task %zu from %d to %d\n", 
              idx, oldKey, val);
      updates++;
    }
  }
  printf("\nThe restructured %s heap after updating 10 priorities\n"
         "shows the new priorities and the new root list\n", 
         F->type == MIN ? "MIN" : "MAX");

  fibShow(F);

  printf("\nPopping all tasks..\n");
  while (! fibIsEmpty(F)) {
    task *top = fibPeek(F);
    printf("\n* Popping %s with priority %d *\n ", 
           top->name, *(int *)fibGetKey(F, top));
    task *t = fibPop(F);
    freeTask(t);
    fibShow(F);
  }
  
  free(tasks);
  fibFree(F);
  
  return 0;
}
