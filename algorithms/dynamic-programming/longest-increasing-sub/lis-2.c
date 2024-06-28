/* 
  file: lis-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: longest increasing subsequence
    top-down DP implementation
  time complexity: O(n^2)
  note: for top-down DP it is necessary to initialize the entire 
    table with a value that is not a valid result so that we can  
    check whether the value has been computed. This is different 
    from bottom-up DP where the table only needs to be initialized
    with the base cases.
*/ 

#include "../../../lib/clib/clib.h"
#include <stdint.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))

//===================================================================
// Returns the length of the longest increasing subsequence
size_t computeLis (int *arr, size_t len, size_t i, 
                   size_t j, size_t **table) {
    
    // base case: check if first element can be included
  if (i == 0) table[i][j] = arr[i] < arr[j];

    // if value is not available, compute it,
    // otherwise return stored value
  if (table[i][j] == SIZE_MAX) {
    
    if (j == len || arr[i] < arr[j])
        // take maximum of including or excluding current element
      table[i][j] = MAX(1 + computeLis(arr, len, i - 1, i, table),
                        computeLis(arr, len, i - 1, j, table));
    else 
        // exclude current element
      table[i][j] = computeLis(arr, len, i - 1, j, table);
  }

  return table[i][j];
}

//===================================================================
// Reconstructs a LIS from the memoization table and prints it
void reconstructLis(int *arr, size_t len, size_t **table, 
                    size_t subLen) {

  CREATE_ARRAY(int, lis, subLen);

  for (size_t i = len - 1, j = len, s = subLen; i > 0; i--) {
    if (j == len || arr[i] < arr[j]) {
      if (table[i][j] == 1 + table[i - 1][i]) {
        lis[--s] = arr[i];
        j = i;
      }
    }
  }

  PRINT_ARRAY(lis, "%d", subLen);
  free(lis);
}

//===================================================================
// Creates and initializes a table of size rows x cols
size_t **makeTable (size_t rows, size_t cols) {
  
  CREATE_MATRIX(size_t, table, rows, cols);
  
  for (size_t i = 0; i < rows; i++)
    for (size_t j = 0; j < cols; j++)
      table[i][j] = SIZE_MAX;

  return table;
}

//===================================================================

int main () {

  READ(int, arr, "%d", len);

  size_t **table = makeTable(len, len + 1);

  size_t subLen = computeLis(arr, len, len - 1, len, table);

  printf("Max length: %zu\nSubsequence:\n ", subLen);
  
  reconstructLis(arr, len, table, subLen);

  FREE_MATRIX(table, len);
  free(arr);
  
  return 0;
}
