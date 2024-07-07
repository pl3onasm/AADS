/* 
  file: kmp.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: string matching using the Knuth-Morris-Pratt 
    algorithm
  time complexity: O(m + n) since the prefix function is 
    computed in O(m) time and the matching is done in O(n) time
  assumption: length of the alphabet is 256 (ASCII)
*/

#include "../../../lib/clib/clib.h"

#define d 256  // number of characters in the alphabet, d = |Σ|

//===================================================================
// Computes the prefix function of the pattern
size_t *computePrefixFunction (string *pattern) {
  
  CREATE_ARRAY(size_t, pi, strLen(pattern), 0);

    // pi[0] is always 0, since no proper prefix exists for
    // a string of length 1. This is why we start q at 1.
  for (size_t q = 1, k = 0; q < strLen(pattern); q++) {
      // get longest pattern prefix of length k that is a
      // proper suffix of pattern[0..q] 
    while (k > 0 && charAt(pattern, k) != charAt(pattern, q))
      k = pi[k - 1];
      // increment k if next character matches
    if (charAt(pattern, k) == charAt(pattern, q)) 
      k++;
      // store the length k of the longest prefix of pattern[0..q]
    pi[q] = k;
  }
  return pi;
}

//===================================================================
// Determines all valid shifts of pattern in text and prints them
void matcher (string *text, string *pattern, size_t *pi) {
  
  printf("Shifts: ");
  bool foundShift = false;  

  for (size_t i = 0, q = 0; i < strLen(text); i++) {
      // get longest pattern prefix of length q
      // matching a suffix of text[0..i]
    while (q > 0 && charAt(pattern, q) != charAt(text, i)) 
      q = pi[q - 1];   
      // increment q if next character matches
    if (charAt(pattern, q) == charAt(text, i)) 
      q++;
      // print valid shift if pattern has been found
    if (q == strLen(pattern)) {
      printf(foundShift ? ", %zu" : "%zu", i - q + 1);
      foundShift = true;  
      q = pi[q - 1];
    }
  }
  if (foundShift) printf("\n");
  else printf("None.\n");
}

//===================================================================

int main () {

  READ_STRING(pattern, '\n');
  READ_STRING(text, EOF);

  size_t *pi = computePrefixFunction(pattern);
  matcher(text, pattern, pi);

  free(pi);
  freeString(text);
  freeString(pattern);
  return 0;
}