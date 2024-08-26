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

#include "../../../lib/clib.h"
#include <time.h>

//===================================================================
// Partitions an array around a random pivot element and returns
// the sorted position of this pivot
size_t partition(int *arr, size_t left, size_t right) {
  
  size_t i = left;
    // put random pivot at the end of the array
  SWAP(arr[left + rand() % (right - left)], arr[right - 1]);

    // keep swapping elements smaller or equal to
    // the pivot to the left side of the array
  for (size_t j = left; j < right - 1; j++) 
    if (arr[j] <= arr[right - 1]) 
      SWAP(arr[i++], arr[j]);
  
    // put pivot in its final sorted position
  SWAP(arr[i], arr[right - 1]);
  return i;
}

//===================================================================
// Returns the k-th smallest element in the input array, i.e. the
// element that would be at index k - 1 if the array were sorted
int quickSelect(int *arr, size_t left, size_t right, size_t k) {
  
  if (left + 1 == right) 
    return arr[left];

    // partition the array around a random pivot 
  size_t pivotIdx = partition(arr, left, right);
    // compute the number of elements <= pivot
  size_t i = pivotIdx - left + 1;    
  
  if (k == i) 
      // we found the k-th smallest element
    return arr[pivotIdx];
  else if (k < i)          
      // search in the low end of the array
    return quickSelect(arr, left, pivotIdx, k);
  else                     
      // update the order statistic and
      // search for the (k-i)th element in the high end
    return quickSelect(arr, pivotIdx + 1, right, k - i);
}

//===================================================================

int main () {
  srand(time(NULL)); 
  
  size_t k;
  assert(scanf("%zu", &k) == 1);
  
  READ(int, arr, "%d", len);

  if (k > len) {
    fprintf(stderr, "Error: k is greater than the input length\n");
    free(arr);
    exit(EXIT_FAILURE);
  }

  printf("%d\n", quickSelect(arr, 0, len, k));

  free(arr);
  return 0;
}