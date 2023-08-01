/* file: genquicksort.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: generic quick sort, can be used to sort any
     type of data, including structs, as long as a comparison
     function is provided
   time complexity: worst case O(n^2), average case O(nlogn)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

void printArray (void *arr, int n, char type) {
  /* prints an array of size n */
  printf("[");
  for (int i = 0; i < n; i++) {
    if (type == 'i') printf("%d", ((int *)arr)[i]);
    else printf("%s", ((char **)arr)[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

int compInt (const void *a, const void *b) {
  /* compares two integers */
  return (*(int*)a - *(int*)b);
}

int strcmpi (const char *a, const char *b) {
  /* case-insensitive string comparison */
  while (*a && *b) {
    if (tolower(*a) != tolower(*b)) 
      return tolower(*a) - tolower(*b);
    a++; b++;
  }
  return tolower(*a) - tolower(*b);
}

int compStr (const void *a, const void *b) {
  /* compares two strings */
  return strcmpi(*(char**)a, *(char**)b);
}

void swap(void *arr, int i, int j, int width) {
  /* swaps two elements with indices i and j in arr */
  void *temp = safeMalloc(width);
  memcpy(temp, (char*)arr + i*width, width);
  memcpy((char*)arr + i*width, (char*)arr + j*width, width);
  memcpy((char*)arr + j*width, temp, width);
  free(temp);
}

int partition(void *arr, int left, int right, int width, 
  int (*comp)(const void*, const void*)) {
  /* partitions the array into two parts, one with elements
     smaller than the pivot, one with elements larger than
     the pivot */
  int pivot = right;
  int i = left;
  for (int j = left; j < right; j++) {
    if (comp((char*)arr + j*width, (char*)arr + pivot*width) < 0) {
      swap(arr, i, j, width);
      i++;
    }
  }
  swap(arr, i, pivot, width);
  return i;
}

void quickSort(void *arr, int left, int right, int width,
  int (*compare)(const void *, const void *)){
  /* sorts the array using quick sort */
  if (left < right) {
    // partition the array into two parts
    int q = partition(arr, left, right, width, compare);
    // recursively sort the two parts
    quickSort(arr, left, q-1, width, compare);
    quickSort(arr, q+1, right, width, compare);
  }
}

int main (int argc, char *argv[]){
  int intExample[] = {10, 8, -9, 6, 7, 5, 2, 3, 4, -1, 2,
                      13, 7, 11, 20, 1, -15, 7, 16, 0};
  char *strExample[] = {"Harry", "Stephen", "Samuel", "James", 
                        "Chris", "Robert", "Michael", "William", 
                        "David", "Richard", "Charles", "Joseph", 
                        "Thomas", "John", "Daniel", "Matthew", 
                        "Anthony", "Jonathan", "Mark", "Paul"};
  printf("Unsorted:\n");
  printArray(intExample, 20, 'i');
  quickSort(intExample, 0, 19, sizeof(int), compInt);
  printf("Sorted:\n");
  printArray(intExample, 20, 'i');
  printf("Unsorted:\n");
  printArray(strExample, 20, 's');
  quickSort(strExample, 0, 19, sizeof(char*), compStr);
  printf("Sorted:\n");
  printArray(strExample, 20, 's');
  return 0;
}
