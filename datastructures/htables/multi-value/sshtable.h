/* 
  Specialized string hash table: 
    both keys and values are strings.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef SSHTABLE_H_INCLUDED
#define SSHTABLE_H_INCLUDED

#include "htable.h"

typedef struct sshtable sshtable;

typedef enum {
  CASE_SENSITIVE,
  CASE_INSENSITIVE
} sshtCase;

sshtable *sshtNew(sshtCase htCase, 
  size_t capacity);

void sshtSetLabel(sshtable *ht, char *label);

void sshtSetValDelim(sshtable *ht, 
  char *valDelim);

void sshtOwnKeys(sshtable *ht);

void sshtOwnVals(sshtable *ht);

void sshtCopyKeys(sshtable *ht);

void sshtCopyVals(sshtable *ht);

void sshtFree(sshtable *ht);

bool sshtHasKeyVals(sshtable *ht, 
  char *key, dll **values);

dll *sshtGetVals(sshtable *ht, char *value);

void sshtAddKey(sshtable *ht, char *key);

void sshtAddKeyVal(sshtable *ht, char *key, 
  char *value);

// same as above, but with an array of values
void sshtAddKeyVals(sshtable *ht, char *key, 
  char **values, size_t len);

bool sshtHasKey(sshtable *H, char *key);

void sshtDelKey(sshtable *ht, char *key);

void sshtDelVal(sshtable *ht, 
  char *key, char *value);

void sshtShowEntry(sshtable *H, char *key);

void sshtStats(sshtable *ht);

void sshtShow(sshtable *ht);

size_t sshtSize(sshtable *ht);

void sshtMerge(sshtable *ht1, sshtable *ht2);

bool sshtIsEmpty(sshtable *ht);

size_t sshtKeySize(sshtable *ht, char *key);

htEntry *sshtFirst(sshtable *ht);

htEntry *sshtNext(sshtable *ht);

#endif // SSHTABLE_H_INCLUDED