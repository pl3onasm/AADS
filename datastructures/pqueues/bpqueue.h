/* 
  Generic priority queue interface, using binary heaps
  Supports updating priorities using a hash table
    mapping data to indices in the queue (str(data) -> idx)
    String representation of data should be unique for each 
    data item
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef BPQUEUE_H_INCLUDED
#define BPQUEUE_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../htables/single-value/sstmap.h"

// function pointer types
typedef int (*bpqCompKey)(void const *a, void const *b);
typedef void (*bpqFreeKey)(void *key);
typedef void *(*bpqCopyKey)(void *key);
typedef void (*bpqShowKey)(void const *key);
typedef char *(*bpqToString)(void const *data);
typedef void (*bpqShowData)(void const *data);
typedef void (*bpqFreeData)(void *data);
typedef void *(*bpqCpyData)(void *data);

// priority queue type
typedef enum { MIN, MAX } bpqType;

// priority queue node
typedef struct {
  void *key;             // key (priority) of the node
  void *data;            // data associated with the key
} bpqNode;

// priority queue
typedef struct {     
  bpqNode **arr;         // array of nodes
  sstMap *map;           // maps input to indices in the queue
  bpqToString toString;  // function to convert data to string
  bpqShowData showData;  // function to show data                        
  size_t size;           // number of nodes in the queue
  size_t capacity;       // capacity of the queue
  bpqCompKey compKey;    // comparison function for the keys
  bpqShowKey showKey;    // show function
  bpqFreeData freeData;  // function to free data
  bpqCpyData copyData;   // function to copy data
  bpqFreeKey freeKey;    // function to free key
  bpqCopyKey copyKey;    // function to copy key
  bpqType type;          // type of priority queue (MIN or MAX)
  char *label;           // label for the priority queue
                         // default is "BINARY PQ"
  char *delim;           // string delimter for show
                         // default is ", "
} bpqueue;

// function prototypes

  // creates a new priority queue, with given capacity, 
  // type (MIN / MAX), key comparison function, key copy
  // function, key free function, and data to string function
bpqueue *bpqNew(size_t capacity, bpqType type, 
                bpqCompKey cmp, bpqCopyKey copyKey,
                bpqFreeKey freeKey, bpqToString toString);

  // initializes the priority queue by setting all keys
  // (priorities) to a given value
void bpqInit(bpqueue *pq, void *initKey);

  // sets the show function for the priority queue
void bpqSetShow(bpqueue *pq, bpqShowKey show, 
                bpqShowData showData);

  // sets the label for the priority queue
void bpqSetLabel(bpqueue *pq, char *label);

  // sets the delimiter for the show function
void bpqSetDelim(bpqueue *pq, char *delim);

  // deallocates the priority queue
void bpqFree(bpqueue *pq);

  // sets the queue to own the input data;
  // freeing what is still stored in the queue 
  // when the queue is freed
void bpqOwnData(bpqueue *pq, bpqFreeData freeData);

  // sets the queue to make copies of the data
void bpqCopyData(bpqueue *pq, bpqCpyData copyData, 
                 bpqFreeData freeData);

  // returns the top element of the priority  
  // queue without removing it; returns NULL
  // if the queue is empty
void *bpqPeek(bpqueue *pq);

  // returns and removes the top element 
  // from the priority queue; returns NULL
  // if the queue is empty
void *bpqPop(bpqueue *pq);

  // updates the priority of a node in the heap
  // by updating the key and reordering the heap
  // if necessary
void bpqUpdateKey(bpqueue *pq, void *data, 
                  void *newKey);

  // adds a new node to the priority queue with
  // the given data and key (priority)
void bpqPush(bpqueue *pq, void *data, void *key);

  // returns true if the data is in the queue
bool bpqContains(bpqueue *pq, void *data);

  // returns the key (priority) of the data
  // in the queue; returns NULL if the data
  // is not in the queue
void *bpqGetKey(bpqueue *pq, void *data);

  // shows the priority queue
void bpqShow(bpqueue *pq);

  // true if the queue is empty
static inline bool bpqIsEmpty(bpqueue *pq) {
  return pq->size == 0;
} 

  // returns the number of items in the queue
static inline size_t bpqSize(bpqueue *pq) {
  return pq->size;
}

#endif  // BPQUEUE_H_INCLUDED