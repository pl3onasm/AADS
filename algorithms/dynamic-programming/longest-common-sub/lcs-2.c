/* 
  file: lcs-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: longest common subsequence, top-down DP approach
  Time complexity: O(nm)
  Space complexity: O(nm)
*/ 

#include "../../../lib/clib/clib.h"
#include <stdint.h> 
#define MAX(a,b) ((a) > (b) ? (a) : (b))

//===================================================================
// Returns a table of size n x m initialized with SIZE_MAX
size_t **makeTable (size_t n, size_t m) {

  CREATE_MATRIX(size_t, table, n, m);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      table[i][j] = SIZE_MAX;
  
  return table;
}

//===================================================================
// Returns the length of the longest common subsequence of X and Y
// using a top-down dynamic programming approach
size_t computeLcs (size_t **table, string *X, string *Y, 
                   size_t x, size_t y) {
    
    // compute length of LCS if not available
  if (table[x][y] == SIZE_MAX) {
      
    if (x == 0 || y == 0) 
      // base case: reached the end of one of the strings
      table[x][y] = 0;

    else if (charAt(X, x - 1) == charAt(Y, y - 1))
      // last chars of X and Y are identical: add 1 
      table[x][y] = computeLcs(table, X, Y, x - 1, y - 1) + 1;

    else 
      // compute the maximum of the LCSs of the two alternatives:
      // either we remove the last char of X or the last char of Y
      table[x][y] = MAX(computeLcs(table, X, Y, x, y - 1),
                        computeLcs(table, X, Y, x - 1, y));
  }

  return table[x][y];
}

//===================================================================
// Recursively reconstructs a longest common subsequence
// from the memoization table and prints it
void reconstructLcs (size_t **table, string *X, size_t x, size_t y) {
  
  if (x == 0 || y == 0) 
    return;

  if (table[x][y] == table[x - 1][y]) 
    // if the value to the left is the same, we move to the left
    reconstructLcs(table, X, x - 1, y);

  else if (table[x][y] == table[x][y - 1]) 
    // if the value above is the same, we move up
    reconstructLcs(table, X, x, y - 1);

  else {
    // move diagonally up and left and print char after recursion
    reconstructLcs(table, X, x - 1, y - 1);
    printf("%c", charAt(X, x - 1));
  }
} 

//===================================================================

int main () {
  
  READ_STRING(X, '\n'); 
  READ_STRING(Y, '\n');

  size_t **table = makeTable(strLen(X) + 1, strLen(Y) + 1);

  size_t lcs = computeLcs(table, X, Y, strLen(X), strLen(Y));

  printf("Max length: %zu\nExample LCS:\n  ", lcs);
  reconstructLcs(table, X, strLen(X), strLen(Y));
  printf("\n");

  FREE_MATRIX(table, strLen(X) + 1);
  freeString(X);
  freeString(Y);
  
  return 0;
}