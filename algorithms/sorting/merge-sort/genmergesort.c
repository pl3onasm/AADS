/* 
  file: genmergesort.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: generic merge sort,  
    can be used to sort any type of data, 
    as long as a comparison function is provided
  time complexity: O(nlogn) 
*/

#include "../../../lib/clib/clib.h"

//===================================================================
// Compares two integers
int compInts (void const *a, void const *b) {
  return (*(int*)a - *(int*)b);
}

//===================================================================
// Compares two strings
int compStrs (void const *a, void const *b) {
  return strcmp(*(char**)a, *(char**)b);
}

//===================================================================
// Merges two sorted subarrays into one sorted array
void merge(void *arr, size_t left, size_t mid, size_t right, 
           size_t elSize, int (*comp)(void const *, void const *)) {
    // allocate memory for an auxiliary array to
    // store the combined sorted array
  void *sorted = safeMalloc((right - left) * elSize);
  size_t l = left, r = mid, s = 0;   
    // merge the two subarrays into the auxiliary array
    // in sorted order until one of the subarrays is exhausted
  while (l < mid && r < right) {
    if (comp((char*)arr + l * elSize, (char*)arr + r * elSize) < 0)
      memcpy((char*)sorted + (s++) * elSize, 
             (char*)arr + (l++) * elSize, elSize);
    else 
      memcpy((char*)sorted + (s++) * elSize, 
             (char*)arr + (r++) * elSize, elSize);
  }
    // copy the remaining elements into the end 
    // of the auxiliary array
  memcpy((char*)sorted + s * elSize, (char*)arr + l * elSize, 
         (mid - l) * elSize);
  memcpy((char*)sorted + s * elSize, (char*)arr + r * elSize, 
         (right - r) * elSize);
    // copy the auxiliary array back to the input array
  memcpy((char*)arr + left * elSize, sorted, 
         (right - left) * elSize);
  free(sorted);
}

//===================================================================
// Sorts an array of any type in ascending order in O(nlogn) time
void mergeSort(void *arr, size_t left, size_t right, size_t elSize, 
               int (*comp)(void const *, void const *)) { 

  if (left + 1 < right) {
      // divide the array in two subarrays
      // around the midpoint
    size_t mid = left + (right - left)/2;
      // conquer the subarrays
    mergeSort(arr, left, mid, elSize, comp);
    mergeSort(arr, mid, right, elSize, comp);
      // combine the sorted halves
    merge(arr, left, mid, right, elSize, comp);
  }
}

//===================================================================

int main (){

  int ints[] = {10, 8, -9, 6, 7, 5, 2, 3, 4, -1, 2,
                13, 7, 11, 20, 1, -15, 7, 16, 0};

  char *strs[] = {"Harry", "Stephen", "Samuel", "James", 
                  "Chris", "Robert", "Michael", "William", 
                  "David", "Richard", "Charles", "Joseph", 
                  "Thomas", "John", "Daniel", "Matthew", 
                  "Anthony", "Jonathan", "Mark", "Paul"};
  
  mergeSort(ints, 0, 20, sizeof(int), compInts);
  mergeSort(strs, 0, 20, sizeof(char*), compStrs);
  
  PRINT_ARRAY(ints, "%d", 20);
  PRINT_ARRAY(strs, "%s", 20);
  return 0;
}