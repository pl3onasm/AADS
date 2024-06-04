/* 
  file: rka.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: string matching using the Rabin-Karp algorithm
  assumption: length of the alphabet is 256 (extended ASCII)
*/

#include <limits.h>
#include "../../../lib/clib/clib.h"

#define d 256       // number of characters in the alphabet, d = |Σ|
#define p ULONG_MAX // a large prime number

//===================================================================
// Determines all valid shifts of pattern in text and prints them
void computeShifts (string *text, string* pattern) {
  unsigned long long pHash = 0, tHash = 0, h = 1; 
  
  printf("Shifts: ");
  bool foundShift = false;

    // precompute h = d^(m-1) % p
  for (size_t i = 0; i < strLen(pattern) - 1; i++) h = (h * d) % p;

    // precompute hash values for pattern and text[0..pLen-1]
  for (size_t i = 0; i < strLen(pattern); i++) {
    pHash = (pHash * d + charAt(pattern, i)) % p;
    tHash = (tHash * d + charAt(text, i)) % p;
  }
  
    // check for valid shifts by comparing hash values,
    // excluding spurious hits in case of hash collisions
  for (size_t i = 0; i <= strLen(text) - strLen(pattern); i++) {
    if (pHash == tHash && memcmp(str(text) + i, str(pattern), 
                                 strLen(pattern)) == 0) {         
        printf(foundShift ? ", %zu" : "%zu", i);
        foundShift = true;
      }
  
      // compute hash value for next shift
    if (i < strLen(text) - strLen(pattern)) 
      tHash = ((tHash - charAt(text, i) * h) * d 
               + charAt(text, strLen(pattern) + i)) % p;
  }
  if (foundShift) printf("\n"); 
  else printf("None.\n");
}

//===================================================================

int main () {

  READ_STRING(pattern, '\n');
  READ_STRING(text, EOF);

  computeShifts(text, pattern);

  freeString(text);
  freeString(pattern);
  return 0;
}