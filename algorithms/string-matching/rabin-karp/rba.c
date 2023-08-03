/* file: rba.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: string matching using the Rabin-Karp algorithm
   assumption: length of the alphabet is 256 (ASCII)
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned int uint;
typedef unsigned long ul;
#define d 256  // number of characters in the alphabet

void *safeMalloc (int n) {
  /* allocates n bytes of memory and checks whether the allocation
     was successful */
  void *ptr = malloc(n);
  if (ptr == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
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

char *readString(uint *size, short type) {
  /* reads a string from stdin and stores its length in size */
  char c; uint len = 0; 
  char tok = type ? '\n' : EOF;   // type = 1: read until newline
  char *str = safeMalloc(100*sizeof(char));
  while (scanf("%c", &c) == 1 && c != tok) {
    str[len++] = c; 
    if (len % 100 == 0) str = safeRealloc(str, (len+100) * sizeof(char));
  }
  str[len] = '\0';
  *size = len;
  return str;
}

void computeShifts (char *text, uint tLen, char *pattern, uint pLen) {
  /* determines the valid pattern shifts using the Rabin-Karp algorithm */
  if (pLen > tLen) {
    printf("Pattern is longer than text. No shifts found.\n"); 
    return;
  }
  ul q = ULONG_MAX, pHash = 0, tHash = 0, h = 1; 
  short r = 0; 
  uint i, j, n = tLen - pLen;
  
  for (i = 0; i < pLen-1; i++) h = (h * d) % q;

  // compute hash values for pattern and text[0..pLen-1]
  for (i = 0; i < pLen; i++) {
    pHash = (pHash * d + pattern[i]) % q;
    tHash = (tHash * d + text[i]) % q;
  }
  
  // check for valid shifts
  for (i = 0; i <= n; i++) {
    if (pHash == tHash) {  
      for (j = 0; j < pLen; j++)    // spurious hit ?
        if (text[i+j] != pattern[j]) break;
      if (j == pLen) {  // valid shift
        if (!r) {
          r = 1;
          printf("Shifts: %d", i);
        } else printf(", %d", i);
      }
    }
    if (i < n) {  // compute hash value for next shift
      tHash = ((tHash - text[i] * h) * d + text[i+pLen]) % q;
      if (tHash < 0) tHash += q;  
    }
  }
  if (r) printf("\n"); 
  else printf("No matches found.\n");
}

int main (int argc, char *argv[]) {
  uint tLen, pLen;

  char *pattern = readString(&pLen, 1);
  char *text = readString(&tLen, 0);

  computeShifts(text, tLen, pattern, pLen);

  free(text);
  free(pattern);
  return 0;
}