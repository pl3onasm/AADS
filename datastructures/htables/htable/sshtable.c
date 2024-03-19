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
static uint ssHtHash(void *key, uint seed, 
                     ssHtCase htCase) {
  uint hash = 0;
  char *str = (char *)key;
  char ch;
  while ((ch = *str++)) {
    if (htCase == CASE_SENSITIVE)
      ch = tolower(ch);
    hash ^= ch;
    hash *= 16777619; // FNV prime
  }
  return hash;
}

  // case sensitive hash
static uint hashCS(void *key, uint seed) {
  return ssHtHash(key, seed, CASE_SENSITIVE);
}

  // case insensitive hash
static uint hashCI(void *key, uint seed) {
  return ssHtHash(key, seed, CASE_INSENSITIVE);
}

  // case sensitive comparison
  // same for keys and values
static int cmpStrCS(void *str1, void *str2) {
  return strcmp((char *)str1, (char *)str2);
}

  // case insensitive comparison
  // same for keys and values
  // returns negative if str1 < str2
  // returns 0 if str1 == str2
  // returns positive if str1 > str2
static int cmpStrCI(void *str1, void *str2) {
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
ssHt *ssHtNew(ssHtCase htCase) {
  htHash hash = htCase == CASE_SENSITIVE ? hashCS : hashCI;
  htCmpKey cmpKey; 
  htCmpValue cmpVal;
  cmpKey = cmpVal = htCase == CASE_SENSITIVE ? cmpStrCS : cmpStrCI;
  ht *ht = htNew(hash, cmpKey, cmpVal);
  htSetShow(ht, showStr, showStr);
  return (ssHt *)ht;
}

void ssHtSetLabel(ssHt *ssht, char *label) {
  htSetLabel((ht *)ssht, label);
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

bool ssHtGetKeyVals(ssHt *ssht, char *key, dll **values) {
  return (ssHt *)htGetKeyVals((ht *)ssht, (void *)key, values);
}

dll *ssHtGetVals(ssHt *ssht, char *key) {
  return htGetVals((ht *)ssht, (void *)key);
}

void ssHtAddKey(ssHt *ssht, char *key, char *value) {
  // make a copy of the key and value
  htAddKey((ht *)ssht, (void *)key, (void *)value);
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

void ssHtFree(ssHt *ssht) {
  htFree((ht *)ssht);
}

size_t ssHtSize(ssHt *ssht) {
  return htSize((ht *)ssht);
}

bool ssHtIsEmpty(ssHt *ssht) {
  return htIsEmpty((ht *)ssht);
}

void ssHtReset(ssHt *ssht) {
  htReset((ht *)ssht);
}

htEntry *ssHtNext(ssHt *ssht) {
  return (htEntry *)htNext((ht *)ssht);
}



  