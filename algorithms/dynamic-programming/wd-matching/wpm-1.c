/* 
  file: wpm-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: wildcard pattern matching
    using a naive recursive approach
  time complexity: exponential
  caution: may explode for strings of large lengths 
    or patterns with many wildcards
*/

#include "../../../lib/clib.h"

//===================================================================
// Naive recursive approach to determine if a source string matches
// a pattern string with wildcards
bool matchPattern (string *src, string *ptn, size_t i, size_t j) {
  
    // source is empty: pattern must be empty
  if (i == 0 && j == 0) return true;

    // source is empty but pattern is not: 
    // check if pattern is all *
  if (i == 0) 
    return charAt(ptn, j - 1) == '*' 
           && matchPattern(src, ptn, i, j - 1);
  
    // pattern is empty: source must be empty 
  if (j == 0) return i == 0;
  
    // current characters match: move to next
  if (charAt(src, i - 1) == charAt(ptn, j - 1) 
      || charAt(ptn, j - 1) == '?')
    return matchPattern(src, ptn, i - 1, j - 1);
  
    // current pattern char is a * wildcard: skip source char
    // or skip pattern char
  if (charAt(ptn, j - 1) == '*')
    return matchPattern(src, ptn, i, j - 1) 
           || matchPattern(src, ptn, i - 1, j);
  
    // current characters don't match
  return false;
}


//===================================================================

int main () {
  
    // read source and pattern strings
  READ_STRING(src, '\n');
  READ_STRING(ptn, '\n');

    // check if source matches pattern
  printf("Match: %s\n", 
          matchPattern(src, ptn, strLen(src), strLen(ptn)) 
          ? "true" : "false");
  
  freeString(src);
  freeString(ptn);
  return 0;
}