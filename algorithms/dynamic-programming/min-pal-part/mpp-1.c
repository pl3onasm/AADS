/* 
  file: mpp-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: minimal palindromic partitioning 
    using a naive recursive approach
  time complexity: O(2^n)
  caution: may explode for n > 20
*/ 

#include "../../../lib/clib/clib.h"
#define MIN(a, b) ((a) < (b) ? (a) : (b))

//===================================================================
// Precomputes a function returning a boolean value indicating
// whether a substring of the input string is a palindrome
bool **computePalindromes(string *S) {

  CREATE_MATRIX(bool, pal, strLen(S), strLen(S), false);

  for (size_t i = strLen(S); i--; )
    for (size_t j = i; j < strLen(S); j++) 
      if (i == j) pal[i][j] = true;
      else if (charAt(S, i) == charAt(S, j)) 
        pal[i][j] = j - i == 1 ? true : pal[i + 1][j - 1];
      
  return pal;
}

//===================================================================
// Computes the minimal number of cuts required to partition a string
// into palindromes, using a naive recursive approach
size_t partition(string *S, size_t i, bool **isPalindrome) {
  
  if (isPalindrome[0][i]) 
    return 0;

  size_t minCuts = i + 1;

  for (size_t j = 0; j < i; j++)
    if (isPalindrome[j + 1][i]) 
      minCuts = MIN(minCuts, 1 + partition(S, j, isPalindrome));

  return minCuts;
}


//===================================================================

int main() {

  READ_STRING(S, '\n');

  bool **isPalindrome = computePalindromes(S);

  size_t cuts = partition(S, strLen(S) - 1, isPalindrome);

  printf("Min cuts: %zu\n", cuts);

  FREE_MATRIX(isPalindrome, strLen(S));
  freeString(S);

  return 0;
}