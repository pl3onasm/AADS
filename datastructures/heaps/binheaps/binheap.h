/* 
  Generic binary heap implementation
  Does not support update-key operations. For that,
  use a bpqueue or a fibheap
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef BINHEAP_H_INCLUDED
#define BINHEAP_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// function pointer types
typedef int (*bhpCompData)(void const *a, void const *b);
typedef void (*bhpShowData)(void const *data);
typedef void (*bhpFreeData)(void *data);
typedef void *(*bhpCopyData)(void const *data);

// binary heap type
typedef enum { MIN, MAX } bhpType;

// binary heap structure
typedef struct {     
  void **arr;              // array of void pointers
  size_t size;             // number of nodes in the heap
  size_t capacity;         // capacity of the heap
  bhpCompData cmp;         // comparison function
  bhpShowData show;        // show function
  bhpType hpType;          // type of heap (min or max)
  bhpCopyData copy;        // copy function
  bhpFreeData free;        // free function
  char *label;             // label for the heap
                           // default is "BINARY HEAP"
  char *delim;             // string delimter for show
                           // default is ", "
} binheap;

// function prototypes

  // creates a new binary heap, with given capacity, 
  // type (MIN / MAX), and comparison function
binheap *bhpNew(size_t capacity, bhpType type, 
                bhpCompData cmp);

  // sets the show function for the heap
void bhpSetShow(binheap *H, bhpShowData show);

  // sets the label for the heap
void bhpSetLabel(binheap *H, char *label);

  // sets the delimiter for the show function
void bhpSetDelim(binheap *H, char *delim);

  // sets the heap to own the input data, deallocating
  // what is still in the heap when it is destroyed
void bhpSetOwner(binheap *H, bhpFreeData free);

  // sets the queue to operate on copies of the data
void bhpSetCopy(binheap *H, bhpCopyData copy,
                bhpFreeData free);

  // deallocates the binary heap
void bhpFree(binheap *H);

  // returns the top element of the heap 
  // without removing it
void *bhpPeek(binheap *H);

  // removes the top element from the heap
void *bhpPop(binheap *H);

  // restores the binary heap property 
  // starting from the given index
void bhpHeapify(binheap *H, size_t idx);

  // adds a new node to the heap
void bhpPush(binheap *H, void *node);

  // builds a binary heap from an array
binheap *bhpBuild(void *arr, size_t nElems, 
    size_t elSize, bhpType hpType, bhpCompData cmp);

  // shows the binary heap
void bhpShow(binheap *H);

  // true if the heap is empty
static inline size_t bhpIsEmpty(binheap *H) {
  return H->size == 0;
} 

#endif  // BINHEAP_H_INCLUDED