/* 
  file: wpm-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: wildcard pattern matching
    using a bottom-up DP approach
  time complexity: O(n*m)
  space complexity: O(n*m)
*/

#include "../../../lib/clib/clib.h"

//===================================================================
// Bottom-up DP approach to determine if a source string matches
// a pattern string with wildcards
bool matchPattern (string *src, string *ptn, bool **dp) {
    
    // base case: source is empty and pattern is empty;
    // match is true
  dp[0][0] = true;

    // base case: source is empty but pattern is not;
    // check if the remaining pattern chars are all '*'
  for (size_t i = 1; i <= strLen(ptn); ++i) 
    if (charAt(ptn, i - 1) == '*') 
      dp[0][i] = dp[0][i - 1];

    // fill the rest of the table using the recursive formula
  for (size_t i = 1; i <= strLen(src); ++i) {
    for (size_t j = 1; j <= strLen(ptn); ++j) {
      if (charAt(src, i - 1) == charAt(ptn, j - 1) 
          || charAt(ptn, j - 1) == '?')
        dp[i][j] = dp[i - 1][j - 1];
      else if (charAt(ptn, j - 1) == '*')
        dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
    }
  }

  return dp[strLen(src)][strLen(ptn)];
}

//===================================================================

int main () {
  
    // read source and pattern strings
  READ_STRING(src, '\n');
  READ_STRING(ptn, '\n');

    // allocate memoization table
  CREATE_MATRIX(bool, dp, strLen(src) + 1, strLen(ptn) + 1, false);

    // check if source matches pattern
  printf("Match: %s\n", 
          matchPattern(src, ptn, dp) ? "true" : "false");

  FREE_MATRIX(dp, strLen(src) + 1);
  freeString(src);
  freeString(ptn);

  return 0;
}