/* 
  file: wpm-4.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: wildcard pattern matching
    using a bottom-up DP approach with space optimization
  time complexity: O(n*m)
  space complexity: O(m)
*/

#include "../../../lib/clib.h"

//===================================================================
// Bottom-up DP approach to determine if a source string matches
// a pattern string with wildcards
bool matchPattern (string *src, string *ptn, bool *dp) {
    
    // base case: source is empty and pattern is empty;
    // match is true
  dp[0] = true;

    // base case: source is empty but pattern is not;
    // check if the remaining pattern chars are all '*'
  for (size_t i = 1; i <= strLen(ptn); ++i) 
    if (charAt(ptn, i - 1) == '*') 
      dp[i] = dp[i - 1];

    // fill the next row of the table using the 
    // results of the previous iteration
  for (size_t i = 1; i <= strLen(src); ++i) {
    bool prev = dp[0];
    dp[0] = false;
    for (size_t j = 1; j <= strLen(ptn); ++j) {
      bool curr = dp[j];
      if (charAt(src, i - 1) == charAt(ptn, j - 1) 
          || charAt(ptn, j - 1) == '?')
        dp[j] = prev;
      else if (charAt(ptn, j - 1) == '*')
        dp[j] = dp[j - 1] || dp[j];
      prev = curr;
    }
  }

  return dp[strLen(ptn)];
}

//===================================================================

int main () {
  
    // read source and pattern strings
  READ_STRING(src, '\n');
  READ_STRING(ptn, '\n');

    // allocate memoization table
  CREATE_ARRAY(bool, dp, strLen(ptn) + 1, false);

    // check if source matches pattern
  printf("Match: %s\n", 
          matchPattern(src, ptn, dp) ? "true" : "false");

  free(dp);
  freeString(src);
  freeString(ptn);

  return 0;
}