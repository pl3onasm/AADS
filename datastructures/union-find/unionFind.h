/* 
  Generic union find implementation
  with path compression and union by rank
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef UNIONFIND_H_INCLUDED
#define UNIONFIND_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include "../htables/single-value/string-size-t/sstMap.h"

typedef void (*ufFreeData)(void *data);
typedef void *(*ufCpyData)(void const *data);
typedef char *(*ufToString)(void const *data);

typedef struct ufSet {
  void *data;
  size_t size;
  size_t parent;
  size_t rank;
} ufSet;

typedef struct unionFind {
  ufSet **sets;
  sstMap *indexMap;
  size_t size;
  size_t capacity;
  ufFreeData freeData;
  ufCpyData copyData;
  ufToString toString;
} unionFind;

  // creates a new union-find structure
unionFind *ufNew(size_t capacity, ufToString toString);

  // sets the union-find to own the input data
  // frees the data when the union-find structure is freed
void ufOwnData(unionFind *uf, ufFreeData freeData);

  // sets the union-find to work on copies of the input data
void ufCopyData(unionFind *uf, ufCpyData cpyData,
                ufFreeData freeData);

  // create a new set with the given data
  // nothing happens if the data is already in a set
void ufAddSet(unionFind *uf, void *data);

  // deallocates the union-find structure
  // frees the data iff a freeData function is provided
void ufFree(unionFind *uf);

  // finds the root of the set containing the given data
  // returns NULL if the data is not in a set
void *ufFindSet(unionFind *uf, void *data);

  // unifies the sets containing the two data elements
void ufUnify(unionFind *uf, void *data1, void *data2);

  // returns true if the data element is in a set
  // returns false if the data element is not in a set
bool ufContains(unionFind *uf, void *data);

  // returns true if two data elements are in the same set
  // false otherwise or if the elements are not in a set
bool ufSameSet(unionFind *uf, void *data1, void *data2);

  // returns the number of sets in the union-find structure
static inline size_t ufNumSets(unionFind *uf) {
  return uf->size;
}

#endif  // UNIONFIND_H_INCLUDED

