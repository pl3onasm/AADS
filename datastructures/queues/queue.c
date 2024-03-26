/* 
  Generic queue, implemented as a circular buffer.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include <stdio.h>
#include "queue.h"
#include "../../lib/clib/clib.h"

//=================================================================
// Creates a new queue with given capacity
queue *newQueue(size_t cap) {
  
  queue *Q = safeCalloc(1, sizeof(queue));
  Q->buffer = safeCalloc(cap, sizeof(void *));
  Q->capacity = cap;
  Q->delim = ", ";
  return Q;
}

//=================================================================
// Deallocates the queue
void freeQueue(queue *Q) {
  free(Q->buffer);
  free(Q);
}

//=================================================================
// Sets the show function for the queue
void setQueueShow(queue* Q, showItem show) {
  Q->show = show;
}

//=================================================================
// Sets the string delimiter for showing items
void setQueueDelim(queue* Q, char* delim) {
  Q->delim = delim;
}

//=================================================================
// Doubles the capacity of the queue
static void doubleQueueSize(queue *Q) {

  Q->buffer = safeRealloc(Q->buffer, 2 * Q->capacity * sizeof(void *));
  
  for (size_t i = 0; i < Q->tail; ++i)
    Q->buffer[i + Q->capacity] = Q->buffer[i];
  
  Q->tail += Q->capacity;
  Q->capacity *= 2;
}

//=================================================================
// Enqueues an item in the queue
void enqueue (queue *Q, void *item) {
  Q->buffer[Q->tail] = item;
  Q->tail = (Q->tail + 1) % Q->capacity;
  if (Q->tail == Q->head) 
    doubleQueueSize(Q);
  Q->size++;
}

//=================================================================
// Dequeues an item from the queue and returns it
void *dequeue (queue *Q) {
  
  if (qIsEmpty(Q)) {
    fprintf(stderr, "Dequeue error: Queue is empty\n");
    exit(EXIT_FAILURE);
  }

  void *item = Q->buffer[Q->head];
  Q->head = (Q->head + 1) % Q->capacity;
  Q->size--;
  return item;
}

//=================================================================
// Peeks at the front of the queue
void *peekHead (queue *Q) {
  if (qIsEmpty(Q)) {
    fprintf(stderr, "PeekHead error: Queue is empty\n");
    exit(EXIT_FAILURE);
  }
  return Q->buffer[Q->head];
}

//=================================================================
// Peeks at the back of the queue
void *peekTail (queue *Q) {
  if (qIsEmpty(Q)) {
    fprintf(stderr, "PeekTail error: Queue is empty\n");
    exit(EXIT_FAILURE);
  }
  return Q->buffer[(Q->tail - 1) % Q->capacity];
}

//=================================================================
// Shows the entire queue
void showQueue(queue *Q) {
  if (! Q->show) {
    fprintf(stderr, "ShowQueue error: show function not set\n");
    return;
  }
  for (size_t i = Q->head; i != Q->tail; i = (i + 1) % Q->capacity) {
    Q->show(Q->buffer[i]);
    printf("%s", (i + 1) % Q->capacity == Q->tail ? "\n" : Q->delim);
  }
}