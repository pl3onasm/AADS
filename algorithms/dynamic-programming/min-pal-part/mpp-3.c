/* 
  file: mpp-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: minimal palindromic partitioning 
    using bottom-up DP approach
  time complexity: O(n^2)
*/ 

#include "../../../lib/clib.h"

//===================================================================
// Reconstructs an optimal palindromic partitioning of a given string
void showCuts(string *S, size_t *cutPoints, size_t k) {
  if (cutPoints[k] == k) 
    showSubstring(S, 0, k, ' ');
  else {
    showCuts(S, cutPoints, cutPoints[k]);
    printf("| ");
    showSubstring(S, cutPoints[k] + 1, k, ' ');
  }
}

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
// into palindromes, using bottom-up DP
size_t partition(string *S, size_t *dp, size_t *cutPoints) {

  bool **isPalindrome = computePalindromes(S);

  for (size_t i = 0; i < strLen(S); i++) 
    if (isPalindrome[0][i]) {
      cutPoints[i] = i;
      dp[i] = 0;
    } else {
      for (size_t j = 0; j < i; j++) 
        if (isPalindrome[j + 1][i] && dp[j] + 1 < dp[i]) {
          dp[i] = dp[j] + 1;
          cutPoints[i] = j;
        }
    }

  FREE_MATRIX(isPalindrome, strLen(S));
  return dp[strLen(S) - 1];
}

//===================================================================

int main() {

  READ_STRING(S, '\n');

  CREATE_ARRAY(size_t, dp, strLen(S), SIZE_MAX);
  CREATE_ARRAY(size_t, cutPoints, strLen(S), 0);

  partition(S, dp, cutPoints);

  printf("Min cuts: %zu\n", dp[strLen(S) - 1]);
  printf("An optimal partitioning:\n  ");
  showCuts(S, cutPoints, strLen(S) - 1);
  printf("\n");

  freeString(S);
  free(dp);
  free(cutPoints);

  return 0;
}