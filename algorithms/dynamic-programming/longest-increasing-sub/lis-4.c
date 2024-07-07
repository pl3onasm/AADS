/* 
  file: lis-4.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: longest increasing subsequence
    solution in linearithmic time
*/ 

#include "../../../lib/clib/clib.h"

//===================================================================
// Populates the memoization table and returns the length of the LIS
size_t computeLis (int *arr, size_t len, size_t *table, 
                   size_t *parents) {
  
  size_t subLen = 1;
  for (size_t i = 1; i < len; i++) {
    if (arr[i] > arr[table[subLen - 1]]) {
        // no pile can be extended, create a new one and update
      parents[i] = table[subLen - 1]; 
      table[subLen++] = i;
    } else {
        // use binary search to find the pile to extend
      size_t low = 0, high = subLen - 1;
      while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (arr[table[mid]] <= arr[i]) 
          low = mid + 1;
        else high = mid;
      }
        // update the table and parent array
      table[low] = i;
      if (low > 0) 
        parents[i] = table[low - 1];
    }
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

  CREATE_ARRAY(size_t, table, len, 0);
  CREATE_ARRAY(size_t, prev, len, 0);

  size_t subLen = computeLis(arr, len, table, prev);

  printf("Max length: %zu\nSubsequence:\n ", subLen);
  reconstructLis(arr, prev, table[subLen - 1], subLen);
  
  free(prev); free(table); free(arr);
  return 0;
}