/* 
  Specialized string hash table: 
    both keys and values are strings.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef _SSHTABLE_H_
#define _SSHTABLE_H_

#include "htable.h"

typedef struct ssHt ssHt;

typedef enum {
  CASE_SENSITIVE,
  CASE_INSENSITIVE
} ssHtCase;

ssHt *ssHtNew(ssHtCase c);

void ssHtSetLabel(ssHt *ht, char *label);

void ssHtOwnKeys(ssHt *ht);

void ssHtOwnVals(ssHt *ht);

void ssHtCopyKeys(ssHt *ht);

void ssHtCopyVals(ssHt *ht);

void ssHtFree(ssHt *ht);

bool ssHtGetKeyVals(ssHt *ht, 
  char *key, dll **values);

dll *ssHtGetVals(ssHt *ht, char *value);

void ssHtAddKey(ssHt *ht, 
  char *key, char *value);

void ssHtDelKey(ssHt *ht, char *key);

void ssHtDelVal(ssHt *ht, 
  char *key, char *value);

void ssHtShow(ssHt *ht);

size_t ssHtSize(ssHt *ht);

bool ssHtIsEmpty(ssHt *ht);

void ssHtReset(ssHt *ht);

htEntry *ssHtNext(ssHt *ht);

#endif // _SSHTABLE_H_