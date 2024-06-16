/* 
  file: qselect-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: 
    implementation of quickselect, using a random pivot to
    partition the array while searching for the k-th smallest
    element in the array.
  time complexity: expected O(n), worst case O(n^2)
*/

#include <time.h>
#include "../../../lib/clib/clib.h"

size_t partition(int *arr, size_t left, size_t right) {
  /* partitions arr[left..right] around a random pivot */
  srand(time(NULL));           // seed the random number generator
  size_t idx = left + rand() % (right - left + 1);
  int pivot = arr[idx];
  SWAP(arr[idx], arr[right]);  // move pivot to the end
  idx = left;                  // start of the partition's high end
  
  // move all elements ≤ pivot to the left
  for (size_t i = left; i < right; i++)
    if (arr[i] <= pivot)       
      SWAP(arr[i], arr[idx++]);
  
  SWAP(arr[idx], arr[right]);  // move pivot to its final place
  return idx;
}

int quickSelect(int *arr, size_t left, size_t right, size_t k) {
  /* returns the k-th smallest element in arr[left..right], i.e. the
     element that would be at index k if the array were sorted */
  if (left >= right) 
    return arr[left];
  size_t q = partition(arr, left, right);
  size_t i = q - left + 1;    // i = number of elements ≤ pivot
  if (k == i) 
    return arr[q];
  else if (k < i)          // search in the low end of the array
    return quickSelect(arr, left, q-1, k);
  else                     // search in the high end of the array
    return quickSelect(arr, q+1, right, k-i);
      // we update k to be relative to the new subarray,
      // i.e. we search for the (k-i)th element in the high end
}

int main () {
  size_t n, k;
  assert(scanf("%zu %zu", &n, &k) == 2);
  
  CREATE_ARRAY(int, arr, n);
  READ_ARRAY(arr, "%d", n);

  printf("%d\n", quickSelect(arr, 0, n-1, k));
  free(arr);
  return 0;
}