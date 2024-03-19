/* 
  Generic hash table implementation.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include <time.h>
#include "htable.h"


ht *htNew(htHash hash, htCmpKey cmpKey, 
          htCmpValue cmpVal) {
  ht *H = safeCalloc(1, sizeof(ht));
  H->capacity = 32;
  H->buckets = safeCalloc(H->capacity, sizeof(dll*));
  H->hash = hash;
  H->cmpKey = cmpKey;
  H->cmpVal = cmpVal;
  H->seed = (uint)time(NULL);
  H->seed ^= (H->seed << 13);
  return H;
}

void htSetLabel(ht *H, char *label) {
  H->label = label;
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

  // sets the optional function for copying keys
void htCopyKeys(ht *H, htCopyKey copyKey, 
               htFreeKey freeKey) {
  H->copyKey = copyKey;
  H->freeKey = freeKey;
}

  // sets the optional function for copying values
void htCopyVals(ht *H, htCopyValue copyValue, 
                 htFreeValue freeValue) {
  H->copyValue = copyValue;
  H->freeValue = freeValue;
}

void htFree(ht *H) {
  for (size_t i = 0; i < H->capacity; i++) {
    if (H->buckets[i]) {
      dll *bucket = H->buckets[i];
      dllFirst(bucket);
      htEntry *entry;
      while ((entry = dllNext(bucket))) {
        if (H->freeKey)
          H->freeKey(entry->key);
        dllFree(entry->values);
        free(entry);
      }
      dllFree(bucket);
    }
  }
  free(H->buckets);
  free(H);
}

static size_t getIndex(ht *H, void *key) {
  return (H->hash(key, H->seed) % H->capacity);
}

  // sets the value pointer to the value associated with the key
  // set to NULL if the key has no value
  // returns true if the key exists
bool htGetKeyVals(ht *H, void *key, dll **values) {
  size_t index = getIndex(H, key);
  dll *bucket = H->buckets[index];
  *values = NULL;
  if (!bucket) 
    return false;
  htEntry *entry;
  dllFirst(bucket);
  while ((entry = dllNext(bucket))) {
    if (! H->cmpKey(key, entry->key)) {
      *values = entry->values;
      if (dllIsEmpty(*values))
        *values = NULL;
      return true;
    }
  }
  return false;
}

  // gets the value list associated with the key
dll *htGetVals(ht *H, void *key) {
  dll *values;
  htGetKeyVals(H, (void*)key, &values);
  return values;
}

  // rehashes the hash table if the number of 
  // keys exceeds 75% of the capacity
void htRehash(ht *H) {
  
  if (H->nKeys < 0.75 * H->capacity)
    return;
  
    // create new buckets
  size_t oldCapacity = H->capacity;
  H->capacity *= 2;
  dll **newBuckets = safeCalloc(H->capacity, sizeof(dll*));
  
    // rehash old entries
  for (size_t i = 0; i < oldCapacity; i++) {
    
    dll *bucket = H->buckets[i];
    if (! bucket)
      continue;
    
    htEntry *entry;
    dllFirst(bucket);
    // rehash all entries in the bucket to the new buckets
    while ((entry = dllNext(bucket))) {
      size_t newIndex = getIndex(H, entry->key);
      if (! newBuckets[newIndex]) 
        newBuckets[newIndex] = dllNew();
      dllPush(newBuckets[newIndex], entry);
    }
    // remove the old bucket without freeing the entries
    dllFree(bucket);
  }

    // free the old buckets
  free(H->buckets);

    // set new buckets
  H->buckets = newBuckets;
}

  // adds a key-value pair to the hash table
static void htPutKey(ht *H, void *key, void *value) {
  size_t index = getIndex(H, key);
  
  if (H->buckets[index] == NULL) {
    H->buckets[index] = dllNew();
    // no ownership, because of the rehash
  }
  
    // if the key exists, add the value to its value list
    // if the value is not yet in the list
  dll *bucket = H->buckets[index];
  dllFirst(bucket);
  htEntry *entry;
  while ((entry = dllNext(bucket))) {
    if (! H->cmpKey(key, entry->key)) {
      if (! dllFind(entry->values, value))
        dllPush(entry->values, value);
      return;
    }
  }

    // if the key does not exist,
    // add a new key-value pair
  entry = safeCalloc(1, sizeof(htEntry));
  entry->key = key;
    // copy the key if a copy function is provided
  if (H->copyKey)
    entry->key = H->copyKey(key);
    // create a new value list
  entry->values = dllNew();
    // set ownership
  if (H->copyValue)
    dllCopyData(entry->values, H->copyValue, H->freeValue);
  else if (H->freeValue)
    dllOwnData(entry->values, H->freeValue);
    // set show and comparison functions
  dllSetShow(entry->values, H->showValue);
  dllSetCmp(entry->values, H->cmpVal);

    // add the value to the new value list
  dllPush(entry->values, value);
    // add the new key-value pair to the bucket
  dllPush(bucket, entry);
  H->nKeys++;
}

void htAddKey(ht *H, void *key, void *value) {
  
  if (!H || !key) return;
    // check if a rehash is needed
  htRehash(H);
    // add the key-value pair
  htPutKey(H, key, value);
}

void htDelKey(ht *H, void *key) {
  size_t index = getIndex(H, key);
  dll *bucket = H->buckets[index];
  if (! bucket) 
    return;
  
  htEntry *entry;
  dllFirst(bucket);
  while ((entry = dllNext(bucket))) {
    if (! H->cmpKey(key, entry->key)) {
        // free the key if a free function is provided
      if (H->freeKey)
        H->freeKey(entry->key);    
        // free the list of values 
      dllFree(entry->values);
        // free the entry itself
      free(entry);
        // remove the entry from the bucket
      dllDelete(bucket, bucket->iter->prev);
        // one key less
      H->nKeys--;
      return;
    }
  }
}

  // deletes a value from the value list of a key
void htDelVal(ht *H, void *key, void *value) {
  dll *values;
  if (! htGetKeyVals(H, key, &values))
    return;
  dllDeleteData(values, value);
    // if the value list is empty, remove the key
  if (dllIsEmpty(values))
    htDelKey(H, key);
}


  // resets the iterator
void htReset(ht *H) {
  H->iterBucket = 0;
  H->iterNode = NULL;
}

  // returns the key-value pair of the current iterator position
  // and sets the iterator to the next key-value pair
  // returns NULL if the end of the hash table is reached
htEntry *htNext(ht *H) {
  htEntry *entry;
 
  if (H->iterBucket >= H->capacity) {
    // reset the iterator for the next iteration
    htReset(H);
    return NULL;
  }

  if (H->iterBucket < H->capacity && 
        (H->buckets[H->iterBucket] == NULL ||
        dllIsEmpty(H->buckets[H->iterBucket]))) {
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
      // we are at the beginning of the list for this bucket
      // move to the first node
    H->iterNode = H->buckets[H->iterBucket]->NIL->next;
  }

    // return the current entry and move the iterator
    // to the next node
  entry = (htEntry *)H->iterNode->dllData;
  H->iterNode = H->iterNode->next;
  
  return entry; 
}

void htShow(ht *H) {
  if (!H->showKey || !H->showValue) {
    fprintf(stderr, "htShow: showKey or showValue function not set\n");
    return;
  }
  htReset(H);
  htEntry *entry;
  
  if (H->label) 
    printf("----------------\n"
           "%s [%zu]\n"
           "----------------\n", H->label, H->nKeys);

  while ((entry = htNext(H))) {
    H->showKey(entry->key);
    printf("[%zu]: ", dllSize(entry->values));
    dllShow(entry->values);
  }

  printf("\n");
}


