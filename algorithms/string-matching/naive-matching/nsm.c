/* file: nsm.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: string matching, naive approach
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../../lib/clib/clib.h"

//===================================================================
// Determines all valid shifts of pattern in text and prints them
void computeShifts (string *text, string *pattern) {
  printf("Shifts: ");
  bool foundShift = false;

    // loop over all possible shifts
  for (size_t i = 0; i <= strLen(text) - strLen(pattern); i++) {
    if (memcmp(str(text) + i, str(pattern), strLen(pattern)) == 0) {
      printf(foundShift ? ", %zu" : "%zu", i);
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

  if (strLen(pattern) > strLen(text)) {
    printf("Pattern is longer than text.\n");
    freeString(text);
    freeString(pattern);
    return 0;
  }

  computeShifts(text, pattern);

  freeString(text);
  freeString(pattern);
  return 0;
}