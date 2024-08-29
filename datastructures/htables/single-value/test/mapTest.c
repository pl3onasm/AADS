/* 
  Some tests for the string-size_t map
  Author: David De Potter
*/

#include "../sstMap.h" // use the string-size_t map

int main (){
  
  sstMap *map = sstMapNew(CASE_INSENSITIVE, 40);
  
    // set ownership of keys so all extra
    // allocated memory is freed when the map is freed
  sstMapOwnKeys(map);  

  sstMapSetLabel(map, "Test map");

    // add some key-value pairs
  for (size_t i = 0; i < 100; i++) {
    char *key = calloc(10, sizeof(char));
    sprintf(key, "key%zu", i);
    sstMapAddKey(map, key, i);
  }
  printf("\nAdded 100 keys to the map\n");
  sstMapShow(map);

      // update some values
  for (size_t i = 1; i < 100; i += 20) {
    char key[10];
    sprintf(key, "key%zu", i);
    printf("Updating value of %s to %zu\n", key, i * 1000);
    sstMapAddKey(map, key, i * 1000);
  }

    // delete some keys
  for (size_t i = 0; i < 100; i += 10) {
    char key[10];
    sprintf(key, "key%zu", i);
    printf("\nDeleting %s", key);
    sstMapDelKey(map, key);
  }

    // check some keys to see if they are still there
  printf("\n\nChecking if keys are still there...\n\n");
  for (size_t i = 0; i < 100; i += 5) {
    char key[10];
    sprintf(key, "key%zu", i);
    printf("%s is %s", key,
      sstMapHasKey(map, key) ? "present\n" : "gone\n");
  }

    // get the values of some keys
  printf("\nGetting values of some keys...\n");
  for (size_t i = 0; i < 100; i += 13) {
    char key[10];
    sprintf(key, "key%zu", i);
    printf("\nValue of key %s is %zu", key, sstMapGetVal(map, key));
  }

  printf("\n\n");
  sstMapShow(map);

  sstMapStats(map);

    // make a second map
  sstMap *map2 = sstMapNew(CASE_INSENSITIVE, 40);
  sstMapSetLabel(map2, "Second map");

    // add some key-value pairs
  for (size_t i = 100; i < 150; i++) {
    char *key = calloc(10, sizeof(char));
    sprintf(key, "key%zu", i);
    sstMapAddKey(map2, key, i);
  }

  printf("\n\nSecond map\n");
  sstMapShow(map2);

    // merge the two maps
  printf("\n\n... merging maps\n");
  sstMapMerge(map, map2);
  
  sstMapSetLabel(map, "Merged map");
  sstMapShow(map);

  sstMapStats(map);

  sstMapFree(map);
  return 0;
}

