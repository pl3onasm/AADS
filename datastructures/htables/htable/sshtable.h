/* 
  Specialized string hash table: 
    both keys and values are strings.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef SSHTABLE_H_INCLUDED
#define SSHTABLE_H_INCLUDED

#include "htable.h"

typedef struct ssHt ssHt;

typedef enum {
  CASE_SENSITIVE,
  CASE_INSENSITIVE
} ssHtCase;

ssHt *ssHtNew(ssHtCase htCase, 
  size_t capacity);

void ssHtSetLabel(ssHt *ht, char *label);

void ssHtSetValDelim(ssHt *ht, 
  char *valDelim);

void ssHtOwnKeys(ssHt *ht);

void ssHtOwnVals(ssHt *ht);

void ssHtCopyKeys(ssHt *ht);

void ssHtCopyVals(ssHt *ht);

void ssHtFree(ssHt *ht);

bool ssHtHasKeyVals(ssHt *ht, 
  char *key, dll **values);

dll *ssHtGetVals(ssHt *ht, char *value);

void ssHtAddKey(ssHt *ht, char *key);

void ssHtAddKeyVal(ssHt *ht, char *key, 
  char *value);

// same as above, but with an array of values
void ssHtAddKeyVals(ssHt *ht, char *key, 
  char **values, size_t len);

bool ssHtHasKey(ssHt *H, char *key);

void ssHtDelKey(ssHt *ht, char *key);

void ssHtDelVal(ssHt *ht, 
  char *key, char *value);

void ssHtShow(ssHt *ht);

void ssHtShowEntry(ssHt *H, char *key);

void ssHtStats(ssHt *ht);

size_t ssHtSize(ssHt *ht);

bool ssHtIsEmpty(ssHt *ht);

size_t ssHtKeySize(ssHt *ht, char *key);

void ssHtReset(ssHt *ht);

htEntry *ssHtNext(ssHt *ht);

#endif // SSHTABLE_H_INCLUDED