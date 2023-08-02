/* file: countingsort-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: counting sort
     This version can only be used to sort arrays
     containing non-negative integers.
   time complexity: O(n) provided that k = O(n)
*/

#include <stdlib.h>
#include <stdio.h>

void *safeCalloc (int n, int size) {
  /* allocates n elements of size size, initializing them to 0, and
     checks whether the allocation was successful */
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void printArray (int arr[], int n) {
  /* prints an array of size n */
  printf("[");
  for (int i = 0; i < n; i++) {
    printf("%d", (arr)[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

int *countingSort(int arr[], int n) {
  /* sorts an array of non-negative integers */
  int max = arr[0];
  // find maximum value k
  for (int i = 1; i < n; i++) 
    if (arr[i] > max) max = arr[i];
  // count the number of occurences of each value
  int *count = safeCalloc(max+1, sizeof(int));
  for (int i = 0; i < n; i++) count[arr[i]]++;
  // compute the cumulative sums of the counts
  for (int i = 1; i <= max; i++) count[i] += count[i-1];
  // place the elements in their correct positions
  int *sorted = safeCalloc(n, sizeof(int));
  for (int i = n-1; i >= 0; i--) {
    sorted[count[arr[i]]-1] = arr[i];
    count[arr[i]]--;
  }
  free(count);
  return sorted;
}

int main (int argc, char *argv[]){
  int example[] = {10, 8, 9, 6, 7, 5, 2, 3, 4, 1, 2,
                   13, 7, 11, 20, 0, 15, 7, 16, 18};
  printf("Unsorted:\n");
  printArray(example, 20);
  int *sorted = countingSort(example, 20);
  printf("Sorted:\n");
  printArray(sorted, 20);
  free(sorted);
  return 0;
}