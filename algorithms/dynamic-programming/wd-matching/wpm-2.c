/* 
  file: wpm-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: wildcard pattern matching
    using top-down DP approach with memoization
  time complexity: O(n*m)
  note: we can't use a boolean matrix for memoization
    because we need to distinguish between true, false and
    uninitialized values. We use -1 for uninitialized values.
*/

#include "../../../lib/clib/clib.h"

//===================================================================
// Naive recursive approach to determine if a source string matches
// a pattern string with wildcards
short matchPattern (string *src, string *ptn, size_t i, size_t j, 
                   short **dp) {
    
    // check if value is already memoized
  if (dp[i][j] != -1) return dp[i][j];

    // source and pattern are both empty: match found
  if (i == 0 && j == 0) return dp[i][j] = 1;

    // source is empty but pattern is not:
    // check if remaining pattern is all *
  if (i == 0)
    return dp[i][j] = charAt(ptn, j - 1) == '*'
                      && matchPattern(src, ptn, i, j - 1, dp);
  
    // pattern is empty: source must be empty 
  if (j == 0) return dp[i][j] = i == 0;
  
    // current characters match: move to next 
  if (charAt(src, i - 1) == charAt(ptn, j - 1) 
      || charAt(ptn, j - 1) == '?')
    return dp[i][j] = matchPattern(src, ptn, i - 1, j - 1, dp);
  
    // current pattern char is wildcard: skip source char
    // or skip pattern char
  if (charAt(ptn, j - 1) == '*')
    return dp[i][j] = matchPattern(src, ptn, i, j - 1, dp)
                      || matchPattern(src, ptn, i - 1, j, dp);
  
    // current characters don't match
  return dp[i][j] = 0;
}


//===================================================================

int main () {
  
    // read source and pattern strings
  READ_STRING(src, '\n');
  READ_STRING(ptn, '\n');

    // allocate memoization table
  CREATE_MATRIX(short, dp, strLen(src) + 1, strLen(ptn) + 1, -1);

    // check if source matches pattern
  printf("Match: %s\n", 
          matchPattern(src, ptn, strLen(src), strLen(ptn), dp)
          ? "true" : "false");
  
  FREE_MATRIX(dp, strLen(src) + 1);
  freeString(src);
  freeString(ptn);

  return 0;
}