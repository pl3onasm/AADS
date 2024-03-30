/* 
  Some tests for the string-size_t map
  Author: David De Potter
*/

#include "sstMap.h" // use the string-size_t map

int main (){
  
  sstMap *map = sstNewMap(CASE_INSENSITIVE, 40);
  
    // set ownership of keys and values so all
    // allocated memory is freed when the map is freed
  sstMapOwnKeys(map);  
  sstMapOwnVals(map);

  sstMapSetLabel(map, "Test map");

    // add some key-value pairs
  for (size_t i = 0; i < 100; i++) {
    char *key = calloc(10, sizeof(char));
    size_t *val = calloc(1, sizeof(size_t));
    sprintf(key, "key%zu", i);
    *val = i;
    sstMapAddKeyVal(map, key, val);
  }

    // delete some keys
  for (size_t i = 0; i < 100; i += 5) {
    char key[10];
    sprintf(key, "key%zu", i);
    sstMapDelKey(map, key);
  }

    // check if the keys are still there
  for (size_t i = 0; i < 100; i += 5) {
    char key[10];
    sprintf(key, "key%zu", i);
    if (sstMapHasKey(map, key))
      printf("Key %s is still there\n", key);
    else
      printf("Key %s is gone\n", key);
  }

  sstMapShow(map);

  sstMapStats(map);

  sstMapFree(map);
  return 0;
}

