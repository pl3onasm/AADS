/* file: qselect-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: 
     implementation of quickselect, using a specific pivot to
     partition the array while searching for the k-th smallest
     element in the array. The pivot is chosen as the median
     of the medians of groups of 5 elements.
   time complexity: O(n)
*/

#include <stdio.h>
#include <stdlib.h>

void *safeCalloc (int n) {
  /* checks if memory allocation was successful */
  void *p = calloc(n, sizeof(int));
  if (p == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);}
  return p;
}

int *readArray (int n) {
  /* reads input and stores it in an array */
  int *arr;
  arr = safeCalloc(n);
  for (int i = 0; i < n; ++i) scanf("%d", &arr[i]);
  return arr;
}

void swap (int i, int j, int *arr) {
  /* swaps arr[i] and arr[j] */
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

int partition(int *arr, int left, int right, int pivot) {
  /* partitions arr[left..right] around a given pivot value */
  int idx = left, pivotIdx = 0; 
  for (int i = left; i <= right; i++) {
    // move all elements <= pivot to the low end
    if (arr[i] <= pivot){
      if (arr[i] == pivot) pivotIdx = idx; // save pivot's index
      swap(i, idx++, arr);
    }
  }
  swap(pivotIdx, idx-1, arr); // move pivot to its final place
  return idx-1;
}

int select (int *arr, int left, int right, int k) {
  /* returns the k-th smallest element in arr[left..right] */

  // makes sure that arr's length is divisible by 5
  while ((right - left + 1) % 5) {
    for (int j = left + 1; j <= right; j++) 
      // get the minimum at arr[left]
      if (arr[left] > arr[j]) swap(left, j, arr);
    if (k == 1) 
      return arr[left];
    left++;   // move the left boundary by 1 to the right
    k--;      // update k to reflect the new position 
              // of the k-th smallest element
  }

  // g = total number of 5-element groups
  int g = (right - left + 1) / 5;

  // sort each group in place using insertion sort
  for (int j = left; j < left + g; j++) {
    for (int k = j; k <= right; k += g) {
      int key = arr[k];
      int l = k - g;
      while (l >= j && arr[l] > key) {
        arr[l + g] = arr[l];
        l -= g;
      }
      arr[l + g] = key;
    }
  }

  // find the pivot recursively as the median of the group medians
  int pivot = select(arr, left + 2*g, left + 3*g - 1, g/2 + 1);

  // partition around the median
  int q = partition(arr, left, right, pivot);

  int i = q - left + 1;   // number of elements ≤ median
  if (i == k) return arr[q];
  else if (k < i) return select(arr, left, q - 1, k);
  else return select(arr, q + 1, right, k - i);
}

int main () {
  int n, k;   // n = number of elements, k = k-th order statistic
  scanf("%d %d", &n, &k);
  int *arr = readArray(n);
  printf("%d\n", select(arr, 0, n-1, k));
  free(arr);
  return 0;
}