#include "clib.h"

//===================================================================
// Determines if pattern is a substring of text starting at start
bool isSubstring(string *text, string *pattern, size_t start) {
  if (start + pattern->size > text->size) return false;
  for (size_t i = 0; i < pattern->size; i++)
    if (text->data[start + i] != pattern->data[i]) return false;
  return true;
}

//===================================================================
// Shows a string
void showString(string *s){
  printf("%s\n", s->data);
}

//===================================================================
// Deallocates a string
void freeString(string *s){
  free(s->data);
  free(s);
}
