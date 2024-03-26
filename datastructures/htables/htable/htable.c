/* 
  Generic hash table implementation.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include <time.h>
#include "htable.h"

//=================================================================
// creates a new hash table
ht *htNew(htHash hash, htCmpKey cmpKey, 
          htCmpValue cmpVal, size_t capacity) {
  
  ht *H = safeCalloc(1, sizeof(ht));
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

void htSetLabel(ht *H, char *label) {
  H->label = label;
}

void htSetValDelim(ht *H, char *valDelim) {
  H->valDelim = valDelim;
}

void htSetShow(ht *H, htShowKey showKey, 
               htShowValue showValue) {
  H->showKey = showKey;
  H->showValue = showValue;
}

void htOwnKeys(ht *H, htFreeKey freeKey) {
  H->freeKey = freeKey;
}

void htOwnVals(ht *H, htFreeValue freeValue) {
  H->freeValue = freeValue;
}

void htCopyKeys(ht *H, htCopyKey copyKey, 
               htFreeKey freeKey) {
  H->copyKey = copyKey;
  H->freeKey = freeKey;
}

void htCopyVals(ht *H, htCopyValue copyValue, 
                 htFreeValue freeValue) {
  H->copyValue = copyValue;
  H->freeValue = freeValue;
}

//=================================================================
// gets number of values associated with a key
size_t htKeySize(ht *H, void *key) {
  dll *values;
  if (! htHasKeyVals(H, key, &values))
    return 0;
  return dllSize(values);
}

//=================================================================
// deallocates the hash table
void htFree(ht *H) {
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
static size_t getIndex(ht *H, void *key) {
  return (H->hash(key, H->seed) % H->capacity);
}

//=================================================================
// returns true if the key exists
bool htHasKey(ht *H, void *key) {
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
void *htGetKey(ht *H, void *key) {
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
void *htGetVal(ht *H, void *key, void *value) {
  dll *values;
  if (! htHasKeyVals(H, key, &values))
    return NULL;
  return dllFind(values, value);
}

//=================================================================
// sets the value pointer to values associated 
// with the key; set to NULL if the key has no 
// values; returns true if the key exists
bool htHasKeyVals(ht *H, void *key, dll **values) {
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
bool htHasKeyVal(ht *H, void *key, void *value) {
  dll *values;
  if (! htHasKeyVals(H, key, &values))
    return false;
  if (dllFind(values, value))
    return true;
  return false;
}

//=================================================================
// gets the value list associated with the key
dll *htGetVals(ht *H, void *key) {
  dll *values;
  htHasKeyVals(H, (void*)key, &values);
  return values;
}

//=================================================================
// rehashes the hash table if the number of 
// keys exceeds 75% of the capacity
static void htRehash(ht *H) {
  
  if (H->nKeys < 0.75 * H->capacity)
    return;
  
    // create new buckets
  size_t oldCapacity = H->capacity;
  H->capacity *= 2;
  dll **newBuckets = safeCalloc(H->capacity, sizeof(dll*));
  H->nFilled = H->nCollisions = H->maxLen = 0;
  
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
      } else H->nCollisions++;
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
static void htAddNewkeyVal(ht *H, void *key, void *value, 
                           dll *bucket) {

  if (! dllIsEmpty(bucket)) 
    H->nCollisions++;
  htEntry *entry = safeCalloc(1, sizeof(htEntry));
  entry->key = key;

    // copy the key if a copy function is provided
  if (H->copyKey)
    entry->key = H->copyKey(key);
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
    // update the maximum length of a bucket
  if (dllSize(bucket) > H->maxLen)
    H->maxLen = dllSize(bucket);
    // one key more
  H->nKeys++;
}

//=================================================================
// tries to add a key-value pair to the table
void htAddKeyVal(ht *H, void *key, void *value) {
    
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
      // update the maximum length of a bucket
      if (dllSize(bucket) > H->maxLen)
        H->maxLen = dllSize(bucket);
      return;
    }
  }

    // if the key does not exist,
    // add a new key-value pair
  htAddNewkeyVal(H, key, value, bucket);
}

//=================================================================
// adds new key to the table without a value
void htAddKey(ht *H, void *key) {
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
bool htDelKey(ht *H, void *key) {
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
      else
        H->nCollisions--;
      return true;
    }
  }
  return false;
}
  
//=================================================================
// deletes a value from the value list of a key
// returns true if the value was removed, false if not found
bool htDelVal(ht *H, void *key, void *value) {
  dll *values = NULL;
  if (! htHasKeyVals(H, key, &values) || !values)
    return false;
  return dllDeleteData(values, value);
}

//=================================================================
// resets the iterator
void htReset(ht *H) {
  H->iterBucket = 0;
  H->iterNode = NULL;
}

//=================================================================
// returns the key-value pair of the current 
// iterator position and sets the iterator to  
// the next key-value pair
// returns NULL if end of the table is reached
htEntry *htNext(ht *H) {

    // if the end of the table is reached, 
    // reset the iterator
  if (H->iterBucket >= H->capacity) {
    htReset(H);
    return NULL;
  }

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
// Gives an overview of the distribution of keys 
// over the buckets
void htStats(ht *H) {
  printf("\n+---------------------------+\n"
         "|   Hash table statistics   |\n"
         "+---------------------------+\n\n"
         "   Number of buckets..: %zu\n"
         "   Buckets used.......: %zu\n"
         "   Number of keys.....: %zu\n"
         "   Load factor........: %.2f\n"
         "   Maximum bucket size: %zu\n"
         "   Collisions.........: %zu\n\n\n",
         H->capacity, H->nFilled, H->nKeys,
         (double)H->nKeys / H->capacity, 
         H->maxLen, H->nCollisions);
}

//=================================================================
// shows the hash table
void htShow(ht *H) {
  if (!H->showKey || !H->showValue) {
    fprintf(stderr, "htShow: showKey or " 
                    "showValue function not set\n");
    return;
  }
  htReset(H);
  htEntry *entry;
  
  
  printf("\n--------------------\n"
          "  %s [%zu]\n"
          "--------------------\n\n", 
          H->label, H->nKeys);

  while ((entry = htNext(H))) {
    H->showKey(entry->key);
    printf("[%zu]", dllSize(entry->values));
    printf(dllIsEmpty(entry->values) ? "\n" : ": ");
    dllShow(entry->values);
  }
  printf("\n");
}

//=================================================================
// shows a key-value pair
void htShowEntry(ht *H, void *key) {
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

