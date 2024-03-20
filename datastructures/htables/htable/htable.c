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
  srand(time(NULL));
  H->seed = rand();
  H->seed ^= (uint)time(NULL);
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
    // rehash all entries in the bucket to new buckets
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

  // adds a new key-value pair to the hash table
static void htAddNewkey(ht *H, void *key, void *value, dll *bucket) {

  htEntry *entry = safeCalloc(1, sizeof(htEntry));
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
    // update the maximum length of a bucket
  if (dllSize(bucket) > H->maxLen)
    H->maxLen = dllSize(bucket);
    // one key more
  H->nKeys++;
}

  // tries to add a key-value pair to the hash table
void htAddKey(ht *H, void *key, void *value) {
    
  if (!H || !key) return;
    // rehash if necessary
  htRehash(H);

  size_t index = getIndex(H, key);
  
  if (H->buckets[index] == NULL) {
    H->buckets[index] = dllNew();
    // no ownership for the entries, since we want
    // to keep the entries when rehashing
    H->nFilled++;
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
      // update the maximum length of a bucket
      if (dllSize(bucket) > H->maxLen)
        H->maxLen = dllSize(bucket);
      return;
    }
  }

    // if the key does not exist,
    // add a new key-value pair
  htAddNewkey(H, key, value, bucket);
}

  // deletes a key from the hash table
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

    // if the end of the table is reached, reset the iterator
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
      // we are at the beginning of the list for this bucket
      // move to the first node
    H->iterNode = H->buckets[H->iterBucket]->NIL->next;
  }

    // return the current entry and move the iterator
    // to the next node
  htEntry *entry = H->iterNode->dllData;
  H->iterNode = H->iterNode->next;
  
  return entry; 
}

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
         "   Maximum bucket size: %zu\n\n\n",
         H->capacity, H->nFilled, H->nKeys,
         (double)H->nKeys / H->capacity, H->maxLen);
}

  // shows the hash table
void htShow(ht *H) {
  if (!H->showKey || !H->showValue) {
    fprintf(stderr, "htShow: showKey or " 
                    "showValue function not set\n");
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
