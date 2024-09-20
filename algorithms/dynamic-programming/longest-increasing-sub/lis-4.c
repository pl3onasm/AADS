/* 
  file: lis-4.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: longest increasing subsequence
    solution in linearithmic time
*/ 

#include "../../../lib/clib.h"

//===================================================================
// Populates the memoization table and returns the length of the LIS
size_t computeLis (int *arr, size_t len, size_t *table, 
                   size_t *parents) {
  
  size_t subLen = 0;
  for (size_t i = 0; i < len; i++) {
      // use binary search to find the pile to extend
    size_t low = 1, high = subLen;
    while (low <= high) {
      size_t mid = low + (1 + high - low) / 2;
      if (arr[i] >= arr[table[mid]]) low = mid + 1;
      else high = mid - 1;
    }
      // update the table and parent array
    table[low] = i;
    parents[i] = table[low - 1];
      // create a new pile if needed
    if (low > subLen) subLen = low;
  }
  return subLen;
}

//===================================================================
// Reconstructs a LIS from the parent array and prints it
void reconstructLis (int *arr, size_t *parents, size_t p, 
                     size_t subLen) {
  
  CREATE_ARRAY(int, lis, subLen, 0);
  
  for (size_t i = subLen; i--; ) {
    lis[i] = arr[p]; 
    p = parents[p];
  }

  PRINT_ARRAY(lis, "%d", subLen);
  free(lis);
}

//===================================================================

int main () {

  READ(int, arr, "%d", len);

  CREATE_ARRAY(size_t, table, len + 1, 0);
  CREATE_ARRAY(size_t, prev, len, 0);

  size_t subLen = computeLis(arr, len, table, prev);

  printf("Max length: %zu\nSubsequence:\n ", subLen);
  reconstructLis(arr, prev, table[subLen], subLen);
  
  free(prev); free(table); free(arr);
  return 0;
}