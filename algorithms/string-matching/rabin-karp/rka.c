/* file: rka.c
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
typedef unsigned char uchar;
#define d 256  // number of characters in the alphabet, d = |Σ|

typedef enum {               
  false = 0,
  true = 1
} bool;

void *safeMalloc (int n) {
  /* allocates memory and checks whether it was successful */
  void *ptr = malloc(n);
  if (ptr == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *safeRealloc (void *ptr, int newSize) {
  /* reallocates memory and checks whether the allocation was successful */
  ptr = realloc(ptr, newSize);
  if (ptr == NULL) {
    printf("Error: realloc(%d) failed. Out of memory?\n", newSize);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

uchar *readString(uint *size, short type) {
  /* reads a string from stdin and stores its length in size */
  uchar c; uint len = 0; 
  uchar tok = type ? '\n' : EOF;   // type = 1: read until newline
  uchar *str = safeMalloc(100*sizeof(uchar));
  while (scanf("%c", &c) == 1 && c != tok) {
    if (c == '\n') c = ' '; // replace newline with space
    str[len++] = c; 
    if (len % 100 == 0) str = safeRealloc(str, (len+100) * sizeof(uchar));
  }
  str[len] = '\0';
  *size = len;
  return str;
}

void computeShifts (uchar *text, uint tLen, uchar *pattern, uint pLen) {
  /* determines all valid shifts of pattern in text and prints them */
  if (pLen > tLen) {
    printf("Pattern is longer than text. No shifts found.\n"); 
    return;
  }
  ul q = ULONG_MAX, pHash = 0, tHash = 0, h = 1; 
  bool r = false;   // result found? 
  uint i, j, n = tLen - pLen;
  
  for (i = 0; i < pLen-1; i++) h = (h * d) % q;

  // precompute hash values for pattern and text[0..pLen-1]
  for (i = 0; i < pLen; i++) {
    pHash = (pHash * d + pattern[i]) % q;
    tHash = (tHash * d + text[i]) % q;
  }
  
  // check for valid shifts by comparing hash values
  for (i = 0; i <= n; i++) {
    if (pHash == tHash) {   // hash values match
      for (j = 0; j < pLen; j++)    
        if (text[i+j] != pattern[j]) break; // spurious hit
      if (j == pLen) {  // valid shift
        if (!r) {
          r = true;
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

int main () {
  uint tLen, pLen;

  uchar *pattern = readString(&pLen, 1);
  uchar *text = readString(&tLen, 0);

  computeShifts(text, tLen, pattern, pLen);

  free(text);
  free(pattern);
  return 0;
}