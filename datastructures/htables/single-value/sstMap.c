/* 
  Specialized string - size_t map implementation:
    keys are strings, values are size_t
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include <ctype.h>
#include <string.h>
#include "sstMap.h"
#include "../../../lib/clib.h"

//===================================================================
// FNV-1a hash function
// (http://www.isthe.com/chongo/tech/comp/fnv/index.html)
static uint64_t sstMapHash(void *key, uint64_t seed, 
                           sstMapCase htCase) {

  char *str = (char *)key;
  char ch;
  // FNV offset basis and magic seed
  uint64_t hash = 14695981039346656037ULL + seed;  
  while ((ch = *str++)) {
    hash ^= ch;
    hash *= 1099511628211ULL;  // FNV prime
  }
  return hash;
}

//===================================================================
// case sensitive hash
static uint64_t hashCS(void *key, uint64_t seed) {
  return sstMapHash(key, seed, CASE_SENSITIVE);
}

//===================================================================
// case insensitive hash
static uint64_t hashCI(void *key, uint64_t seed) {
  return sstMapHash(key, seed, CASE_INSENSITIVE);
}

//===================================================================
// case sensitive comparison for keys
static int cmpStrCS(void const *str1, void const *str2) {
  return strcmp((char *)str1, (char *)str2);
}

//===================================================================
// case insensitive comparison for keys
// returns negative if str1 < str2
// returns 0 if str1 == str2
// returns positive if str1 > str2
static int cmpStrCI(void const *str1, void const *str2) {
  char *s1 = (char *)str1;
  char *s2 = (char *)str2;
  while (*s1 && *s2) {
    int diff = tolower(*s1) - tolower(*s2);
    if (diff)
      return diff;
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

//===================================================================
// shows the key 
static void showStr(void *str) {
  printf("%s", (char *)str);
}

//===================================================================
// Copy function for key 
static void *copyStr(void *str) {
  char *newKey = safeCalloc(1, strlen((char *)str) + 1);
  strcpy(newKey, (char *)str);
  return newKey;
}

//===================================================================
// shows the value
static void showSize(void *size) {
  printf("%zu", *(size_t *)size);
}

//===================================================================
// creates a new map
sstMap *sstMapNew(sstMapCase mapCase, size_t capacity) {
  mapHash hash = mapCase == CASE_SENSITIVE ? 
                            hashCS : hashCI;
  mapCompKey cmpKey = mapCase == CASE_SENSITIVE ? 
                                 cmpStrCS : cmpStrCI;
  map *M = mapNew(hash, capacity, cmpKey); 
  mapSetLabel(M, "sstMap");
  mapSetShow(M, showStr, showSize);
  mapOwnVals(M, free);  // we make copies of the size_t values, 
                        // so we have to free them at the end
  return (sstMap *)M;
}

//===================================================================
// sets the label for the map
void sstMapSetLabel(sstMap *M, char *label) {
  mapSetLabel((map *)M, label);
}

//===================================================================
// sets the map to copy the keys
void sstMapCopyKeys(sstMap *M) {
  mapCopyKeys((map *)M, copyStr, free);
}

//===================================================================
// sets the map to own the keys
void sstMapOwnKeys(sstMap *M) {
  mapOwnKeys((map *)M, free);
}

//===================================================================
// deallocates the map
void sstMapFree(sstMap *M) {
  mapFree((map *)M);
}

//===================================================================
// returns true if the key exists and sets the
// value pointer to the value associated with the key
bool sstMapHasKeyVal(sstMap *M, char *key, size_t *value) {
  size_t *val = mapGetVal((map *)M, (void *)key);
  if (val) {
    *value = *val;
    return true;
  }
  return false;
}

//===================================================================
// returns true if the key exists
bool sstMapHasKey(sstMap *M, char *key) {
  return mapHasKey((map *)M, (void *)key);
}

//===================================================================
// adds a key-value pair to the map; if the key
// exists, the value is updated
void sstMapAddKey(sstMap *M, char *key, size_t value) {
  size_t *val = safeCalloc(1, sizeof(size_t));
  *val = value;
  mapAddKey((map *)M, (void *)key, (void *)val);
}

//===================================================================
// returns the value associated with the key
// returns 0 if the key is not found
size_t sstMapGetVal(sstMap *M, char *key) {
  size_t *val = (size_t *)mapGetVal((map *)M, (void *)key);
  if (val)
    return *val;
  return 0;
}

//===================================================================
// removes the key and its value
// true if the key was removed
// false if the key was not found
bool sstMapDelKey(sstMap *M, char *key) {
  return mapDelKey((map *)M, (void *)key);
}

//===================================================================
// shows the map
void sstMapShow(sstMap *M) {
  mapShow((map *)M);
}

//===================================================================
// shows a key and its value
void sstMapShowEntry(sstMap *M, char *key) {
  mapShowEntry((map *)M, (void *)key);
}

//===================================================================
// shows distribution statistics
void sstMapStats(sstMap *M) {
  mapStats((map *)M);
}

//===================================================================
// returns the number of keys in the map
inline size_t sstMapSize(sstMap *M) {
  return mapSize((map *)M);
}

//===================================================================
// returns true if the map is empty
inline bool sstMapIsEmpty(sstMap *M) {
  return isEmptyMap((map *)M);
}

//===================================================================
// returns the first key-value pair in the map and
// updates the iterator to the next pair
// returns NULL if the map is empty
mapEntry *sstMapFirst(sstMap *M) {
  return mapFirst((map *)M);
}

//===================================================================
// returns the key-value pair at current position
// of iterator and sets the iterator to the next pair
// returns NULL if the end of the map is reached
mapEntry *sstMapNext(sstMap *M) {
  return mapNext((map *)M);
}

//===================================================================
// merges the keys and values of map M2 into map M1
// if a key exists in both maps, the value in M1 is updated
// M2 is destroyed
void sstMapMerge(sstMap *M1, sstMap *M2) {
  for (mapEntry *e = sstMapFirst(M2); e; e = sstMapNext(M2)) {
    char *key = (char *)e->key;
    size_t value = *(size_t *)e->value;
    sstMapAddKey(M1, key, value);
  }
  sstMapFree(M2);
}