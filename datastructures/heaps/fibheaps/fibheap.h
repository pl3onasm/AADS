/* 
  Generic Fibonacci heap interface, using fibonacci heaps
    Uses a hash table mapping data to nodes in the heap 
    (str(data) -> fibnode ptr)
    String representation of data should be unique for each 
    data item
    The comparison function should compare the keys of the
    nodes in the queue (not the data) and return -1, 0, 1
    for less than, equal to, greater than, respectively.
    Mind that if you use values as large as INT_MAX or INT_MIN
    for the sentinel key, you give a correct comparison function
    that can handle these values.
    The sentinel key is used to initialize the key of a node
    that is to be deleted from the heap. The key is set to the
    sentinel key, and the node is popped from the heap.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef FIBHEAP_H_INCLUDED
#define FIBHEAP_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../../htables/single-value/map.h"

// function pointer types
typedef int (*fibCompKey)(void const *a, void const *b);
typedef void (*fibFreeKey)(void *key);
typedef void *(*fibCopyKey)(void const *key);
typedef void (*fibShowKey)(void const *key);
typedef char *(*fibToString)(void const *data);
typedef void (*fibShowData)(void const *data);
typedef void (*fibFreeData)(void *data);
typedef void *(*fibCopyData)(void const *data);

// Fibonacci heap type
typedef enum { MIN, MAX } fibType;

// Fibonacci heap node
typedef struct fibnode {
  void *key;              // key (priority) of the node
  void *data;             // data associated with the key
  size_t degree;          // degree of the node
  bool mark;              // mark of the node
  struct fibnode *parent; // parent of the node
  struct fibnode *child;  // child of the node
  struct fibnode *prev;   // previous node in the list
  struct fibnode *next;   // next node in the list
} fibnode;

// Fibonacci heap
typedef struct {     
  fibnode *top;           // pointer to the top node of the heap
  map *datamap;           // map (string -> fibnode ptr) for data
  fibToString toString;   // function to convert data to string
  fibShowData showData;   // function to show data                        
  size_t size;            // number of nodes in the queue
  fibCompKey compKey;     // comparison function for the keys
  fibShowKey showKey;     // show function
  fibFreeData freeData;   // function to free data
  fibCopyData copyData;   // function to copy data
  fibFreeKey freeKey;     // function to free key
  fibCopyKey copyKey;     // function to copy key
  fibType type;           // type of Fibonacci heap (MIN or MAX)
  int fac;                // factor for comparison
  char *label;            // label for the Fibonacci heap
                          // default is "FIB HEAP"
  void *sentinel;         // sentinel key for deletion:
                          // a non-inclusive upper limit for a max heap
                          // a non-inclusive lower limit for a min heap
                          
} fibheap;

// function prototypes

  // creates a new Fibonacci heap, with given type (MIN / MAX),
  // key comparison function, key copy function, key free function,
  // key show function, and sentinel key for deletion
fibheap *fibNew(fibType type, fibCompKey cmp, fibCopyKey copy,
                fibFreeKey free, fibToString toString, void *sentinel);

  // makes the Fibonacci heap own what is in the heap when the heap
  // is deallocated (freeData is called on each data item)
void fibSetOwner(fibheap *F, fibFreeData freeData);

  // makes the Fibonacci heap work with copies of the data; this means
  // that if data is popped from the heap, you get a copy of the data,
  // not the original data itself, meaning you need to deallocate both 
  // the copy and the original data
void fibSetCopy(fibheap *F, fibCopyData copyData, fibFreeData freeData);

  // sets the show function for the Fibonacci heap
void fibSetShow(fibheap *F, fibShowKey showKey, fibShowData showData);

  // sets the label for the Fibonacci heap
void fibSetLabel(fibheap *F, char *label);

  // deallocates the Fibonacci heap
void fibFree(fibheap *F);

  // inserts a new node into the Fibonacci heap with
  // the given data and key (priority)
void fibPush(fibheap *F, void *data, void *key);

  // returns the top element of the Fibonacci heap without removing it
  // from the heap; returns NULL if the queue is empty
void *fibPeek(fibheap *F);

  // returns and removes the top element from the Fibonacci heap
  // returns NULL if the heap is empty
void *fibPop(fibheap *F);

  // updates the priority of a node in the heap
  // returns true if the update was successful
bool fibUpdateKey(fibheap *F, void *data, void *newKey);

  // returns true if the data is in the Fibonacci heap
bool fibContains(fibheap *F, void *data);

  // returns the key (priority) of the data in the heap
  // returns NULL if the data is not in the heap
void *fibGetKey(fibheap *F, void *data);

  // deletes the node containing the data from the Fibonacci heap
  // returns true if the deletion was successful
bool fibDelete(fibheap *F, void *data);

  // returns the size of the Fibonacci heap
static inline size_t fibSize(fibheap *F) {
  return F->size;
}

  // returns true if the heap is empty
static inline bool fibIsEmpty(fibheap *F) {
  return F->size == 0;
}

  // shows the Fibonacci heap
void fibShow(fibheap *F);

#endif  // FIBHEAP_H_INCLUDED