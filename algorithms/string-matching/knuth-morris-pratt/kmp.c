/* file: kmp.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: string matching using the Knuth-Morris-Pratt algorithm
   time complexity: O(m + n) since the prefix function is computed in O(m) time
     and the matching is done in O(n) time
   assumption: length of the alphabet is 256 (ASCII)
*/

#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;
typedef unsigned char uchar;
#define d 256  // number of characters in the alphabet

typedef enum {               
  false = 0,
  true = 1
} bool;

void *safeCalloc (int n, int size) {
  /* allocates memory, and checks whether this was successful */
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *safeRealloc (void *ptr, int newSize) {
  /* reallocates memory and checks whether it was successful */
  ptr = realloc(ptr, newSize);
  if (ptr == NULL) {
    printf("Error: realloc(%d) failed. Out of memory?\n", newSize);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

uint **newM (uint m, uint n) {
  /* allocates memory for a m x n matrix */
  uint i, **M = safeCalloc(m, sizeof(uint*));
  for (i = 0; i < m; i++) M[i] = safeCalloc(n, sizeof(uint));
  return M;
}

uchar *readString(uint *size, short type) {
  /* reads a string from stdin and stores its length in size */
  uchar c; uint len = 0; 
  uchar tok = type ? '\n' : EOF;   // type = 1: read until newline
  uchar *str = safeCalloc(100, sizeof(uchar));
  while (scanf("%c", &c) == 1 && c != tok) {
    if (c == '\n') c = ' '; // replace newline with space
    str[len++] = c; 
    if (len % 100 == 0) str = safeRealloc(str, (len+100) * sizeof(uchar));
  }
  str[len] = '\0';
  *size = len;
  return str;
}

uint *computePrefixFunction (uchar *pattern, uint pLen) {
  /* computes the prefix function of pattern */
  uint *pi = safeCalloc(pLen, sizeof(uint));
  for (uint q = 1, k = 0; q < pLen; q++) {
    while (k > 0 && pattern[k] != pattern[q]) k = pi[k-1];
    if (pattern[k] == pattern[q]) k++;
    pi[q] = k;
  }
  return pi;
}

void matcher (uchar *pattern, uint pLen, uchar *text, uint tLen) {
  /* matches pattern in text using the Knuth-Morris-Pratt algorithm */
  uint *pi = computePrefixFunction(pattern, pLen);
  bool found = false;  // match found?
  for (uint i = 0, q = 0; i < tLen; i++) {
    // get longest prefix of pattern that matches a suffix of text[0..i]
    while (q > 0 && pattern[q] != text[i]) q = pi[q-1];   
    if (pattern[q] == text[i]) q++;
    if (q == pLen) {
      if (!found) {
        found = true;
        printf("Shifts: %d", i-q+1);
      } else printf(", %d", i-q+1);
      q = pi[q-1];
    }
  }
  if (found) printf("\n");
  else printf("No matches found.\n");
  free(pi);
}

int main () {
  uint tLen, pLen;

  uchar *pattern = readString(&pLen, 1);
  uchar *text = readString(&tLen, 0);

  matcher(pattern, pLen, text, tLen);

  free(text);
  free(pattern);
  return 0;
}