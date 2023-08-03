/* file: nsm.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: string matching, naive approach
*/

#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;

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
  /* determines the valid pattern shifts using brute force */
  if (pLen > tLen) {
    printf("Pattern is longer than text. No shifts found.\n"); 
    return;
  }
  short r = 0; 
  uint i, j, n = tLen - pLen;
  for (i = 0; i <= n; i++) {
    for (j = 0; j < pLen; j++) {
      if (text[i+j] != pattern[j]) break;
    }
    if (j == pLen) {  // valid shift
      if (!r) {
        r = 1;
        printf("Shifts: %d", i);
      } else printf(", %d", i);
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