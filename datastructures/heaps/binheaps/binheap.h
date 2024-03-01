/* 
  Generic binary heap implementation
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef BINHEAP_H_INCLUDED
#define BINHEAP_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// macros
#define RIGHT(i) (2 * i + 2)
#define LEFT(i) (2 * i + 1)
#define PARENT(i) ((i - 1) / 2)

// binary heap structure
typedef enum { MIN, MAX } hpType;

typedef struct {     
  void **arr;                  // array of void pointers
  size_t size;                 // number of nodes in the heap
  size_t capacity;             // capacity of the heap
  int (*cmp)(const void *, const void *);  // comparison function
  hpType hpType;               // type of heap (min or max)
} binheap;

// function prototypes
binheap *newBinHeap(size_t capacity, hpType type,
    int (*cmp)(const void *, const void *));
void freeBinHeap(binheap *H);
void *peekAtBinHeap(binheap *H);
void *popFromBinHeap(binheap *H);
void pushToBinHeap(binheap *H, void *node);
void heapifyBinHeap(binheap *H, size_t idx);
binheap *buildBinHeap(void *arr, size_t nElems, size_t elemSize,
  hpType type, int (*cmp)(const void *, const void *));
void showBinHeap(binheap *H, char *delim, void (*showData)(const void *));

#endif  // BINHEAP_H_INCLUDED