/* 
  Generic stack implementation.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "stack.h"
#include "../../lib/clib.h"

//===================================================================
// Creates a new stack with given capacity
stack *newStack(size_t cap) {
  
  stack *S = safeCalloc(1, sizeof(stack));
  S->buffer = safeCalloc(cap, sizeof(void *));
  S->capacity = cap;
  S->delim = ", ";
  return S;
}

//===================================================================
// Sets the stack to own the input data, deallocating
// what is still in the stack when it is destroyed
void setStackOwner(stack *S, freeStackItem free) {
  S->free = free;
}

//===================================================================
// Makes the stack operate on copies of the input data
void setStackCopy(stack *S, copyStackItem copy,
                  freeStackItem free) {
  S->copy = copy;
  S->free = free;
}

//===================================================================
// Deallocates the stack
void freeStack(stack *S) {
  if (!S) return;
  if (S->free) {
    for (size_t i = 0; i < S->size; ++i)
      S->free(S->buffer[i]);
  }
  free(S->buffer);
  free(S);
}

//===================================================================
// Sets the show function for the stack
void setStackShow(stack *S, showStackItem show) {
  S->show = show;
}

//===================================================================
// Sets the string delimiter for showing items
void setStackDelim(stack *S, char *delim) {
  S->delim = delim;
}

//===================================================================
// Doubles the capacity of the stack
static void checkCapacity(stack *S) {
  if (S->size < S->capacity)
    return;
  S->capacity *= 2;
  S->buffer = safeRealloc(S->buffer, S->capacity * sizeof(void *));
}

//===================================================================
// Pushes an item onto the stack
void stackPush(stack *S, void *item) {
  if (S->copy) 
    item = S->copy(item);
  checkCapacity(S);
  S->buffer[S->size++] = item;
}

//===================================================================
// Pops an item from the stack
void *stackPop(stack *S) {
  if (isEmptyStack(S))
    return NULL;
  return S->buffer[--S->size];
}

//===================================================================
// Peeks at the top of the stack
void *stackTop(stack *S) {
  if (isEmptyStack(S))
    return NULL;
  return S->buffer[S->size - 1];
}

//===================================================================
// Shows the stack
void showStack(stack *S) {
  if (! S->show) {
    fprintf(stderr, "StackShow Error: show function not set\n");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < S->size; ++i) {
    S->show(S->buffer[i]);
    printf("%s", i == S->size - 1 ? "\n" : S->delim);
  }
}
