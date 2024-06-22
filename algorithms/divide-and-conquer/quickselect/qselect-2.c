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
#include <math.h>

//===================================================================
// Partitions the array around a given pivot value and returns
// the sorted position of this pivot
size_t partition(int *arr, size_t left, size_t right, int pivot) {
  
  size_t i = left, pivotIdx = 0; 

    // keep swapping elements smaller or equal to 
    // the pivot to the left side of the array
  for (size_t j = left; j < right; j++) 
    if (arr[j] <= pivot) {
      if (arr[j] == pivot) 
        pivotIdx = i; 
      SWAP(arr[j], arr[i++]);
    }
    
    // put pivot in its final sorted position
  SWAP(arr[i - 1], arr[pivotIdx]); 
  return i - 1;
}

//===================================================================
// Trims the array by extracting the minima of the array and moving 
// them to the leftmost positions until the array length is
// divisible by 5 or the order statistic k is found.
// The function returns the new left index of the trimmed array 
// and updates the value of k accordingly
size_t trimArray(int *arr, size_t left, size_t right, size_t *k) {
  
  while ((right - left) % 5) {
    for (size_t i = left + 1; i < right; i++) 
      if (arr[left] > arr[i]) 
        SWAP(arr[left], arr[i]);
    if (*k == 1) 
        // we found the k-th smallest element
      return left;
    ++left;
    --*k;
  }
  return left;
}

//===================================================================
// Sorts the groups of 5 elements in the array using insertion sort;
// g is the total number of such groups in the array and is also the
// distance between the elements in each group
void sortGroups(int *arr, size_t left, size_t right, size_t g) {

  for (size_t i = left; i < left + g; i++) {
      // sort the next group of 5 elements
    for (size_t j = i + g; j < right; j += g) {
      int key = arr[j];
      size_t k = j;
        // shift elements of the sorted subarray within the group 
        // by distance g to the right until the correct position 
        // for key is found
      while (k > i && arr[k - g] > key) {
        arr[k] = arr[k - g];
        k -= g;
      }
        // insert key into its sorted position
      arr[k] = key;
    }
  }
}

//===================================================================
// Returns the k-th smallest element in the input array, i.e. the
// element that would be at index k - 1 if the array were sorted
int quickSelect (int *arr, size_t left, size_t right, size_t k) {
  
    // trim the array to have a length divisible by 5
  left = trimArray(arr, left, right, &k);
  if ((right - left) % 5)
    return arr[left];

    // compute the number of 5-element groups and sort them
  size_t g = (right - left) / 5;
  sortGroups(arr, left, right, g);

    // find the pivot recursively as the median of the group medians
  int pivot = quickSelect(arr, left + 2*g, left + 3*g, ceil(g/ 2.0));

    // partition around the median
  size_t pivotIdx = partition(arr, left, right, pivot);

    // compute the number of elements <= median
  size_t i = pivotIdx - left + 1;   

  if (i == k) 
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