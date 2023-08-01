/* file: genwhamsort.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: generic WHAM sort,  
     can be used to sort any type of data, including structs,
     as long as a comparison function is provided
   time complexity: O(nlogn) 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b)); 

void *safeMalloc (int n) {
  /* checks if memory has been allocated successfully */
  void *p = malloc(n);
  if (p == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);
  }
  return p;
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

void merge (int left, int invIndex, int right, void *arr, int width, 
int (*comp)(const void *, const void *)) {
  int idx = 0, l = left, r = invIndex;
  void *temp = safeMalloc((right-left)*width);
  while (l < invIndex && r < right) {
    if (comp((char*)arr + l*width, (char*)arr + r*width) < 0 ) 
      memcpy((char*)temp + (idx++)*width, (char*)arr + (l++)*width, width);
    else memcpy((char*)temp + (idx++)*width, (char*)arr + (r++)*width, width);
  }
  memcpy((char*)temp + idx*width, (char*)arr + l*width, (invIndex - l)*width);
  memcpy((char*)temp + idx*width, (char*)arr + r*width, (right - r)*width);
  memcpy((char*)arr + left*width, (char*)temp, (right - left)*width);
  free(temp);
}

int getInvIndex (int left, int right, void *arr, int width, 
int (*comp)(const void*, const void*) ) {
  /* returns the index of the first inversion in arr[left..right] */
  int idx = left + 1;
  while (idx < right && comp((char*)arr + (idx-1)*width, 
    	   (char*)arr + idx*width) <= 0) idx++;
  return idx;
}

void whamSort (void *arr, int left, int right, int width, 
int (*comp)(const void*, const void*)) {
  while (left < right) {
    int invIndex = getInvIndex (left, right, arr, width, comp); 
    if (invIndex >= right) break;
    int newRight = MIN(2*invIndex - left, right);
    whamSort (arr, invIndex, newRight, width, comp);
    merge (left, invIndex, right, arr, width, comp);
  }
}

int main(int argc, char *argv[]) {
  int intExample[] = {5, 6, 7, 8, 9, 10, 2, -1, 3, 4, 1, 2, -35, 78, -10,
                      13, 7, -11, 20, 1, 15, 7, 16, 0, 1, 2, 5, 6, 100, 23};
  char *strExample[] = {"Harry", "Stephen", "Samuel", "SAM", "James",
                        "Thomas", "Robert", "Michael", "William", 
                        "David", "Richard", "Charles", "Joseph", 
                        "John", "Chris", "DANNY", "Dan", "MATT", 
                        "Anthony", "Jonathan", "Mark", "Paul"};
  printf("Unsorted:\n");
  printArray(intExample, 30, 'i');
  whamSort(intExample, 0, 30, sizeof(int), compInt);
  printf("Sorted:\n");
  printArray(intExample, 30, 'i');
  printf("Unsorted:\n");
  printArray(strExample, 22, 's');
  whamSort(strExample, 0, 22, sizeof(char*), compStr);
  printf("Sorted:\n");
  printArray(strExample, 22, 's');
  return 0;
}