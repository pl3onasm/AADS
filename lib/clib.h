/* file: clib.h
   author: David De Potter
   description: library containing some useful definitions 
     and functions
*/

#ifndef CLIB_H_INCLUDED    
#define CLIB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

//::::::::::::::::::::::::::::: MATH :::::::::::::::::::::::::::::://

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(a) ((a) < 0 ? -(a) : (a))

//::::::::::::::::::::::::::: PRINTING ::::::::::::::::::::::::::://

  // macro for printing an array of a given type and length
  // Examples:  PRINT_ARRAY(myInts, "%d", 10);
  //            PRINT_ARRAY(myDbls, "%.2lf", 20);
#define PRINT_ARRAY(arr, format, len) \
  for (size_t arr##i = 0; arr##i < len; ++arr##i){ \
    printf(format, arr[arr##i]);\
    printf(arr##i == len-1 ? "\n" : ", "); \
  }

  // macro for printing a 2D matrix of a given type and dimensions
  // A 2D matrix is a stack of 1D arrays
  // Examples:  PRINT_MATRIX(myInts, "%d", rows, cols);
  //            PRINT_MATRIX(myDbls, "%.2lf", rows, cols);
  //            PRINT_MATRIX(myChrs, "%c", rows, cols);
#define PRINT_MATRIX(matrix, format, rows, cols) \
  for (size_t matrix##i = 0; matrix##i < rows; ++matrix##i) { \
    for (size_t matrix##j = 0; matrix##j < cols; ++matrix##j) {\
      printf(format, matrix[matrix##i][matrix##j]); \
      printf(matrix##j == cols-1 ? "\n" : " "); \
    } \
  }

//::::::::::::::::::::::::: READING INPUT :::::::::::::::::::::::://

  // macro for reading input into a 1D array of known length
  // Examples:  READ_ARRAY(myInts, "%d", 20);
  //            READ_ARRAY(myDbls, "%lf", 15);
  //            READ_ARRAY(myString, "%c", 10);
#define READ_ARRAY(arr, format, len) \
  for (size_t arr##i = 0; arr##i < len; ++arr##i) \
    (void)! scanf(format, &arr[arr##i])

  // macro for reading input into a 2D matrix of given dimensions
  // Examples:  READ_MATRIX(myInts, "%d", 10, 5);
  //            READ_MATRIX(myDbls, "%lf", 8, 8);
  //            READ_MATRIX(myChrs, "%c", 5, 10);
#define READ_MATRIX(matrix, format, rows, cols) \
  for (size_t arr##i = 0; arr##i < rows; ++arr##i) \
    for (size_t arr##j = 0; arr##j < cols; ++arr##j) \
      (void)! scanf(format, &matrix[arr##i][arr##j])

  // macro for reading input from stdin as long as it lasts
  // creates a new array of the given type and format, and
  // a new variable size with the number of elements read
  // and sets the last element to '\0'
  // Examples:  READ(int, "%d", myInts, myIntsLen);
  //            READ(double, "%lf", myDbls, myDblsLen);
  //            READ(char, "%c", myChrs, myChrsLen);
#define READ(type, arr, format, size) \
  type *arr = safeCalloc(100, sizeof(type)); \
  size_t size = 0; type arr##var; \
  while (scanf(format, &arr##var) == 1) { \
    arr[size++] = arr##var; \
    if (size % 100 == 0) { \
      arr = safeRealloc(arr, (size + 100) * sizeof(type)); \
      memset(arr + size, 0, 100); \
    } \
  } \
  arr[size] = '\0'

  // macro for reading input from stdin until a given 
  // delimiter is encountered. The delimiter should not
  // be able to be read by the format string.
  // Returns a new array of the given type and creates a
  // new variable size with the number of elements read
  // Examples:  READ_UNTIL(double, myDbls, "%lf", '\n', dblLen);
  //            READ_UNTIL(int, myInts, "%d", '.', intLen);
#define READ_UNTIL(type, arr, format, delim, size) \
  type *arr = safeCalloc(100, sizeof(type)); \
  size_t size = 0; type arr##var; \
  while (scanf(format, &arr##var) == 1) { \
    arr[size++] = arr##var; \
    if (size% 100 == 0) { \
      arr = safeRealloc(arr, (size + 100) * sizeof(type)); \
      memset(arr + size, 0, 100); \
    } \
  } \
  arr[size] = '\0';\
  (void) ! scanf("%*c ")
    
//::::::::::::::::::::::: MEMORY MANAGEMENT :::::::::::::::::::::://

  // macro for creating a 1D array of a given type and length
  // Examples:  CREATE_ARRAY(int, myInts, 10, 0);
  //            CREATE_ARRAY(double, myDbls, 20, DBL_MAX);
  //            CREATE_ARRAY(char, myString, 15, '\0');
#define CREATE_ARRAY(type, arr, len, init) \
  type *arr = safeCalloc(len, sizeof(type)); \
  if (init) \
    for (size_t arr##i = 0; arr##i < len; ++arr##i) \
      arr[arr##i] = init;

  // macro for creating a 2D matrix of given type and dimensions
  // Examples:  CREATE_MATRIX(int, myInts, 10, 10, INT_MAX);
  //            CREATE_MATRIX(double, myDbls, 10, 15, 0);
  //            CREATE_MATRIX(char, myChrs, 15, 10, '\0');
#define CREATE_MATRIX(type, matrix, rows, cols, init) \
  type **matrix = safeCalloc(rows, sizeof(type *)); \
  for (size_t matrix##i = 0; matrix##i < rows; ++matrix##i) { \
    matrix[matrix##i] = safeCalloc(cols, sizeof(type)); \
    if (init) \
      for (size_t matrix##j = 0; matrix##j < cols; ++matrix##j) \
        matrix[matrix##i][matrix##j] = init; \
  }

  // macro for creating a 3D matrix of given type and dimensions
  // A 3D matrix is a stack of 2D matrices, just like a 2D matrix
  // is a stack of 1D arrays.
  // Examples:  CREATE_3DMATRIX(int, myInts, 10, 10, 10);
  //            CREATE_3DMATRIX(double, myDbls, 10, 15, 20);
  //            CREATE_3DMATRIX(char, myChrs, 15, 10, 5);
#define CREATE_3DMATRIX(type, matrix, rows, cols, depth) \
  type ***matrix = safeCalloc(rows, sizeof(type **)); \
  for (size_t matrix##i = 0; matrix##i < rows; ++matrix##i) { \
    matrix[matrix##i] = safeCalloc(cols, sizeof(type *)); \
    for (size_t matrix##j = 0; matrix##j < cols; ++matrix##j) \
      matrix[matrix##i][matrix##j] = safeCalloc(depth, sizeof(type)); \
  }

  // macro for freeing the memory of a 2D matrix 
#define FREE_MATRIX(matrix, rows) \
  for (size_t matrix##i = 0; matrix##i < rows; ++matrix##i) \
    free(matrix[matrix##i]); \
  free(matrix);

    // macro for freeing the memory of a 3D matrix
#define FREE_3DMATRIX(matrix, rows, cols) \
  for (size_t matrix##i = 0; matrix##i < rows; ++matrix##i) { \
    for (size_t matrix##j = 0; matrix##j < cols; ++matrix##j) \
      free(matrix[matrix##i][matrix##j]); \
    free(matrix[matrix##i]); \
  } \
  free(matrix);

  // macro definition for swapping two variables
#define SWAP(a, b) swap(&a, &b, sizeof(a))

  // swaps two variables of any type
void swap(void *a, void *b, size_t size);

  // allocates memory and checks if it succeeded
void *safeMalloc(size_t n);
    
  // allocates memory, initialized to 0, and checks if it succeeded
void *safeCalloc(size_t n, size_t size);
    
  // reallocates memory and checks if it succeeded
void *safeRealloc(void *ptr, size_t newSize);
  
  // clears the input buffer
void clearStdin(char *buffer);

//:::::::::::::::::::::::::::: STRINGS ::::::::::::::::::::::::::://

typedef struct {
  unsigned char *data;
  size_t size, cap;
} string;

  // macro for reading all text until delimiter is found
  // Example:  READ_STRING(myString, '\n');
#define READ_STRING(arr, delim) \
  string *arr = safeCalloc(1, sizeof(string)); \
  arr->data = safeCalloc(100, sizeof(unsigned char)); \
  arr->cap = 100; \
  size_t arr##size##Len = 0; unsigned char arr##size##var; \
  while (scanf("%c", &arr##size##var) == 1 \
         && arr##size##var != delim) { \
    arr->data[arr##size##Len++] = arr##size##var; \
    if (arr##size##Len == arr->cap) { \
      arr->cap <<= 1; \
      arr->data = safeRealloc(arr->data, \
        (arr->cap) * sizeof(unsigned char)); \
      memset(arr->data + arr##size##Len, 0, arr->cap / 2); \
    } \
  } \
  arr->size = arr##size##Len

  // creates a new string with a given capacity 
static inline string *newString(size_t cap) {
  string *s = safeCalloc(1, sizeof(string));
  s->data = safeCalloc(cap, sizeof(unsigned char));
  s->cap = cap;
  return s;
}

  // copies a string  
static inline string *copyString(string *s) {
  string *copy = newString(s->cap);
  copy->size = s->size;
  memcpy(copy->data, s->data, s->size);
  return copy;
}

  // reverses a string
static inline string *reverseString(string *s) {
  for (size_t i = 0; i < s->size / 2; i++) {
    unsigned char tmp = s->data[i];
    s->data[i] = s->data[s->size - i - 1];
    s->data[s->size - i - 1] = tmp;
  }
  return s;
}

  // concatenates two strings
static inline void concatStrings(string *s1, string *s2) {
  if (s1->size + s2->size >= s1->cap) {
    s1->cap = s1->size + s2->size + 1;
    s1->data = safeRealloc(s1->data, 
                           s1->cap * sizeof(unsigned char));
  }
  memcpy(s1->data + s1->size, s2->data, s2->size);
  s1->size += s2->size;
  s1->data[s1->size] = '\0';
}

  // appends a character to a string
static inline void appendChar(string *s, unsigned char c) {
  if (s->size == s->cap) {
    s->cap <<= 1;
    s->data = safeRealloc(s->data,
                          s->cap * sizeof(unsigned char));
  }
  s->data[s->size] = c;
  s->data[++s->size] = '\0';
}

  // shows a string
static inline void showString(string *s) {
  if (s->size == 0) {
    printf("<Empty string>\n");
    return;
  }
  printf("%s\n", s->data);
}

  // shows a substring of a string
static inline void showSubstring(string *s, size_t start, 
                                 size_t end, char delim) {
  for (size_t i = start; i <= end; i++) 
    printf("%c", s->data[i]);
  if (delim) printf("%c", delim);
}

  // deallocates a string
static inline void freeString(string *s) {
  free(s->data);
  free(s);
}

  // returns the i-th character of a string
static inline unsigned char charAt(string *s, size_t i) {
  return s->data[i];
}

  // sets the i-th character of a string
static inline void setCharAt(string *s, size_t i, unsigned char c) {
  s->data[i] = c;
}

static inline unsigned char *str(string *s) {
  return s->data;
}

static inline size_t strLen(string *s) {
  return s->size;
}

#endif // CLIB_H_INCLUDED