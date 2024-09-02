/* 
  Generic map implementation, using separate chaining
  Keys have exactly one value associated with them
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>   // uint64_t
#include "../../lists/dll.h"

  // function pointer types
typedef uint64_t (*mapHash)(void *hashKey, uint64_t seed);
typedef int (*mapCompKey)(void const *key1, void const *key2);
typedef void (*mapShowKey)(void const *key);
typedef void (*mapShowValue)(void const *value);
typedef void (*mapFreeKey)(void *key);
typedef void (*mapFreeValue)(void *val);
typedef void *(*mapCopyKey)(void const *key);
typedef void *(*mapCopyValue)(void const *val);

  // hash map structure
typedef struct {
  size_t capacity;        // number of available buckets
  size_t nKeys;           // number of keys
  dll **buckets;          // array of doubly linked lists
  mapHash hash;           // hash function
  mapCompKey cmpKey;      // comparison function for the keys
  uint64_t seed;          // magic seed for the map
  size_t iterBucket;      // current bucket for the iterator
  dllNode *iterNode;      // current node for the iterator
  mapShowKey showKey;     // function to show the key
  mapShowValue showValue; // function to show the value
  mapFreeKey freeKey;     // function to free the key
  mapFreeValue freeValue; // function to free the value
  mapCopyKey copyKey;     // function to copy the key
  mapCopyValue copyValue; // function to copy the value
  size_t nFilled;         // number of filled buckets
  char *label;            // label for the map
} map;

typedef struct {          // key-value pair
  void *key;              // key
  void *value;            // value
} mapEntry;

  // map function prototypes

  // creates a new map
map *mapNew(mapHash hash, size_t capacity, 
            mapCompKey cmpKey);

  // sets the label for the map
  // default is "map"
void mapSetLabel(map *M, char *label);

  // sets the show functions for the map
void mapSetShow(map *M, mapShowKey showKey, 
                mapShowValue showValue);

  // sets the map to make copies of the keys
  // if set, the map will only free the copies
void mapCopyKeys(map *M, mapCopyKey copyKey, 
                 mapFreeKey freeKey);

  // sets the map to make copies of the values
  // if set, the map will only free the copies
void mapCopyVals(map *M, mapCopyValue copyValue, 
                 mapFreeValue freeValue);

  // sets the map to own the input keys,
  // freeing them when the map is freed
void mapOwnKeys(map *M, mapFreeKey freeKey);

  // sets the map to own the input values,
  // freeing them when the map is freed
void mapOwnVals(map *M, mapFreeValue freeValue);

  // dellocates the map
void mapFree(map *M);

  // returns true if the key exists and sets the
  // value pointer to the value associated with the key
bool mapHasKeyVal(map *M, void *key, void **value);

  // returns true if the key exists
bool mapHasKey(map *M, void *key);

  // adds a key-value pair to the map; if the key 
  // exists, the value is updated
void mapAddKey(map *M, void *key, void *value);

  // returns the key if it exists in the map
  // returns NULL if the key is not found
void *mapGetKey(map *M, void *key);

  // returns the value associated with the key
  // returns NULL if the key is not found
void *mapGetVal(map *M, void *key);

  // removes the key and its value 
  // true if the key was removed
  // false if the key was not found
bool mapDelKey(map *M, void *key);

  // shows the map
void mapShow(map *M);

  // shows a key and its value
void mapShowEntry(map *M, void *key);

  // shows distribution statistics
void mapStats(map *M);

  // returns a copy of the map
map *mapCopy(map *M);

  // merges the keys of M2 into M1
  // returns the merged map and deallocates M2
  // returns NULL if the hash or comparison functions differ
map *mapMerge(map *M1, map *M2);

  // returns the number of keys in the map
static inline size_t mapSize(map *M) {
  return M->nKeys;
}

  // returns true if the map is empty
static inline bool isEmptyMap(map *M) {
  return M->nKeys == 0;
}

  // returns the first key-value pair in the map and 
  // updates the iterator to the next pair; 
  // returns NULL if the map is empty
mapEntry *mapFirst(map *M);

  // returns the key-value pair at current position 
  // of iterator and sets the iterator to the next pair
  // returns NULL if the end of the map is reached
mapEntry *mapNext(map *M);

#endif  // MAP_H_INCLUDED