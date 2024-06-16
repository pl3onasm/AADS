/* 
  file: qselect-2.c
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

#include "../../../lib/clib/clib.h"

size_t partition(int *arr, size_t left, size_t right, int pivot) {
  /* partitions arr[left..right] around a given pivot value */
  size_t idx = left, pivotIdx = 0; 
  for (size_t i = left; i <= right; i++) {
    // move all elements <= pivot to the low end
    if (arr[i] <= pivot){
      if (arr[i] == pivot) pivotIdx = idx; // save pivot's index
      SWAP(arr[i], arr[idx++]);
    }
  }
  SWAP(arr[pivotIdx], arr[idx - 1]);  // move pivot to its final place
  return idx - 1;
}

int select (int *arr, size_t left, size_t right, size_t k) {
  /* returns the k-th smallest element in arr[left..right] */

  // makes sure that arr's length is divisible by 5
  while ((right - left + 1) % 5) {
    for (size_t j = left + 1; j <= right; j++) 
      // get the minimum at arr[left]
      if (arr[left] > arr[j]) SWAP(arr[left], arr[j]);
    if (k == 1) 
      return arr[left];
    left++;   // move the left boundary by 1 to the right
    k--;      // update k to reflect the new position 
              // of the k-th smallest element
  }

  // g = total number of 5-element groups
  size_t g = (right - left + 1) / 5;

  // sort each group in place using insertion sort
  for (size_t j = left; j < left + g; j++) {
    for (size_t m = j + g; m < right; m += g) {
      int key = arr[m];
      size_t l = m;
      while (l > j && arr[l - g] > key) {
        arr[l] = arr[l - g];
        l -= g;
      }
      arr[l] = key;
    }
  }

  // find the pivot recursively as the median of the group medians
  int pivot = select(arr, left + 2*g, left + 3*g - 1, g/2 + 1);

  // partition around the median
  size_t q = partition(arr, left, right, pivot);

  size_t i = q - left + 1;   // number of elements ≤ median
  if (i == k) return arr[q];
  else if (k < i) return select(arr, left, q - 1, k);
  else return select(arr, q + 1, right, k - i);
}

int main () {
  size_t n, k;   // n = number of elements, k = k-th order statistic
  assert(scanf("%zu %zu", &n, &k) == 2);
  
  CREATE_ARRAY(int, arr, n);
  READ_ARRAY(arr, "%d", n);

  printf("%d\n", select(arr, 0, n - 1, k));
  free(arr);
  return 0;
}