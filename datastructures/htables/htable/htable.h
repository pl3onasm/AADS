/* 
  Generic hash table, using separate chaining
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef _HTABLE_H_INCLUDED_
#define _HTABLE_H_INCLUDED_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../lists/dllist/dll.h"
#include "../../../lib/clib/clib.h"

  // function pointer types
typedef uint (*htHash)(void *hashKey, uint seed);
typedef int (*htCmpKey)(void *key1, void *key2);
typedef int (*htCmpValue)(void *value1, void *value2);
typedef void (*htShowKey)(void *key);
typedef void (*htShowValue)(void *value);
typedef void (*htFreeKey)(void *key);
typedef void (*htFreeValue)(void *val);
typedef void *(*htCopyKey)(void *key);
typedef void *(*htCopyValue)(void *val);

  // hash table structure
typedef struct {
  size_t capacity;        // number of available buckets
  size_t nKeys;           // number of keys
  dll **buckets;          // array of doubly linked lists
  htHash hash;            // hash function
  htCmpValue cmpVal;      // comparison function for the values
  htCmpKey cmpKey;        // comparison function for the keys
  uint seed;              // random seed for the hash table
  size_t iterBucket;      // current bucket for the iterator
  dllNode *iterNode;      // current node for the iterator
  htShowKey showKey;      // function to show the key
  htShowValue showValue;  // function to show the value
  htFreeKey freeKey;      // function to free the key
  htFreeValue freeValue;  // function to free the value
  htCopyKey copyKey;      // function to copy the key
  htCopyValue copyValue;  // function to copy the value
  size_t nCollisions;     // number of collisions
  size_t nFilled;         // number of filled buckets
  size_t maxLen;          // maximum length of a bucket
  char *label;            // label for the hash table
  char *valDelim;         // delimiter for the values
} ht;

typedef struct {          // key-value pair
  void *key;              // key
  dll *values;            // list of values
} htEntry;

  // hash table function prototypes

  // creates a new hash table
ht *htNew(htHash hash, htCmpKey cmpKey, 
          htCmpValue cmpVal, size_t capacity);

  // sets the label for the hash table
void htSetLabel(ht *H, char *label);

  // sets the string delimiter for the values
  // default is ", "
void htSetValDelim(ht *H, char *delim);

  // sets the show functions for the hash table
void htSetShow(ht *H, htShowKey showKey, 
               htShowValue showValue);

  // sets the table to make copies of the keys
  // if set, the table will only free the copies
void htCopyKeys(ht *H, htCopyKey copyKey, 
                htFreeKey freeKey);

  // sets the table to make copies of the values
  // if set, the table will only free the copies
void htCopyVals(ht *H, htCopyValue copyValue, 
                htFreeValue freeValue);

  // sets the table to own the input keys,
  // freeing them when the table is freed
void htOwnKeys(ht *H, htFreeKey freeKey);

  // sets the table to own the input values,
  // freeing them when the table is freed
void htOwnVals(ht *H, htFreeValue freeValue);

  // frees the hash table
void htFree(ht *H);

  // sets the value pointer to the value list associated 
  // with the key, set to NULL if the key has no values
  // returns true if the key exists
bool htHasKeyVals(ht *H, void *key, dll **values);

  // returns true if the key-value pair exists
bool htHasKeyVal(ht *H, void *key, void *value);

  // returns the value list associated with the key
  // returns NULL if the value or key is not found
dll *htGetVals(ht *H, void *key);

  // adds a key to the hash table
void htAddKey(ht *H, void *key);

  // adds a key-value pair to the hash table
  // if the key is already in the hash table, the 
  // new value is added to the list
  // if the key is not in the hash table, 
  // a new key-value pair is added
void htAddKeyVal(ht *H, void *key, void *value);

  // returns true if the key exists in the hash table
bool htHasKey(ht *H, void *key);

  // returns the key given a hash key
void *htGetKey(ht *H, void *hashKey);

  // removes the key and its value list 
void htDelKey(ht *H, void *key);

  // removes a value from the key's value list
void htDelVal(ht *H, void *key, void *value);

  // shows the hash table
void htShow(ht *H);

  // shows a key and its values
void htShowEntry(ht *H, void *key);

  // shows distribution statistics
void htStats(ht *H);

  // returns the number of keys in the hash table
inline size_t htSize(ht *H) {
  return H->nKeys;
}

  // returns true if the hash table is empty
inline bool htIsEmpty(ht *H) {
  return H->nKeys == 0;
}

  // returns the number of values associated with a key
size_t htKeySize(ht *H, void *key);

  // resets the iterator
void htReset(ht *H);

  // returns the key-value pair of the current iterator position
  // and sets the iterator to the next key-value pair
  // returns NULL if the end of the hash table is reached
htEntry *htNext(ht *H);

#endif // _HTABLE_H_INCLUDED_