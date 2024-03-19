/* 
  Generic Doubly Linked List (DLL) implementation
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/


#ifndef _DLL_H_INCLUDED_
#define _DLL_H_INCLUDED_

#include <stdlib.h>
#include <stdio.h>  
#include <stdbool.h>

// function pointers for the DLL
typedef int (*dllCmpData)(void *a, void *b);
typedef void (*dllShowData)(void *data);
typedef void *(*dllCpyData)(void *data);
typedef void (*dllFreeData)(void *data);

// DLL data structures
typedef struct dllNode {
  void *dllData;          // pointer to the data
  struct dllNode *next;   // pointer to the next node
  struct dllNode *prev;   // pointer to the previous node
} dllNode;

typedef struct {
  dllNode *NIL;           // sentinel node
  dllNode *iter;          // iterator
  size_t size;            // size of the DLL
  dllCmpData cmp;         // comparison function
  dllShowData showData;   // show data function
  dllFreeData freeData;   // free data function
  dllCpyData copyData;    // copy data function
  char *delim;            // delimiter string
  char *label;            // label for the DLL
} dll;

// DLL function prototypes

  // Creates a new DLL 
dll *dllNew();

  // Sets the DLL to own the input data,
  // freeing it when the DLL is freed
void dllOwnData(dll *L, dllFreeData freeData);

  // Sets the DLL to make copies of the input data
  // If set, the DLL will only free the copies
void dllCopyData(dll *L, dllCpyData copyData, 
                    dllFreeData freeData);

  // Sets the delimiter function for the DLL
void dllSetDelim(dll *L, char *delim);

  // Sets the label for the DLL
void dllSetLabel(dll *L, char *label);

  // Sets the showData function for the DLL
void dllSetShow(dll *L, dllShowData showData);

  // Sets the comparison function for the DLL
void dllSetCmp(dll *L, dllCmpData cmp);

  // Frees the DLL
  // Input data is freed if the DLL owns it
void dllFree(dll *L);

  // Makes the DLL empty
void dllEmpty(dll *L);

  // Makes a copy of the DLL
dll *dllCopy(dll *L, dllCpyData copyData, 
             dllFreeData freeData);

  // Creates a new DLL node
dllNode *dllNewNode();

  // Frees a DLL node, including its data 
  // if the DLL owns it
void dllFreeNode(dll *L, dllNode *node);

  // Pushes data to the front of the DLL
void dllPush(dll *L, void *data);

  // Inserts data in a sorted DLL
void dllInsert(dll *L, void *data);

  // Pushes data to the back of the DLL
void dllPushBack(dll *L, void *data);

  // Deletes a node from the DLL
void dllDelete(dll *L, dllNode *node);

  // Deletes a node from the DLL given its data
void dllDeleteData(dll *L, void *data);

  // Searches for a node in the DLL with a given key
  // returns the data of the node if found, NULL otherwise
void *dllFind(dll *L, void *key);

  // Shows the DLL using the showData function 
  // and the delimiter string (default is ", ")
void dllShow(dll *L);

  // Sets the list iterator to the first node
void dllFirst(dll *L);

  // Sets the list iterator to the last node
void dllLast(dll *L);

  // Returns true if the iterator has reached
  // the end of the DLL
bool dllEnd(dll *L);

  // Returns the data of the current node
  // and updates the list iterator to the next node
  // value is NULL if the end of the DLL is reached
void *dllNext(dll *L);

  // Returns the data of the current node
  // and updates the list iterator to the previous node
  // value is NULL if the beginning of the DLL is reached
void *dllPrev(dll *L);

  // Checks if the DLL is empty
inline bool dllIsEmpty (dll *L) {
  return L->NIL->next == L->NIL;
}

  // Gets the size of the DLL
inline size_t dllSize (dll *L) {
  return L->size;
}

  // Gets the dllData of a node
inline void *dllData (dllNode *node) {
  return node->dllData;
}

#endif // _DLL_H_INCLUDED_