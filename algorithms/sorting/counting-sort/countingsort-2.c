/* file: countingsort-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: counting sort
     This is an adaptation so that it also works in cases where
     the array contains negative integers or is more efficient
     if the minimum value is much larger than 0.
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

void printArray (int *arr, int n) {
  /* prints an array of size n */
  printf("[");
  for (int i = 0; i < n; i++) {
    printf("%d", arr[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

int *countingSort(int arr[], int n) {
  /* sorts an array of integers */
  int min = arr[0], max = arr[0], range;
  // find minimum and maximum values
  for (int i = 1; i < n; i++) {
    min = arr[i] < min ? arr[i] : min;
    max = arr[i] > max ? arr[i] : max;
  }
  range = max - min;
  int *count = safeCalloc(range+1, sizeof(int));
  // count the number of occurences of each value
  // subtracting min to make sure that the smallest value is 0
  for (int i = 0; i < n; i++) 
    count[arr[i] - min]++;
  // compute the cumulative sums of the counts
  for (int i = 1; i <= range; i++) 
    count[i] += count[i-1];
  // place the elements in their correct positions
  int *sorted = safeCalloc(n, sizeof(int));
  for (int i = n-1; i >= 0; i--) {
    sorted[count[arr[i] - min] - 1] = arr[i];
    count[arr[i] - min]--;
  }
  free(count);
  return sorted; 
}

int main (int argc, char *argv[]){
  int example[] = {10, -8, -9, 6, -7, 5, -2, 3, -4, 1, 2,
                   -13, 7, -11, 20, -1, 15, -7, -16, -20};
  printf("Unsorted:\n");
  printArray(example, 20);
  int *sorted = countingSort(example, 20);
  printf("Sorted:\n");
  printArray(sorted, 20);
  free(sorted);
  return 0;
}