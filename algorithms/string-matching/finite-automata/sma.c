/* 
  file: sma.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: string matching using an automaton,
    i.e. a FSM with a transition function
  time complexity: automaton construction is in O(m³|Σ|) 
    and matching time is in O(n) 
  assumption: length of the alphabet is 256 (ASCII)
*/

#include "../../../lib/clib/clib.h"

#define d 256  // number of characters in the alphabet, d = |Σ|
#define MIN(a, b) ((a) < (b) ? (a) : (b))

//===================================================================
// Determines whether the pattern prefix of length k is also a  
// suffix of Pq + a
bool isSuffix (string *pattern, size_t q, size_t a, size_t k) {
  if (charAt(pattern, k - 1) != a) 
    return false;
  for (size_t i = 0; i < k - 1; i++) 
    if (charAt(pattern, i) != charAt(pattern, q - (k-1) + i)) 
      return false;
  return true;
}

//===================================================================
// Computes the transition function of the automaton in the form of
// a lookup table delta[q][a] = k, where q is the current state,  
// a is the input character and k is the next state
size_t **computeDelta (string *pattern) {

  CREATE_MATRIX(size_t, delta, strLen(pattern) + 1, d);

    // compute the transition for each state q and each input a
  for (size_t q = 0; q <= strLen(pattern); q++) {
    for (size_t a = 0; a < d; a++) {
        // determine k as the length of the longest pattern
        // prefix that is also a suffix of the string Pq + a
      size_t k = MIN(q + 1, strLen(pattern));
      while (k > 0 && !isSuffix(pattern, q, a, k)) 
        --k;
        // set k as the next state from state q on input a
      delta[q][a] = k;   
    }
  }  
  return delta;
}

//===================================================================
// Matches the pattern against the text and prints the shifts
void matcher(string *text, string *pattern, size_t **delta) {

  printf("Shifts: ");
  bool foundShift = false;
  
  for (size_t i = 0, q = 0; i < strLen(text); i++) {
    q =  delta[q][(size_t)charAt(text, i)];
    if (q == strLen(pattern)) {           
      printf(foundShift ? ", %zu" : "%zu", i - strLen(pattern) + 1);
      foundShift = true;  
    }
  }
  if (foundShift) printf("\n"); 
  else printf("None.\n");
}

//===================================================================

int main () {

  READ_STRING(pattern, '\n');
  READ_STRING(text, EOF);

  size_t **delta = computeDelta(pattern);
  matcher(text, pattern, delta);

  FREE_MATRIX(delta, strLen(pattern) + 1);
  freeString(text);
  freeString(pattern);
  return 0;
}