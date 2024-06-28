/* 
  file: lis-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: longest increasing subsequence
    bottom-up DP approach
*/ 

#include "../../../lib/clib/clib.h"

//===================================================================
// Populates the memoization table and returns the length of the LIS
size_t computeLis(int *arr, size_t n, size_t *table) {

  size_t max = 0;
  for (int i = 1; i < n; i++) 
    for (int j = 0; j < i; j++) 
      if (arr[j] < arr[i] && table[j] >= table[i]) {
        table[i] = table[j] + 1;
        if (table[i] > max) 
          max = table[i];
      }
  
  return max + 1;
}

//===================================================================
// Reconstructs a LIS from the memoization table and prints it
void reconstructLis(int *arr, size_t len, size_t *table, 
                    size_t subLen) {
  
  CREATE_ARRAY(int, lis, subLen);

  for (size_t i = len, s = subLen; i--; ) 
    if (table[i] == s - 1) 
      lis[--s] = arr[i];

  PRINT_ARRAY(lis, "%d", subLen);
  free(lis);
}

//===================================================================

int main () {

  READ(int, arr, "%d", len);

  CREATE_ARRAY(size_t, table, len);

  size_t subLen = computeLis(arr, len, table);

  printf("Max length: %zu\nSubsequence:\n ", subLen);
  reconstructLis(arr, len, table, subLen);

  free(table);
  free(arr);
  
  return 0;
}