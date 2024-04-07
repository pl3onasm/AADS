/* 
  Generic hash table implementation, using separate chaining
  Keys can have multiple values
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef HTABLE_H_INCLUDED
#define HTABLE_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>   // uint64_t
#include "../../lists/dllist/dll.h"

  // function pointer types
typedef uint64_t (*htHash)(void *hashKey, uint64_t seed);
typedef int (*htCmpKey)(void const *key1, void const *key2);
typedef int (*htCmpValue)(void const *value1, void const *value2);
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
  uint64_t seed;          // magic seed for the hash table
  size_t iterBucket;      // current bucket for the iterator
  dllNode *iterNode;      // current node for the iterator
  htShowKey showKey;      // function to show the key
  htShowValue showValue;  // function to show the value
  htFreeKey freeKey;      // function to free the key
  htFreeValue freeValue;  // function to free the value
  htCopyKey copyKey;      // function to copy the key
  htCopyValue copyValue;  // function to copy the value
  size_t nFilled;         // number of filled buckets
  char *label;            // label for the hash table
  char *valDelim;         // delimiter for the values
                          // default is ", "
} htable;

typedef struct {          // key-value pair
  void *key;              // key
  dll *values;            // list of values
} htEntry;

  // hash table function prototypes

  // creates a new hash table
htable *htNew(htHash hash, htCmpKey cmpKey, 
          htCmpValue cmpVal, size_t capacity);

  // sets the label for the hash table
  // default is "hash table"
void htSetLabel(htable *H, char *label);

  // sets the string delimiter for the values
  // default is ", "
void htSetValDelim(htable *H, char *delim);

  // sets the show functions for the hash table
void htSetShow(htable *H, htShowKey showKey, 
               htShowValue showValue);

  // sets the table to make copies of the keys
  // if set, the table will only free the copies
void htCopyKeys(htable *H, htCopyKey copyKey, 
                htFreeKey freeKey);

  // sets the table to make copies of the values
  // if set, the table will only free the copies
void htCopyVals(htable *H, htCopyValue copyValue, 
                htFreeValue freeValue);

  // sets the table to own the input keys,
  // freeing them when the table is freed
void htOwnKeys(htable *H, htFreeKey freeKey);

  // sets the table to own the input values,
  // freeing them when the table is freed
void htOwnVals(htable *H, htFreeValue freeValue);

  // frees the hash table
void htFree(htable *H);

  // sets the value pointer to the value list associated 
  // with the key, set to NULL if the key has no values
  // returns true if the key exists
bool htHasKeyVals(htable *H, void *key, dll **values);

  // returns true if the key-value pair exists
bool htHasKeyVal(htable *H, void *key, void *value);

  // returns a value associated with the key
  // returns NULL if the value or key is not found
void *htGetVal(htable *H, void *key, void *value);

  // returns the value list associated with the key
  // returns NULL if the value or key is not found
dll *htGetVals(htable *H, void *key);

  // adds a key to the hash table
  // if the key exists, nothing happens
void htAddKey(htable *H, void *key);

  // adds a key-value pair to the hash table
  // if the key exists, the value is added to the its
  // value list, if the value is not already in there
  // if the key is not in the hash table, 
  // a new key-value pair is added
void htAddKeyVal(htable *H, void *key, void *value);

  // returns true if the key exists in the hash table
bool htHasKey(htable *H, void *key);

  // returns the key if it exists in the hash table
  // returns NULL if the key is not found
void *htGetKey(htable *H, void *key);

  // removes the key and its value list 
  // true if the key was removed
  // false if the key was not found
bool htDelKey(htable *H, void *key);

  // removes a value from the key's value list
  // true if the value was removed
  // false if the key or value was not found
bool htDelVal(htable *H, void *key, void *value);

  // shows the hash table
void htShow(htable *H);

  // shows a key and its values
void htShowEntry(htable *H, void *key);

  // shows distribution statistics
void htStats(htable *H);

  // returns the number of keys in the hash table
static inline size_t htSize(htable *H) {
  return H->nKeys;
}

  // returns true if the hash table is empty
static inline bool htIsEmpty(htable *H) {
  return H->nKeys == 0;
}

  // returns the number of values associated with a key
size_t htKeySize(htable *H, void *key);

  // returns the first key-value pair in the hash table
  // and updates the iterator to the next pair
  // returns NULL if the hash table is empty
htEntry *htFirst(htable *H);

  // returns the key-value pair of the current iterator position
  // and sets the iterator to the next key-value pair
  // returns NULL if the end of the hash table is reached
htEntry *htNext(htable *H);

#endif  // HTABLE_H_INCLUDED