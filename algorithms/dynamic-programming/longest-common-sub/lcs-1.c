/* 
  file: lcs-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: longest common subsequence
    naive recursive implementation
    As the input strings get longer, the running time 
    of this algorithm grows exponentially.
  Time complexity: O(2^n)
  Don't try this code at home!
*/ 

#include "../../../lib/clib.h"

//===================================================================
// Returns the length of the longest common subsequence of X and Y
// using a naive recursive approach
size_t computeLcs (string *X, string *Y, size_t m, size_t n) {

  if (m == 0 || n == 0)
    // base case: reached the end of one of the strings 
    return 0;

  if (charAt(X, m - 1) == charAt(Y, n - 1)) 
    // last chars of X and Y are identical: add 1 
    return 1 + computeLcs(X, Y, m - 1, n - 1);

  else 
    // compute the maximum of the LCSs of the two alternatives:
    // either we remove the last char of X or the last char of Y
    return MAX(computeLcs(X, Y, m, n - 1), 
               computeLcs(X, Y, m - 1, n));
}

//===================================================================

int main () {
  
  READ_STRING(X, '\n'); 
  READ_STRING(Y, '\n');

  size_t lcs = computeLcs(X, Y, strLen(X), strLen(Y));

  printf("Max length: %lu\n", lcs);

  freeString(X);
  freeString(Y);
  
  return 0;
}