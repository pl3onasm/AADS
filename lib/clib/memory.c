#include "clib.h"
#include <string.h>

//=================================================================  
// swaps two variables of any type
void swap(void *a, void *b, size_t size) {
  unsigned char *buffer = safeMalloc(size);
  memcpy(buffer, a, size);
  memcpy(a, b, size);
  memcpy(b, buffer, size);
  free(buffer);
}

//=================================================================
// clears the input buffer
void clearStdin(char *buffer) {
  if (buffer[strlen(buffer) - 1] != '\n') {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
  }
}

//=================================================================
// allocates memory and checks whether this was successful
void *safeMalloc(size_t n) {
  void *ptr = malloc(n);
  if (ptr == NULL) {
    printf("Error: malloc(%zu) failed. "
           "Out of memory?\n", n);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

//=================================================================
// allocates memory, initialized to 0, and
// checks whether this was successful 
void *safeCalloc(size_t n, size_t size) {
  
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    printf("Error: calloc(%lu, %zu) failed. "
           "Out of memory?\n", n, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

//=================================================================
// reallocates memory and checks if this was successful
void *safeRealloc(void *ptr, size_t newSize) {
  ptr = realloc(ptr, newSize);
  if (ptr == NULL) {
    printf("Error: realloc(%zu) failed. "
           "Out of memory?\n", newSize);
    exit(EXIT_FAILURE);
  }
  return ptr;
}
