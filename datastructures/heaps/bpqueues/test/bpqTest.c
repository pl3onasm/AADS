/* 
  Generic priority queue, using a binary heap
  Some tests with a priority queue of tasks having 
    a name, and a duration
  Author: David De Potter
*/

#include "../bpqueue.h"  
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
  bpqType type = rand() % 2;

  size_t size = rand() % 30 + 20;

  int bound = type == MIN ? INT_MIN : INT_MAX;

  bpqueue *pq = bpqNew(size, type, compKeys, copyKey, 
                       free, taskToString, &bound);
  bpqSetShow(pq, showKey, showTask);
  bpqSetLabel(pq, "Task queue");
  bpqSetDelim(pq, " | ");

  printf("TESTING %s BINARY PRIORITY QUEUE\n\n", 
         pq->type == MIN ? "MIN" : "MAX");
  printf("Creating %zu tasks for the %s binary priority queue...\n\n",
          size, pq->type == MIN ? "MIN" : "MAX");

  task **tasks = safeCalloc(size, sizeof(task *));
  for (size_t i = 0; i < size; i++) {
    tasks[i] = newTask(i);
    printf("  Created task %s with duration %.2f min\n", 
           tasks[i]->name, tasks[i]->duration);
  }

  printf("\nPushing the tasks with random priorities...\n\n");

  for (size_t i = 0; i < size; i++) {
    int key = rand() % size;
    bpqPush(pq, tasks[i], &key);
    printf("  Pushed %s with priority %d\n", tasks[i]->name, key);
  }

  printf("\nAfter pushing all tasks, this is what the queue looks like\n\n");

  bpqShow(pq);

  printf("\nAll data is mapped to indices in the queue as shown below\n");

  sstMapSetLabel(pq->datamap, "Data map");
  sstMapShow(pq->datamap);

  printf("Deleting 7 tasks ...\n\n");
  size_t deletions = 0;
  while (deletions < 7) {
    size_t idx = rand() % size;
    if (bpqDelete(pq, tasks[idx])) {
      printf("  Deleted task %zu\n", idx);
      freeTask(tasks[idx]);
      tasks[idx] = NULL;
      deletions++;
    }
  }

  printf("\nAfter deleting some tasks, we have the below queue\n\n");

  bpqShow(pq);
  
  printf("Randomly updating 10 priorities..\n\n");
  size_t updates = 0;
  while (updates < 10) {
    size_t idx = rand() % size;
    int val = pq->type == MIN ? rand() % 10 + 1 : rand() % 100 + 150;
    if (bpqUpdateKey(pq, tasks[idx], &val)) {
      printf ("Updated priority of task %zu to %d\n", idx, val);
      updates++;
    }
  }
  printf("\nAfter updating priorities, we have the following queue\n\n");

    // show the updated queue
  bpqShow(pq);

    // pop all tasks
  printf("Popping all tasks..\n\n");
  while (! bpqIsEmpty(pq)) {
    task *top = bpqPeek(pq);
    int *prio = bpqGetKey(pq, top);
    printf("Popped %s with priority %d\n", top->name, *prio);
    task *t = bpqPop(pq);
    freeTask(t);
  }
  
  free(tasks);
  bpqFree(pq);

  return 0;
}
