#include "unionFind.h"
#include "../../lib/clib/clib.h"

//===================================================================
// Creates a new union-find data structure with n elements
unionFind *ufNew(size_t capacity, ufToString toString) {
  unionFind *uf = safeCalloc(1, sizeof(unionFind)); 
  uf->sets = safeCalloc(capacity, sizeof(ufSet *));
  uf->size = 0;
  uf->capacity = capacity;
  uf->indexMap = sstMapNew(CASE_SENSITIVE, capacity);
  sstMapCopyKeys(uf->indexMap);
  uf->toString = toString;
  return uf;
}

//===================================================================
// Creates a new union-find set
static ufSet *ufSetNew(size_t parent, size_t rank) {
  ufSet *set = safeCalloc(1, sizeof(ufSet));
  set->parent = parent;
  set->rank = rank;
  return set;
}

//===================================================================
// Deallocates the union-find data structure
void ufFree(unionFind *uf) {
  if (!uf) return;
  for (size_t i = 0; i < uf->size; i++) {
    if (uf->freeData)
      uf->freeData(uf->sets[i]->data);
    free(uf->sets[i]);
  }
  sstMapFree(uf->indexMap);
  free(uf->sets);
  free(uf);
}

//===================================================================
// Makes the union-find own the input data
void ufOwnData(unionFind *uf, ufFreeData freeData) {
  uf->freeData = freeData;
}

//===================================================================
// Makes the union-find work with copies of the input data
void ufCopyData(unionFind *uf, ufCpyData copyData, 
                ufFreeData freeData) {
  uf->copyData = copyData;
  uf->freeData = freeData;
}

//===================================================================
// Adds a new set to the union-find data structure
void ufAddSet(unionFind *uf, void *data) {
  if (sstMapHasKey(uf->indexMap, uf->toString(data)))
    return;

  if (uf->size >= uf->capacity) {
    uf->capacity *= 2;
    uf->sets = safeRealloc(uf->sets, uf->capacity * sizeof(ufSet *));
  }
  
  if (uf->copyData)
    data = uf->copyData(data);

  size_t idx = uf->size;
  ufSet *set = ufSetNew(idx, 0);
  sstMapAddKey(uf->indexMap, uf->toString(data), idx);
  uf->sets[idx] = set;
  uf->sets[idx]->data = data;
  uf->size++;
}

//===================================================================
// Finds the root of the set containing the given index
static size_t ufFindIdx(unionFind *uf, size_t idx) {
  size_t parent = uf->sets[idx]->parent;
  if (parent == idx) 
    return parent;
  return ufFindIdx(uf, parent);
}

//===================================================================
// Finds the root of the set containing the given data
// returns NULL if the data is not in a set
void *ufFindSet(unionFind *uf, void *data) {
  size_t idx;
  if (!sstMapHasKeyVal(uf->indexMap, uf->toString(data), &idx))
    return NULL;
  return uf->sets[ufFindIdx(uf, idx)]->data;
}

//===================================================================
// Unites the sets given the roots of the sets
static void link(unionFind *uf, size_t x, size_t y) {
  if (x >= uf->size || y >= uf->size) return;
  if (uf->sets[x]->rank > uf->sets[y]->rank) {
    uf->sets[y]->parent = x;
  } else {
    uf->sets[x]->parent = y;
    if (uf->sets[x]->rank == uf->sets[y]->rank)
      uf->sets[y]->rank++;
  }
}

//===================================================================
// Unifies the sets containing the given data
void ufUnify(unionFind *uf, void *data1, void *data2) {
  size_t idx1, idx2;
  if (!sstMapHasKeyVal(uf->indexMap, uf->toString(data1), &idx1) ||
      !sstMapHasKeyVal(uf->indexMap, uf->toString(data2), &idx2))
    return;
  link(uf, ufFindIdx(uf, idx1), ufFindIdx(uf, idx2));
  uf->size--;
}

//===================================================================
// Returns true if the data is in the union-find data structure
bool ufContains(unionFind *uf, void *data) {
  return sstMapHasKey(uf->indexMap, uf->toString(data));
}

//===================================================================
// Returns the rank of the set containing the data
size_t ufRank(unionFind *uf, void *data) {
  size_t idx;
  if (!sstMapHasKeyVal(uf->indexMap, uf->toString(data), &idx))
    return 0;
  return uf->sets[idx]->rank;
}

//===================================================================
// Returns true if the data is in the same set as the other data
bool ufSameSet(unionFind *uf, void *data1, void *data2) {
  size_t idx1, idx2;
  if (!sstMapHasKeyVal(uf->indexMap, uf->toString(data1), &idx1) ||
      !sstMapHasKeyVal(uf->indexMap, uf->toString(data2), &idx2))
    return false;
  return ufFindIdx(uf, idx1) == ufFindIdx(uf, idx2);
}


