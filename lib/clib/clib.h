/* file: clib.h
   author: David De Potter
   description: library containing some useful definitions 
     and functions
*/

#ifndef CLIB_H_INCLUDED    
#define CLIB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

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
    (void)! scanf(format, &arr[arr##i]);

  // macro for reading input into a 2D matrix of given dimensions
  // Examples:  READ_MATRIX(myInts, "%d", 10, 5);
  //            READ_MATRIX(myDbls, "%lf", 8, 8);
  //            READ_MATRIX(myChrs, "%c", 5, 10);
#define READ_MATRIX(matrix, format, rows, cols) \
  for (size_t arr##i = 0; arr##i < rows; ++arr##i) \
    for (size_t arr##j = 0; arr##j < cols; ++arr##j) \
      (void)! scanf(format, &matrix[arr##i][arr##j]);

  // macro for reading input from stdin as long as it lasts
  // creates a new array of the given type and format,
  // sets the parameter size to the number of elements read
  // and sets the last element to '\0'
  // Examples:  READ(int, "%d", myInts, size);
  //            READ(double, "%lf", myDbls, size);
  //            READ(char, "%c", myChrs, size);
#define READ(type, arr, format, size) \
  type *arr = safeCalloc(100, sizeof(type)); \
  size_t arr##Len = 0; type arr##var; \
  while (scanf(format, &arr##var) == 1) { \
    arr[arr##Len++] = arr##var; \
    if (arr##Len % 100 == 0) { \
      arr = safeRealloc(arr, (arr##Len + 100) * sizeof(type)); \
      memset(arr + arr##Len, 0, 100); \
    } \
  } \
  size = arr##Len;\
  arr[arr##Len] = '\0';

  // macro for reading input from stdin until a given 
  // delimiter is encountered 
  // returns a string containing all chars read
  // and sets size to the number of chars read
  // Examples:  READ_UNTIL(char, myString, "%c", '\n', size);
  //            READ_UNTIL(int, myInts, "%d", -1, size);
#define READ_UNTIL(type, arr, format, delim, size) \
  type *arr = safeCalloc(100, sizeof(type)); \
  size_t arr##Len = 0; type arr##var; \
  while (scanf(format, &arr##var) == 1 && arr##var != delim) { \
    arr[arr##Len++] = arr##var; \
    if (arr##Len % 100 == 0) { \
      arr = safeRealloc(arr, (arr##Len + 100) * sizeof(type)); \
      memset(arr + arr##Len, 0, 100); \
    } \
  } \
  size = arr##Len;\
  arr[arr##Len] = '\0';
    
//::::::::::::::::::::::: MEMORY MANAGEMENT :::::::::::::::::::::://

  // macro for creating a 1D array of a given type and length
  // Examples:  CREATE_ARRAY(int, myInts, 10);
  //            CREATE_ARRAY(double, myDbls, 20);
  //            CREATE_ARRAY(char, myString, 15);
#define CREATE_ARRAY(type, arr, len) \
  type *arr = safeCalloc(len, sizeof(type))

  // macro for creating a 2D matrix of given type and dimensions
  // Examples:  CREATE_MATRIX(int, myInts, 10, 10);
  //            CREATE_MATRIX(double, myDbls, 10, 15);
  //            CREATE_MATRIX(char, myChrs, 15, 10);
#define CREATE_MATRIX(type, matrix, rows, cols) \
  type **matrix = safeCalloc(rows, sizeof(type *)); \
  for (size_t matrix##i = 0; matrix##i < rows; ++matrix##i) \
    matrix[matrix##i] = safeCalloc(cols, sizeof(type)); \

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

void clearStdin(char *buffer);

//:::::::::::::::::::::::::::: STRINGS ::::::::::::::::::::::::::://

typedef struct {
  char *str;
  size_t size, cap;
} string;

  // macro for reading all text until the end of the file in a string
  // Example:  READ_STRING(myString);
#define READ_STRING(arr) \
  string *arr = safeCalloc(1, sizeof(string)); \
  arr->str = safeCalloc(1000, sizeof(char)); \
  arr->cap = 1000; \
  size_t str##size##Len = 0; char str##size##var; \
  while (scanf("%c", &str##size##var) == 1) { \
    arr->str[str##size##Len++] = str##size##var; \
    if (str##size##Len % 1000 == 0) {\
      arr->str = safeRealloc(arr->str, \
        (str##size##Len + 1000) * sizeof(char)); \
      memset(arr->str + str##size##Len, 0, 1000); \
    } \
  } \
  arr->size = str##size##Len;\

  // shows a string
void showString(string *s);

  // deallocates a string
void freeString(string *s);

#endif // CLIB_H_INCLUDED