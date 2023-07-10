/* file: kmp-4.c
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
#define d 256  // number of characters in the alphabet

void *safeCalloc (int n, int size) {
  /* allocates n elements of size size, initializing them to 0, and
     checks whether the allocation was successful */
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *safeRealloc (void *ptr, int newSize) {
  // reallocates memory and checks whether the allocation was successful
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

char *readString(uint *size, short type) {
  /* reads a string from stdin and stores its length in size */
  char c; uint len = 0; 
  char tok = type ? '\n' : EOF;   // type = 1: read until newline
  char *str = safeCalloc(100, sizeof(char));
  while (scanf("%c", &c) == 1 && c != tok) {
    str[len++] = c; 
    if (len % 100 == 0) str = safeRealloc(str, (len+100) * sizeof(char));
  }
  str[len] = '\0';
  *size = len;
  return str;
}

uint *computePrefixFunction (char *pattern, uint pLen) {
  /* computes the prefix function of pattern */
  uint *pi = safeCalloc(pLen, sizeof(uint));
  for (uint q = 1, k = 0; q < pLen; q++) {
    while (k > 0 && pattern[k] != pattern[q]) k = pi[k-1];
    if (pattern[k] == pattern[q]) k++;
    pi[q] = k;
  }
  return pi;
}

void matcher (char *pattern, uint pLen, char *text, uint tLen) {
  /* matches pattern in text using the Knuth-Morris-Pratt algorithm */
  uint *pi = computePrefixFunction(pattern, pLen);
  short found = 0;
  for (uint i = 0, q = 0; i < tLen; i++) {
    while (q > 0 && pattern[q] != text[i]) q = pi[q-1];   // get longest prefix
    if (pattern[q] == text[i]) q++;
    if (q == pLen) {
      if (!found) {
        found = 1;
        printf("Shifts: %d", i-q+1);
      } else printf(", %d", i-q+1);
      q = pi[q-1];
    }
  }
  if (found) printf("\n");
  else printf("No matches found.\n");
  free(pi);
}

int main (int argc, char *argv[]) {
  uint tLen, pLen;

  char *pattern = readString(&pLen, 1);
  char *text = readString(&tLen, 0);

  matcher(pattern, pLen, text, tLen);

  free(text);
  free(pattern);
  return 0;
}