/* 
  Generic hash table implementation.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include <time.h>
#include "htable.h"
#include "../../../lib/clib/clib.h"

//=================================================================
// creates a new hash table
htable *htNew(htHash hash, htCmpKey cmpKey, 
          htCmpValue cmpVal, size_t capacity) {
  
  htable *H = safeCalloc(1, sizeof(htable));
  H->capacity = capacity < 32 ? 32 : capacity;
  H->buckets = safeCalloc(H->capacity, sizeof(dll*));
  H->hash = hash;
  H->cmpKey = cmpKey;
  H->cmpVal = cmpVal;
  H->label = "Hash table";
  srand(time(NULL));
  H->seed = rand();
  H->seed ^= (uint64_t)time(NULL) << 16;
  return H;
}

//=================================================================
// setters

void htSetLabel(htable *H, char *label) {
  H->label = label;
}

void htSetValDelim(htable *H, char *valDelim) {
  H->valDelim = valDelim;
}

void htSetShow(htable *H, htShowKey showKey, 
               htShowValue showValue) {
  H->showKey = showKey;
  H->showValue = showValue;
}

void htOwnKeys(htable *H, htFreeKey freeKey) {
  H->freeKey = freeKey;
}

void htOwnVals(htable *H, htFreeValue freeValue) {
  H->freeValue = freeValue;
}

void htCopyKeys(htable *H, htCopyKey copyKey, 
               htFreeKey freeKey) {
  H->copyKey = copyKey;
  H->freeKey = freeKey;
}

void htCopyVals(htable *H, htCopyValue copyValue, 
                 htFreeValue freeValue) {
  H->copyValue = copyValue;
  H->freeValue = freeValue;
}

//=================================================================
// gets number of values associated with a key
size_t htKeySize(htable *H, void *key) {
  dll *values;
  if (! htHasKeyVals(H, key, &values))
    return 0;
  return dllSize(values);
}

//=================================================================
// deallocates the hash table
void htFree(htable *H) {
  for (size_t i = 0; i < H->capacity; i++) {
    if (H->buckets[i]) {
      dll *bucket = H->buckets[i];
      for (htEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) {
        if (H->freeKey)
          H->freeKey(e->key);         
        dllFree(e->values);
        free(e);
      }
      dllFree(bucket);
    }
  }
  free(H->buckets);
  free(H);
}

//=================================================================
// returns the index of the bucket for a key
static size_t getIndex(htable *H, void *key) {
  return (H->hash(key, H->seed) % H->capacity);
}

//=================================================================
// returns true if the key exists
bool htHasKey(htable *H, void *key) {
  size_t index = getIndex(H, key);
  dll *bucket = H->buckets[index];
  if (! bucket || dllIsEmpty(bucket))
    return false;
  for (htEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) {
    if (! H->cmpKey(key, e->key))
      return true;
  }
  return false;
}

//=================================================================
// Returns the key from the table given an identifying key
void *htGetKey(htable *H, void *key) {
  size_t index = getIndex(H, key);
  dll *bucket = H->buckets[index];
  if (! bucket) 
    return NULL;
  for (htEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) {
    if (! H->cmpKey(key, e->key))
      return e->key;
  }
  return NULL;
}

//=================================================================
// Returns the value associated with the key
void *htGetVal(htable *H, void *key, void *value) {
  dll *values;
  if (! htHasKeyVals(H, key, &values))
    return NULL;
  return dllFind(values, value);
}

//=================================================================
// sets the value pointer to values associated 
// with the key; set to NULL if the key has no 
// values; returns true if the key exists
bool htHasKeyVals(htable *H, void *key, dll **values) {
  size_t index = getIndex(H, key);
  dll *bucket = H->buckets[index];
  *values = NULL;
  if (!bucket) 
    return false;
  for (htEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) {
    if (! H->cmpKey(key, e->key)) {
      *values = e->values;
      if (dllIsEmpty(*values))
        *values = NULL;
      return true;
    }
  }
  return false;
}

//=================================================================
// returns true if the value is associated with the key
bool htHasKeyVal(htable *H, void *key, void *value) {
  dll *values;
  if (! htHasKeyVals(H, key, &values))
    return false;
  if (dllFind(values, value))
    return true;
  return false;
}

//=================================================================
// gets the value list associated with the key
dll *htGetVals(htable *H, void *key) {
  dll *values;
  htHasKeyVals(H, (void*)key, &values);
  return values;
}

//=================================================================
// rehashes the hash table if the number of 
// keys exceeds 75% of the capacity
static void htRehash(htable *H) {
  
  if (H->nKeys < 0.75 * H->capacity)
    return;
  
    // create new buckets
  size_t oldCapacity = H->capacity;
  H->capacity *= 2;
  dll **newBuckets = safeCalloc(H->capacity, sizeof(dll*));
  H->nFilled = 0;
  
    // rehash old entries
  for (size_t i = 0; i < oldCapacity; i++) {
    
    dll *bucket = H->buckets[i];
    if (! bucket)
      continue;
    
    // rehash all entries in the bucket to new buckets
    for (htEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) {
      size_t newIndex = getIndex(H, e->key);
      if (! newBuckets[newIndex]) {
        newBuckets[newIndex] = dllNew();
        H->nFilled++;
      } 
      dllPush(newBuckets[newIndex], e);
    }
    // remove the old bucket without freeing the entries
    dllFree(bucket);
  }

    // free the old buckets and set the new ones
  free(H->buckets);
  H->buckets = newBuckets;
}

//=================================================================
// adds a new key-value pair to the hash table
static void htAddNewkeyVal(htable *H, void *key, void *value, 
                           dll *bucket) {

  htEntry *entry = safeCalloc(1, sizeof(htEntry));

    // copy the key if a copy function is provided
  entry->key = H->copyKey ? H->copyKey(key) : key;
    // create a new value list
  entry->values = dllNew();
    // set ownership of the value list
  if (H->copyValue)
    dllCopyData(entry->values, H->copyValue, H->freeValue);
  else if (H->freeValue)
    dllOwnData(entry->values, H->freeValue);
    // set different functions for the value list
  dllSetShow(entry->values, H->showValue);
  dllSetCmp(entry->values, H->cmpVal);
  if (H->valDelim)
    dllSetDelim(entry->values, H->valDelim);

    // add the value to the new value list if not NULL
  if (value)
    dllPush(entry->values, value);
    // add the new key-value pair to the bucket
  dllPush(bucket, entry);
    // one key more
  H->nKeys++;
}

//=================================================================
// tries to add a key-value pair to the table
void htAddKeyVal(htable *H, void *key, void *value) {
    
    // rehash if necessary
  htRehash(H);

  size_t index = getIndex(H, key);
  
  if (! H->buckets[index]) {
    H->buckets[index] = dllNew();
    // no ownership for the entries, since we want
    // to keep the entries when rehashing, but still
    // want to free the bucket after rehashing
    H->nFilled++;
  }
  
    // if the key exists, add the value to its value 
    // list if the value is not yet in the list
  dll *bucket = H->buckets[index];
  for (htEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) {
    if (! H->cmpKey(key, e->key)) {
      if (! dllFind(e->values, value))
        dllPush(e->values, value);
      return;
    }
  }

    // if the key does not exist,
    // add a new key-value pair
  htAddNewkeyVal(H, key, value, bucket);
}

//=================================================================
// adds new key to the table without a value
void htAddKey(htable *H, void *key) {
  if (!H || !key) return;

  size_t index = getIndex(H, key);

  if (! H->buckets[index]) {
    H->buckets[index] = dllNew();
    // no ownership for the entries, since we 
    // want to keep the entries when rehashing
    H->nFilled++;
  }
  
    // if the key exists, do nothing
  dll *bucket = H->buckets[index];
  for (htEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) {
    if (! H->cmpKey(key, e->key))
      return;
  }

    // if the key does not exist,
    // add a new key-value pair
  htAddNewkeyVal(H, key, NULL, bucket);
}

//=================================================================
// deletes a key from the hash table
// returns true if the key was removed, false if not found
bool htDelKey(htable *H, void *key) {
  size_t index = getIndex(H, key);
  dll *bucket = H->buckets[index];
  if (! bucket) 
    return false;
  
  for (htEntry *e = dllFirst(bucket); e; e = dllNext(bucket)) {
    if (! H->cmpKey(key, e->key)) {
        // free key if a free function is provided
      if (H->freeKey)
        H->freeKey(e->key);    
        // free the list of values 
      dllFree(e->values);
        // free the entry itself
      free(e);
        // remove the entry from the bucket
      dllDelete(bucket, bucket->iter);
        // one key less
      H->nKeys--;
        // update statistics
      if (dllIsEmpty(bucket))
        H->nFilled--;
      return true;
    }
  }
  return false;
}
  
//=================================================================
// deletes a value from the value list of a key
// returns true if the value was removed, false if not found
bool htDelVal(htable *H, void *key, void *value) {
  dll *values = NULL;
  if (! htHasKeyVals(H, key, &values) || !values)
    return false;
  return dllDeleteData(values, value);
}

//=================================================================
// returns the first key-value pair in the table and sets the
// iterator to the next key-value pair; 
// returns NULL if the table is empty
htEntry *htFirst(htable *H) {
  H->iterBucket = 0;
  H->iterNode = NULL;
  return htNext(H);
}

//=================================================================
// returns the key-value pair of the current 
// iterator position and sets the iterator to  
// the next key-value pair
// returns NULL if end of the table is reached
htEntry *htNext(htable *H) {

    // end of the table reached
  if (H->iterBucket >= H->capacity) 
    return NULL;

    // skip empty buckets
  if (H->buckets[H->iterBucket] == NULL ||
      dllIsEmpty(H->buckets[H->iterBucket])) {
    H->iterBucket++;
    H->iterNode = NULL;
    return htNext(H);
  }
  
  if (H->iterNode == H->buckets[H->iterBucket]->NIL) {
      // we are at the end of the list for this bucket
      // move to the next bucket
    H->iterBucket++;
    H->iterNode = NULL;
    return htNext(H);
  }

  if (H->iterNode == NULL) {
      // we are at the beginning of the list 
      // for this bucket; move to the first node
    H->iterNode = H->buckets[H->iterBucket]->NIL->next;
  }

    // return the current entry and move 
    // the iterator to the next node
  htEntry *entry = H->iterNode->dllData;
  H->iterNode = H->iterNode->next;
  
  return entry; 
}

//=================================================================
// Computes the maximum bucket size
size_t htMaxBucketSize(htable *H) {
  size_t max = 0;
  for (size_t i = 0; i < H->capacity; i++) 
    if (H->buckets[i] && dllSize(H->buckets[i]) > max)
      max = dllSize(H->buckets[i]);
  return max;
}

//=================================================================
// Gives an overview of the distribution of keys 
// over the buckets
void htStats(htable *H) {
  printf("\n+---------------------------+\n"
         "|   Hash table statistics   |\n"
         "+---------------------------+\n\n"
         "   Number of buckets..: %zu\n"
         "   Buckets used.......: %zu\n"
         "   Number of keys.....: %zu\n"
         "   Load factor........: %.2f\n"
         "   Max. bucket size...: %zu\n"
         "   Collisions.........: %zu\n\n\n",
         H->capacity, H->nFilled, H->nKeys,
         (double)H->nKeys / H->capacity, 
         htMaxBucketSize(H),
         H->nKeys - H->nFilled);
}

//=================================================================
// shows the hash table
void htShow(htable *H) {
  if (!H->showKey || !H->showValue) {
    fprintf(stderr, "htShow: showKey or " 
                    "showValue function not set\n");
    return;
  }
  
  printf("\n--------------------\n"
          "  %s [%zu]\n"
          "--------------------\n", 
          H->label, H->nKeys);

  for (htEntry *e = htFirst(H); e; e = htNext(H)) 
    htShowEntry(H, e->key);
  printf("--------------------\n\n");
}

//=================================================================
// shows a key-value pair
void htShowEntry(htable *H, void *key) {
  if (!H->showKey || !H->showValue) {
    fprintf(stderr, "htShowEntry: showKey or " 
                    "showValue function not set\n");
    return;
  }
  dll *values;
  if (! htHasKeyVals(H, key, &values)) {
    printf("Key not found\n");
    return;
  }
  H->showKey(key);
  printf("[%zu]", dllSize(values));
  printf(dllIsEmpty(values) ? "\n" : ": ");
  dllShow(values);
}

