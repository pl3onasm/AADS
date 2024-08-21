/* 
  Generic stack, implemented as an array of void pointers.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>

typedef void (*showStackItem)(void const *item);
typedef void (*freeStackItem)(void *item);
typedef void *(*copyStackItem)(void const *item);

typedef struct {
  void** buffer;        // Array of void pointers
  size_t capacity;      // Capacity of the stack
  size_t size;          // Number of elements in the stack
  showStackItem show;   // Function to show an item
  freeStackItem free;   // Function to free an item
  copyStackItem copy;   // Function to copy an item
  size_t elSize;        // Size of an element, only for copying
  char *delim;          // Delimiter for showing items 
                        // (default: ", ")
} stack;


  // Creates a new stack with given capacity
stack* newStack(size_t capacity);

  // Sets the stack to own the input data, deallocating
  // what is still in the stack when it is destroyed
void setStackOwner(stack* S, freeStackItem free);

  // Sets the function to copy an item
void setStackCopy(stack* S, copyStackItem copy, 
                  size_t elSize, freeStackItem free);

  // Sets the function to show an item
void setStackShow(stack* S, showStackItem show);

  // Sets the string delimiter for showing items
void setStackDelim(stack* S, char* delim);

  // Destroys the stack
void freeStack(stack* S);

  // Pushes an element onto the stack
void stackPush(stack* S, void* element);

  // Pops an element from the stack
void* stackPop(stack* S);

  // Peeks at the top of the stack
void* stackTop(stack* S);

  // Shows the stack
void showStack(stack* S);

  // True if the stack is empty
static inline bool isEmptyStack(stack* S) {
  return S->size == 0;
}

#endif  // STACK_H_INCLUDED