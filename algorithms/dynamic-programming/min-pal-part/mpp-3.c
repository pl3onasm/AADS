/* 
  file: mpp-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: minimal palindromic partitioning 
    using bottom-up DP approach
  time complexity: O(n^2)
*/ 

#include "../../../lib/clib/clib.h"

//===================================================================
// Reconstructs an optimal palindromic partitioning of a given string
void showCuts(string *S, size_t *cuts, size_t k) {
  if (cuts[k] == k) {
    for (size_t i = 0; i <= k; i++) 
      printf("%c", charAt(S, i));
  } else {
    showCuts(S, cuts, cuts[k]);
    printf(" | ");
    for (size_t i = cuts[k] + 1; i <= k; i++) 
      printf("%c", charAt(S, i));
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
size_t partition(string *S, size_t *dp, size_t *cuts) {

  bool **isPalindrome = getPalindromes(S);

  for (size_t i = 0; i < strLen(S); i++) {
    if (isPalindrome[0][i]) 
      cuts[i] = i;
    else {
      dp[i] = i + 1;
      for (size_t j = 0; j < i; j++) {
        if (isPalindrome[j + 1][i] && dp[j] + 1 < dp[i]) {
          dp[i] = dp[j] + 1;
          cuts[i] = j;
        }
      }
    }
  }

  FREE_MATRIX(isPalindrome, strLen(S));
  return dp[strLen(S) - 1];
}

//===================================================================

int main() {

  READ_STRING(S, '\n');

  CREATE_ARRAY(size_t, dp, strLen(S), 0);
  CREATE_ARRAY(size_t, cuts, strLen(S), 0);

  partition(S, dp, cuts);

  printf("Min cuts: %zu\n", dp[strLen(S) - 1]);
  printf("An optimal partitioning:\n  ");
  showCuts(S, cuts, strLen(S) - 1);
  printf("\n");

  freeString(S);
  free(dp);
  free(cuts);

  return 0;
}