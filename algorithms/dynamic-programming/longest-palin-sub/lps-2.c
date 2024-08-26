/* 
  file: lps-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: longest palindromic subsequence
    top-down dynamic programming implementation
  time complexity: O(n^2)
*/ 

#include "../../../lib/clib.h"

//===================================================================
// Returns the length of the longest palindromic subsequence of X
size_t computeLps(string *X, size_t start, size_t end, 
                  size_t **memo) {

  if (memo[start][end])
    return memo[start][end];

  if (start >= end) 
    return memo[start][end] = start == end;

  if (charAt(X, start) == charAt(X, end)) 
    return 
      memo[start][end] = 2 + computeLps(X, start + 1, end - 1, memo);

  return 
    memo[start][end] = MAX(computeLps(X, start + 1, end, memo), 
                           computeLps(X, start, end - 1, memo));
}

//===================================================================
// Reconstructs an LPS of X
void reconstructLps(string *X, size_t row, size_t col, 
                    size_t **memo, size_t lps) {

  string *lpsStr = newString(lps + 1);
  size_t start = 0, end = lps - 1;

  while (memo[row][col]) {
    if (memo[row][col] == memo[row + 1][col]) 
      ++row;
    else if (memo[row][col] == memo[row][col - 1]) 
      --col;
    else {
      setCharAt(lpsStr, start++, charAt(X, col));
      setCharAt(lpsStr, end--, charAt(X, col));
      ++row; --col;
    }
  }

  showString(lpsStr);
  freeString(lpsStr);
}

//===================================================================

int main () {
  
  READ_STRING(X, '\n'); 

  CREATE_MATRIX(size_t, memo, strLen(X), strLen(X), 0);

  size_t lps = computeLps(X, 0, strLen(X) - 1, memo);

  printf("LPS length: %zu\nLPS: ", lps);
  reconstructLps(X, 0, strLen(X) - 1, memo, lps);

  FREE_MATRIX(memo, strLen(X));
  freeString(X);
  
  return 0;
}

