/* 
  file: lps-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: Longest Palindromic Subsequence
    This is a naive recursive implementation. 
  time complexity: O(2^n)
  Use at your own risk for n > 20
*/

#include "../../../lib/clib.h"

//===================================================================
// Returns the length of the longest palindromic subsequence of X
size_t computeLps(string *s, size_t start, size_t end) {

  if (start >= end) 
    return start == end;

  if (charAt(s, start) == charAt(s, end)) 
    return 2 + computeLps(s, start + 1, end - 1);  

  return MAX(computeLps(s, start + 1, end), 
             computeLps(s, start, end - 1));
}

//===================================================================

int main () {
  
  READ_STRING(X, '\n'); 

  size_t lps = computeLps(X, 0, strLen(X) - 1);

  printf("LPS length: %lu\n", lps);

  freeString(X);
  
  return 0;
}