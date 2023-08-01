/* file: genmergesort.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: generic merge sort,  
     can be used to sort any type of data, including structs,
     as long as a comparison function is provided
   time complexity: O(nlogn) 
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

void merge(void *arr, int left, int mid, int right, int width, 
  int (*comp)(const void*, const void*)) {
  /* merges two sorted subarrays into one sorted array */
  void *sorted = safeMalloc((right - left + 1) * width);
  int l = left, r = mid + 1, s = 0;   
  // merge while both subarrays are not empty
  while (l <= mid && r <= right) {
    if (comp((char*)arr + l*width, (char*)arr + r*width) < 0) 
      memcpy((char*)sorted + (s++)*width, (char*)arr + (l++)*width, width);
    else memcpy((char*)sorted + (s++)*width, (char*)arr + (r++)*width, width);
  }
  // copy remaining elements from subarrays
  memcpy((char*)sorted + s*width, (char*)arr + l*width, (mid - l + 1)*width);
  memcpy((char*)sorted + s*width, (char*)arr + r*width, (right - r + 1)*width);
  // copy sorted array back to original array
  memcpy((char*)arr + left*width, sorted, (right - left + 1)*width);
  free(sorted);
}

void mergeSort(void *arr, int left, int right, int width, 
int (*comp)(const void*, const void*)) { 
  if (left < right) {
    // divide array in two halves
    int mid = left + (right - left)/2;
    // conquer recursively
    mergeSort(arr, left, mid, width, comp);
    mergeSort(arr, mid + 1, right, width, comp);
    // combine the sorted halves
    merge(arr, left, mid, right, width, comp);
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
  mergeSort(intExample, 0, 19, sizeof(int), compInt);
  printf("Sorted:\n");
  printArray(intExample, 20, 'i');
  printf("Unsorted:\n");
  printArray(strExample, 20, 's');
  mergeSort(strExample, 0, 19, sizeof(char*), compStr);
  printf("Sorted:\n");
  printArray(strExample, 20, 's');
  return 0;
}