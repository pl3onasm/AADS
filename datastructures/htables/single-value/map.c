/* 
  Generic hash table implementation.
  Keys can have only one value.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include <time.h>
#include "map.h"
#include "../../../lib/clib.h"

//=================================================================
// Creates a new map
map *mapNew(mapHash hash, size_t capacity, 
            mapCompKey cmpKey) {
  
  map *M = safeCalloc(1, sizeof(map));
  M->capacity = capacity < 32 ? 32 : capacity;
  M->buckets = safeCalloc(M->capacity, sizeof(dll*));
  M->hash = hash;
  M->cmpKey = cmpKey;
  M->label = "Map";
  srand(time(NULL));
  M->seed = rand();
  M->seed ^= (uint64_t)time(NULL) << 16;
  return M;
}

//=================================================================
// Setters

void mapSetLabel(map *M, char *label) {
  M->label = label;
}

void mapSetShow(map *M, mapShowKey showKey, 
               mapShowValue showValue) {
  M->showKey = showKey;
  M->showValue = showValue;
}

void mapOwnKeys(map *M, mapFreeKey freeKey) {
  M->freeKey = freeKey;
}

void mapOwnVals(map *M, mapFreeValue freeValue) {
  M->freeValue = freeValue;
}

void mapCopyKeys(map *M, mapCopyKey copyKey, 
                 mapFreeKey freeKey) {
  M->copyKey = copyKey;
  M->freeKey = freeKey;
}

void mapCopyVals(map *M, mapCopyValue copyValue, 
                 mapFreeValue freeValue) {
  M->copyValue = copyValue;
  M->freeValue = freeValue;
}

//=================================================================
// Deallocates the map
void mapFree(map *M) {
  for (size_t i = 0; i < M->capacity; i++) {
    if (M->buckets[i]) {
      dll *bucket = M->buckets[i];
      for (mapEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) {
        if (M->freeKey)
          M->freeKey(e->key);         
        if (M->freeValue)
          M->freeValue(e->value);
        free(e);
      }
      dllFree(bucket);
    }
  }
  free(M->buckets);
  free(M);
}

//=================================================================
// Returns the index of the bucket for a key
static size_t getIndex(map *M, void *key) {
  return (M->hash(key, M->seed) % M->capacity);
}

//=================================================================
// Returns true if the key exists and sets the pointer to the value
bool mapHasKeyVal(map *M, void *key, void **value) {
  size_t index = getIndex(M, key);
  dll *bucket = M->buckets[index];
  if (! bucket || dllIsEmpty(bucket))
    return false;
  for (mapEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) {
    if (! M->cmpKey(key, e->key)) {
      *value = e->value;
      return true;
    }
  }
  return false;
}

//=================================================================
// Returns true if the key exists
bool mapHasKey(map *M, void *key) {
  if (! M->buckets[getIndex(M, key)])
    return false;
  void *value = NULL;
  return mapHasKeyVal(M, key, &value);
}

//=================================================================
// Returns the key from the table given an identifying key
void *mapGetKey(map *M, void *key) {
  size_t index = getIndex(M, key);
  dll *bucket = M->buckets[index];
  if (! bucket) 
    return NULL;
  for (mapEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) {
    if (! M->cmpKey(key, e->key))
      return e->key;
  }
  return NULL;
}

//=================================================================
// Returns the value given a key; 
// returns NULL if the key is not found
void *mapGetVal(map *M, void *key) {
  void *value = NULL;
  mapHasKeyVal(M, key, &value);
  return value;
}

//=================================================================
// rehashes the hash table if the number of 
// keys exceeds 75% of the capacity
static void mapRehash(map *M) {
  
  if (M->nKeys < 0.75 * M->capacity)
    return;
  
    // create new buckets
  size_t oldCapacity = M->capacity;
  M->capacity *= 2;
  dll **newBuckets = safeCalloc(M->capacity, sizeof(dll*));
  M->nFilled = 0;
  
    // rehash old entries
  for (size_t i = 0; i < oldCapacity; i++) {
    
    dll *bucket = M->buckets[i];
    if (! bucket)
      continue;
    
      // rehash all entries in the bucket to new buckets
    for (mapEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) {
      size_t newIndex = getIndex(M, e->key);
      if (! newBuckets[newIndex]) {
        newBuckets[newIndex] = dllNew();
        M->nFilled++;
      } 
      dllPush(newBuckets[newIndex], e);
    }
      // remove the old bucket without freeing the entries
    dllFree(bucket);
  }

    // free the old buckets and set the new ones
  free(M->buckets);
  M->buckets = newBuckets;
}

//=================================================================
// Adds a new key-value pair to the map
static void mapAddNewkeyVal(map *M, void *key, void *value, 
                            dll *bucket) {
  
  mapEntry *entry = safeCalloc(1, sizeof(mapEntry));

    // copy the key if a copy function is provided
  entry->key = M->copyKey ? M->copyKey(key) : key;

    // copy the value if a copy function is provided
  entry->value = M->copyValue ? M->copyValue(value) : value;

    // add the new key-value pair to the bucket
  dllPush(bucket, entry);
    // one key more
  M->nKeys++;
}

//=================================================================
// Tries to add a key-value pair to the table; if the key
// already exists, its value is updated with the new value
void mapAddKey(map *M, void *key, void *value) {
    
    // rehash if necessary
  mapRehash(M);

  size_t index = getIndex(M, key);
  
  if (! M->buckets[index]) {
    M->buckets[index] = dllNew();
    // no ownership for the entries, since we want
    // to keep the entries when rehashing, but still
    // want to free the bucket after rehashing
    M->nFilled++;
  }
  
    // if the key exists, update the value
  dll *bucket = M->buckets[index];
  for (mapEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) 
    if (! M->cmpKey(key, e->key)) {
        // free the old value if a free function is provided
      if (M->freeValue) 
        M->freeValue(e->value);
      
        // copy the new value if a copy function is provided
      e->value = M->copyValue ? M->copyValue(value) : value;
      return;
    }

    // if the key does not exist,
    // add a new key-value pair
  mapAddNewkeyVal(M, key, value, bucket);
}

//=================================================================
// Deletes a key from the map
// returns true if the key was removed, false if not found
bool mapDelKey(map *M, void *key) {
  size_t index = getIndex(M, key);
  dll *bucket = M->buckets[index];
  if (! bucket) 
    return false;
  
  for (mapEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) {
    if (! M->cmpKey(key, e->key)) {
        // free key if a free function is provided
      if (M->freeKey) 
        M->freeKey(e->key);
        // free value if a free function is provided
      if (M->freeValue) 
        M->freeValue(e->value);
        // free the entry itself
      free(e);
        // remove the entry from the bucket
      dllDeleteCurr(bucket);
        // one key less
      M->nKeys--;
        // update statistics
      if (dllIsEmpty(bucket))
        M->nFilled--;
      return true;
    }
  }
  return false;
}

//=================================================================
// Resets the iterator
mapEntry *mapFirst(map *M) {
  M->iterBucket = 0;
  M->iterNode = NULL;
  return mapNext(M);
}

//=================================================================
// Returns the key-value pair of the current 
// iterator position and sets the iterator to  
// the next key-value pair
// returns NULL if end of the map is reached
mapEntry *mapNext(map *M) {

    // if the end of the table is reached, 
    // return NULL
  if (M->iterBucket >= M->capacity) 
    return NULL;

    // skip empty buckets
  if (M->buckets[M->iterBucket] == NULL || 
      dllIsEmpty(M->buckets[M->iterBucket])) {
    M->iterBucket++;
    M->iterNode = NULL;
    return mapNext(M);
  }
  
  if (M->iterNode == M->buckets[M->iterBucket]->NIL) {
      // we are at the end of the list for this bucket
      // move to the next bucket
    M->iterBucket++;
    M->iterNode = NULL;
    return mapNext(M);
  }

  if (M->iterNode == NULL) {
      // we are at the beginning of the list 
      // for this bucket; move to the first node
    M->iterNode = M->buckets[M->iterBucket]->NIL->next;
  }

    // return the current entry and move 
    // the iterator to the next node
  mapEntry *entry = M->iterNode->dllData;
  M->iterNode = M->iterNode->next;
  
  return entry; 
}

//=================================================================
// Computes the maximum size of the buckets
size_t mapMaxBucketSize(map *M) {
  size_t max = 0;
  for (size_t i = 0; i < M->capacity; i++) 
    if (M->buckets[i] && M->buckets[i]->size > max)
      max = M->buckets[i]->size;
  return max;
}

//=================================================================
// Gives an overview of the distribution of keys 
// over the buckets
void mapStats(map *M) {
  printf("\n+---------------------------+\n"
         "|      Map statistics       |\n"
         "+---------------------------+\n\n"
         "   Number of buckets..: %zu\n"
         "   Buckets used.......: %zu\n"
         "   Number of keys.....: %zu\n"
         "   Load factor........: %.2f\n"
         "   Max. bucket size...: %zu\n"
         "   Collisions.........: %zu\n\n\n",
         M->capacity, M->nFilled, M->nKeys,
         (double)M->nKeys / M->capacity, 
         mapMaxBucketSize(M),
         M->nKeys - M->nFilled);
}

//=================================================================
// Shows the map
void mapShow(map *M) {
  if (!M->showKey || !M->showValue) {
    fprintf(stderr, "htShow: showKey or " 
                    "showValue function not set\n");
    return;
  }
  
  printf("\n--------------------\n"
          "  %s [%zu]\n"
          "--------------------\n", 
          M->label, M->nKeys);

  for (mapEntry *e = mapFirst(M); e; e = mapNext(M)) {
    printf("  ");
    mapShowEntry(M, e->key);
  }
  
  printf("--------------------\n\n");
}

//=================================================================
// Shows a key-value pair
void mapShowEntry(map *M, void *key) {
  if (!M->showKey || !M->showValue) {
    fprintf(stderr, "htShowEntry: showKey or " 
                    "showValue function not set\n");
    return;
  }
  void *value;
  if (! mapHasKeyVal(M, key, &value)) {
    printf("Key not found\n");
    return;
  }
  M->showKey(key);
  printf(": ");
  M->showValue(value);
  printf("\n");
}

//=================================================================
// Returns a copy of the map
map *mapCopy(map *M) {
  
  map *copy = mapNew(M->hash, M->capacity, M->cmpKey);
  if (M->copyKey)
    mapCopyKeys(copy, M->copyKey, M->freeKey);
  if (M->copyValue)
    mapCopyVals(copy, M->copyValue, M->freeValue);
  if (M->showKey && M->showValue) {
    mapSetShow(copy, M->showKey, M->showValue);
    copy->label = "Map copy";
  }
  
  for (mapEntry *e = mapFirst(M); e; e = mapNext(M)) 
    mapAddKey(copy, e->key, e->value);
  
  return copy;
}

//=================================================================
// Merges the keys of the smaller map into the larger map
map *mapMerge(map *M1, map *M2) {
  
  if (M1->hash != M2->hash || M1->cmpKey != M2->cmpKey) {
    fprintf(stderr, "mapMerge: hash or cmpKey functions differ\n");
    return NULL;
  }

  if (mapSize(M1) < mapSize(M2)) 
    SWAP(M1, M2);

  for (mapEntry *e = mapFirst(M2); e; e = mapNext(M2)) 
    mapAddKey(M1, e->key, e->value);

  mapFree(M2);
  return M1;
}

