#include "clib.h"

// generic linear search, returns index of key in arr, -1 if not found
// arr: array to search
// len: length of arr
// size: size of each element in arr
// key: element to search for
// cmp: comparison function, returns 0 if elements are equal, 
//      < 0 if a < b, > 0 if a > b
size_t linSearch(void *arr, size_t len, size_t size, void *key, 
              int (*cmp)(const void *, const void *)){
  for (size_t i = 0; i < len; ++i){
    // cast to char* to allow pointer arithmetic
    void *elem = (char *)arr + i * size;
    if (cmp(elem, key) == 0) 
      return i;
  }
  return SIZE_MAX;
}

// generic binary search
// returns index of key in arr, SIZE_MAX if not found
// arr: array to search
// len: length of arr
// size: size of each element in arr
// key: element to search for
// cmp: comparison function, returns 0 if elements are equal,
//      < 0 if a < b, > 0 if a > b
size_t binSearch(void *arr, size_t len, size_t size, void *key, 
              int (*cmp)(const void *, const void *)){
  size_t low = 0, high = len - 1;
  while (low <= high){
    size_t mid = low + (high - low) / 2;
    int cmpVal = cmp((char *)arr + mid * size, key);
    if (cmpVal == 0) 
      return mid;
    if (cmpVal < 0) 
      low = mid + 1;
    else 
      high = mid - 1;
  }
  return SIZE_MAX;
}