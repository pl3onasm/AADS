/* 
  file: lps-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: longest palindromic subsequence
    bottom-up dynamic programming implementation
  time complexity: O(n^2)
*/ 

#include "../../../lib/clib.h"

//===================================================================
// Returns the length of the longest palindromic subsequence of X
size_t computeLps(string *X, size_t start, size_t end, 
                  size_t **memo) {
    
    // base case: any single character is a palindrome
  for (size_t i = 0; i < strLen(X); ++i)
    memo[i][i] = 1;

    // bottom-up approach: fill the memo table by increasing
    // the length of the subsequences
  for (size_t i = 1; i < strLen(X); ++i)
      // i is the length of the current subsequence
    for (size_t j = 0; j < strLen(X) - i; ++j) {
        // k is the end index of the current subsequence X[j..k]
      size_t k = j + i;
      if (charAt(X, j) == charAt(X, k))
        memo[j][k] = 2 + memo[j + 1][k - 1];
      else
        memo[j][k] = MAX(memo[j + 1][k], memo[j][k - 1]);
    }

  return memo[start][end];
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

