/* file: sma-3.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: string matching using an automaton,
     i.e. a FSM with a transition function
   time complexity: the automaton is built in O(md) time
     and the matching is done in O(n) time
   assumption: length of the alphabet is 256 (ASCII)
*/

#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;
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

void freeDelta (uint **delta, uint pLen) {
  /* frees the memory allocated for the transition function */
  uint i;
  for (i = 0; i <= pLen; i++) free(delta[i]);
  free(delta);
}

uint **computeDelta (char *pattern, uint pLen) {
  /* computes the transition function of the automaton */
  uint **delta = newM(pLen+1, d); 
  short a; 

  for (uint q = 0; q <= pLen; q++) {
    for (a = 0; a < d; a++) {
      // if match, go to next state
      if (q < pLen && pattern[q] == a) delta[q][a] = q+1;  
      // if empty string, stay in state 0
      else if (q == 0) delta[q][a] = 0;      
      // else transition to the previous state for the same character           
      else delta[q][a] = delta[delta[q-1][a]][a];  
    }
  }  
  return delta;
}

void matcher(char *text, uint tLen, uint** delta, uint pLen) {
  /* matches the pattern against the text */
  bool found = false;   // match found?
  for (uint i = 0, q = 0; i < tLen; i++) {
    q = delta[q][(short)text[i]];  // q is the state of the automaton
    if (q == pLen) {               // a match is found
      if (!found) {
        found = true; 
        printf("Shifts: %d", i-pLen+1);
      } else printf(", %d", i-pLen+1);
    }
  }
  if (found) printf("\n"); 
  else printf("No matches found.\n");
}

int main (int argc, char *argv[]) {
  uint tLen, pLen;

  char *pattern = readString(&pLen, 1);
  char *text = readString(&tLen, 0);

  uint **delta = computeDelta(pattern, pLen);
  matcher(text, tLen, delta, pLen);

  freeDelta(delta, pLen);
  free(text);
  free(pattern);
  return 0;
}