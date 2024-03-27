/* 
  Specialized string hash table: 
    both keys and values are strings.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include <ctype.h>
#include <string.h>
#include "sshtable.h"

  // FNV-1a hash function
static uint64_t ssHtHash(void *key, uint64_t seed, 
                         ssHtCase htCase) {

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

  // case sensitive hash
static uint64_t hashCS(void *key, uint64_t seed) {
  return ssHtHash(key, seed, CASE_SENSITIVE);
}

  // case insensitive hash
static uint64_t hashCI(void *key, uint64_t seed) {
  return ssHtHash(key, seed, CASE_INSENSITIVE);
}

  // case sensitive comparison
  // same for keys and values
static int cmpStrCS(void const *str1, void const *str2) {
  return strcmp((char *)str1, (char *)str2);
}

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

  // shows the key or value
static void showStr(void *str) {
  printf("%s", (char *)str);
}

static void *copyStr(void *str) {
  char *newKey = safeMalloc(strlen((char *)str) + 1);
  strcpy(newKey, (char *)str);
  return newKey;
}

  // creates a new string-string hash table
ssHt *ssHtNew(ssHtCase htCase, size_t capacity) {
  htHash hash = htCase == CASE_SENSITIVE ? hashCS : hashCI;
  htCmpKey cmpKey; 
  htCmpValue cmpVal;
  cmpKey = cmpVal = htCase == CASE_SENSITIVE ? cmpStrCS : cmpStrCI;
  ht *ht = htNew(hash, cmpKey, cmpVal, capacity);
  htSetShow(ht, showStr, showStr);
  return (ssHt *)ht;
}

void ssHtSetLabel(ssHt *ssht, char *label) {
  htSetLabel((ht *)ssht, label);
}

void ssHtSetValDelim(ssHt *ssht, char *valDelim) {
  htSetValDelim((ht *)ssht, valDelim);
}

void ssHtOwnKeys(ssHt *ssht) {
  htOwnKeys((ht *)ssht, free);
}

void ssHtOwnVals(ssHt *ssht) {
  htOwnVals((ht *)ssht, free);
}

void ssHtCopyKeys(ssHt *ssht) {
  htCopyKeys((ht *)ssht, copyStr, free);
}

void ssHtCopyVals(ssHt *ssht) {
  htCopyVals((ht *)ssht, copyStr, free);
}

bool ssHtHasKeyVals(ssHt *ssht, char *key, dll **values) {
  return (ssHt *)htHasKeyVals((ht *)ssht, (void *)key, values);
}

dll *ssHtGetVals(ssHt *ssht, char *key) {
  return htGetVals((ht *)ssht, (void *)key);
}

void ssHtAddKey(ssHt *ssht, char *key) {
  htAddKey((ht *)ssht, (void *)key);
}

void ssHtAddKeyVal(ssHt *ssht, char *key, char *value) {
  htAddKeyVal((ht *)ssht, (void *)key, (void *)value);
}

bool ssHtHasKey(ssHt *ssht, char *key) {
  return htHasKey((ht *)ssht, (void *)key);
}

void ssHtAddKeyVals(ssHt *ssht, char *key, char **values, size_t len) {
  for (size_t i = 0; i < len; i++)
    ssHtAddKeyVal(ssht, key, values[i]);
}

void ssHtDelKey(ssHt *ssht, char *key) {
  htDelKey((ht *)ssht, (void *)key);
}

void ssHtDelVal(ssHt *ssht, char *key, char *value) {
  htDelVal((ht *)ssht, (void *)key, (void *)value);
}

void ssHtShow(ssHt *ssht) {
  htShow((ht *)ssht);
}

void ssHtShowEntry(ssHt *ssht, char *key) {
  htShowEntry((ht *)ssht, (void *)key);
}

void ssHtStats(ssHt *ssht) {
  htStats((ht *)ssht);
}

void ssHtFree(ssHt *ssht) {
  htFree((ht *)ssht);
}

size_t ssHtSize(ssHt *ssht) {
  return htSize((ht *)ssht);
}

bool ssHtIsEmpty(ssHt *ssht) {
  return htIsEmpty((ht *)ssht);
}

size_t ssHtKeySize(ssHt *ssht, char *key) {
  return htKeySize((ht *)ssht, (void *)key);
}

void ssHtReset(ssHt *ssht) {
  htReset((ht *)ssht);
}

htEntry *ssHtNext(ssHt *ssht) {
  return (htEntry *)htNext((ht *)ssht);
}



  