/* 
  Specialized string hash table: 
    both keys and values are strings.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include <ctype.h>
#include <string.h>
#include "sshtable.h"
#include "../../../lib/clib.h"

//===================================================================
// FNV-1a hash function
// (http://www.isthe.com/chongo/tech/comp/fnv/index.html)
static uint64_t sshtHash(void *key, uint64_t seed, 
                         sshtCase htCase) {

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
  return sshtHash(key, seed, CASE_SENSITIVE);
}

//===================================================================
// case insensitive hash
static uint64_t hashCI(void *key, uint64_t seed) {
  return sshtHash(key, seed, CASE_INSENSITIVE);
}

//===================================================================
// case sensitive comparison
// same for keys and values
static int cmpStrCS(void const *str1, void const *str2) {
  return strcmp((char *)str1, (char *)str2);
}

//===================================================================
// case insensitive comparison
// same for keys and values
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
// shows the key or value
static void showStr(void const *str) {
  printf("%s", (char *)str);
}

//===================================================================
// Copy function for key or value
static void *copyStr(void const *str) {
  char *newKey = safeMalloc(strlen((char *)str) + 1);
  strcpy(newKey, (char *)str);
  return newKey;
}

//===================================================================
// creates a new string-string hash table
sshtable *sshtNew(sshtCase htCase, size_t capacity) {
  htHash hash = htCase == CASE_SENSITIVE ? hashCS : hashCI;
  htCmpKey cmpKey; 
  htCmpValue cmpVal;
  cmpKey = cmpVal = htCase == CASE_SENSITIVE ? cmpStrCS : cmpStrCI;
  htable *ht = htNew(hash, cmpKey, cmpVal, capacity);
  htSetShow(ht, showStr, showStr);
  return (sshtable *)ht;
}

//===================================================================
// Casts

void sshtSetLabel(sshtable *ssht, char *label) {
  htSetLabel((htable *)ssht, label);
}

void sshtSetValDelim(sshtable *ssht, char *valDelim) {
  htSetValDelim((htable *)ssht, valDelim);
}

void sshtOwnKeys(sshtable *ssht) {
  htOwnKeys((htable *)ssht, free);
}

void sshtOwnVals(sshtable *ssht) {
  htOwnVals((htable *)ssht, free);
}

void sshtCopyKeys(sshtable *ssht) {
  htCopyKeys((htable *)ssht, copyStr, free);
}

void sshtCopyVals(sshtable *ssht) {
  htCopyVals((htable *)ssht, copyStr, free);
}

bool sshtHasKeyVals(sshtable *ssht, char *key, dll **values) {
  return (sshtable *)htHasKeyVals((htable *)ssht, (void *)key, values);
}

dll *sshtGetVals(sshtable *ssht, char *key) {
  return htGetVals((htable *)ssht, (void *)key);
}

void sshtAddKey(sshtable *ssht, char *key) {
  htAddKey((htable *)ssht, (void *)key);
}

void sshtAddKeyVal(sshtable *ssht, char *key, char *value) {
  htAddKeyVal((htable *)ssht, (void *)key, (void *)value);
}

bool sshtHasKey(sshtable *ssht, char *key) {
  return htHasKey((htable *)ssht, (void *)key);
}

void sshtAddKeyVals(sshtable *ssht, char *key, char **values, size_t len) {
  for (size_t i = 0; i < len; i++)
    sshtAddKeyVal(ssht, key, values[i]);
}

void sshtDelKey(sshtable *ssht, char *key) {
  htDelKey((htable *)ssht, (void *)key);
}

void sshtDelVal(sshtable *ssht, char *key, char *value) {
  htDelVal((htable *)ssht, (void *)key, (void *)value);
}

void sshtShowEntry(sshtable *ssht, char *key) {
  htShowEntry((htable *)ssht, (void *)key);
}

void sshtStats(sshtable *ssht) {
  htStats((htable *)ssht);
}

void sshtShow(sshtable *ssht) {
  htShow((htable *)ssht);
}

void sshtFree(sshtable *ssht) {
  htFree((htable *)ssht);
}

sshtable *sshtMerge(sshtable *ssht1, sshtable *ssht2) {
  return (sshtable *)htMerge((htable *)ssht1, (htable *)ssht2);
}

size_t sshtSize(sshtable *ssht) {
  return htSize((htable *)ssht);
}

bool sshtIsEmpty(sshtable *ssht) {
  return htIsEmpty((htable *)ssht);
}

size_t sshtKeySize(sshtable *ssht, char *key) {
  return htKeySize((htable *)ssht, (void *)key);
}

htEntry *sshtFirst(sshtable *ssht) {
  return (htEntry *)htFirst((htable *)ssht);
}

htEntry *sshtNext(sshtable *ssht) {
  return (htEntry *)htNext((htable *)ssht);
}



  