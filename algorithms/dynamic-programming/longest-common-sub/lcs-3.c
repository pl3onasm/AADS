/* 
  file: lcs-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: longest common subsequence, bottom-up DP approach
  Time complexity: O(nm)
  Space complexity: O(nm)
  Note: no need to initialize the table with SIZE_MAX as in 
    lcs-2.c, because we fill the table in a bottom-up fashion
    and start from the base cases.
*/ 

#include "../../../lib/clib/clib.h"
#include <stdint.h> 
#define MAX(a,b) ((a) > (b) ? (a) : (b))

//===================================================================
// Returns the length of the longest common subsequence of X and Y
// using a bottom-up dynamic programming approach
size_t computeLcs (size_t **table, string *X, string *Y) {
  
  for (size_t i = 1; i <= strLen(X); ++i) {
    for (size_t j = 1; j <= strLen(Y); ++j) {
      if (charAt(X, i - 1) == charAt(Y, j - 1))
        // last chars of X and Y are identical: add 1
        table[i][j] = table[i - 1][j - 1] + 1; 
      else 
        // compute the maximum of the LCSs of the two alternatives:
        // either we remove the last char of X or the last char of Y
        table[i][j] = MAX(table[i - 1][j], table[i][j - 1]); 
    }
  }
  return table[strLen(X)][strLen(Y)];
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

  CREATE_MATRIX(size_t, table, strLen(X) + 1, strLen(Y) + 1);

  size_t lcs = computeLcs(table, X, Y);
  
  printf("Max length: %zu\nExample LCS:\n  ", lcs);
  reconstructLcs(table, X, strLen(X), strLen(Y));

  FREE_MATRIX(table, strLen(X) + 1);
  freeString(X);
  freeString(Y);
  
  return 0;
}
