/* 
  Generic priority queue, using a binary heap
  Some tests with a priority queue of tasks having 
    a name, and a duration
  Author: David De Potter
*/

#include "bpqueue.h"  
#include <string.h>
#include <time.h>
#include "../../lib/clib/clib.h"

typedef struct {
  char *name;
  double duration;
} task;

  // make a copy of a string
char *copyStr(char *s) {
  char *copy = safeCalloc(strlen(s) + 1, sizeof(char));
  strcpy(copy, s);
  return copy;
}

  // comparison function for size_t keys
int compKey(void const *a, void const *b) {
  size_t x = *(size_t *)a;
  size_t y = *(size_t *)b;
  return x - y;
}

  // make a copy of a size_t key
void *copyKey(void *key) {
  size_t *copy = safeCalloc(1, sizeof(size_t));
  *copy = *(size_t *)key;
  return copy;
}

  // show a size_t key
void showKey(void const *key) {
  printf("%zu", *(size_t *)key);
}

  // create a new task
task *newTask(char *name, double duration) {
  task *t = safeCalloc(1, sizeof(task));
  t->name = name;
  t->duration = duration;
  return t;
}

  // deallocate a task
void freeTask(void *data) {
  task *t = (task *)data;
  free(t->name);
  free(t);
}
  
    // show a task
void showTask(void const *data) {
  task *t = (task *)data;
  printf("%s - %.2fmin", t->name, t->duration);
}

  // convert a task to a string
char *taskToString(void const *data) {
  task *t = (task *)data;
  return t->name;
}


int main () {
  srand(time(NULL));
  bpqueue *pq = bpqNew(10, MIN, compKey, copyKey, 
                       free, taskToString);
  bpqSetShow(pq, showKey, showTask);
  bpqSetLabel(pq, "Task queue");
  bpqSetDelim(pq, " | ");
  bpqOwnData(pq, freeTask);

    // create some random tasks
  task *tasks[20];
  for (size_t i = 0; i < 20; i++) {
    char *name = safeCalloc(20, sizeof(char));
    sprintf(name, "Task %zu", i);
    char duration[5];
    sprintf(duration, "%d", rand() % 60);
    sprintf(duration + strlen(duration), ".%02d", rand() % 60);
    
    tasks[i] = newTask(name, atof(duration));
  }

    // push the tasks with their priorities
  for (size_t i = 0; i < 20; i++) {
    size_t key = rand() % 20 + 10;
    bpqPush(pq, tasks[i], &key);
  }

  bpqShow(pq);

    // show the idx map
  sstMapSetLabel(pq->map, "Index Map");
  sstMapShow(pq->map);
  
    // update the priority of some tasks
  printf("Updating 10 priorities..\n\n");
  size_t val = 10;
  for (size_t i = 0; i < 20; i += 2) {
    size_t idx = rand() % 20;
    bpqUpdateKey(pq, tasks[idx], &val);
    printf ("Updated priority of task %zu to %zu\n", idx, val);
    val--;
  }
  printf("\n");

    // show the updated queue
  bpqShow(pq);

    // pop all tasks
  printf("Popping all tasks..\n\n");
  while (! bpqIsEmpty(pq)) {
    task *top = bpqPeek(pq);
    size_t *prio = bpqGetKey(pq, top);
    printf("Popped %s with priority %zu\n", top->name, *prio);
    task *t = bpqPop(pq);
    freeTask(t);
  }
  
  bpqFree(pq);

  return 0;
}
