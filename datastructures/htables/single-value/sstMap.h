/* 
  Specialized string - size_t map implementation:
    keys are strings, values are size_t
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef SSTMAP_H_INCLUDED
#define SSTMAP_H_INCLUDED

#include "map.h"

typedef struct sstMap sstMap;

typedef enum {
  CASE_SENSITIVE,
  CASE_INSENSITIVE
} sstMapCase;

sstMap *sstMapNew(sstMapCase c, size_t capacity);

void sstMapFree(sstMap *M);

void sstMapSetLabel(sstMap *M, char *label);

void sstMapCopyKeys(sstMap *M);

void sstMapOwnKeys(sstMap *M);

bool sstMapHasKeyVal(sstMap *M, char *key, size_t *val);

bool sstMapHasKey(sstMap *M, char *key);

void sstMapAddKey(sstMap *M, char *key, size_t val);

size_t sstMapGetVal(sstMap *M, char *key);

bool sstMapDelKey(sstMap *M, char *key);

void sstMapShow(sstMap *M);

void sstMapShowEntry(sstMap *M, char *key);

void sstMapStats(sstMap *M);

size_t sstMapSize(sstMap *M);

bool sstMapIsEmpty(sstMap *M);

mapEntry *sstMapFirst(sstMap *M);

mapEntry *sstMapNext(sstMap *M);

#endif  // SSTMAP_H_INCLUDED