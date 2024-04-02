/* 
  Generic binary heap implementation
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef BINHEAP_H_INCLUDED
#define BINHEAP_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// function pointer types
typedef int (*hpCompData)(void const *a, void const *b);
typedef void (*hpShowData)(void const *data);

// binary heap type
typedef enum { MIN, MAX } hpType;

// binary heap structure
typedef struct {     
  void **arr;              // array of void pointers
  size_t size;             // number of nodes in the heap
  size_t capacity;         // capacity of the heap
  hpCompData cmp;          // comparison function
  hpShowData show;         // show function
  hpType hpType;           // type of heap (min or max)
  char *label;             // label for the heap
                           // default is "BINARY HEAP"
  char *delim;             // string delimter for show
                           // default is ", "
} binheap;

// function prototypes

  // creates a new binary heap, with given capacity, 
  // type (MIN / MAX), and comparison function
binheap *newBinHeap(size_t capacity, hpType type, 
                    hpCompData cmp);

  // sets the show function for the heap
void setBinHeapShow(binheap *H, hpShowData show);

  // sets the label for the heap
void setBinHeapLabel(binheap *H, char *label);

  // sets the delimiter for the show function
void setBinHeapDelim(binheap *H, char *delim);

  // deallocates the binary heap
void freeBinHeap(binheap *H);

  // returns the top element of the heap 
  // without removing it
void *peekAtBinHeap(binheap *H);

  // removes the top element from the heap
void *popFromBinHeap(binheap *H);

  // restores the binary heap property 
  // starting from the given index
void heapifyBinHeap(binheap *H, size_t idx);

  // adds a new node to the heap
void pushToBinHeap(binheap *H, void *node);

  // builds a binary heap from an array
binheap *buildBinHeap(void *arr, size_t nElems, 
    size_t elSize, hpType hpType, hpCompData cmp);

  // shows the binary heap
void showBinHeap(binheap *H);

  // true if the heap is empty
inline size_t isEmptyBinHeap(binheap *H) {
  return H->size == 0;
} 

#endif  // BINHEAP_H_INCLUDED