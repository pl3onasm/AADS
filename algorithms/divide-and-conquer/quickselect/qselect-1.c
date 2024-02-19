/* file: qselect-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: 
     implementation of quickselect, using a random pivot to
     partition the array while searching for the k-th smallest
     element in the array.
   time complexity: 
     expected O(n), worst case O(n^2)
*/

#include <time.h>
#include "../../../lib/clib/clib.h"

int partition(int *arr, int left, int right) {
  /* partitions arr[left..right] around a random pivot */
  srand(time(NULL));           // seed the random number generator
  int idx = left + rand() % (right - left + 1);
  int pivot = arr[idx];
  SWAP(arr[idx], arr[right]);  // move pivot to the end
  idx = left;                  // start of the partition's high end
  
  // move all elements ≤ pivot to the left
  for (int i = left; i < right; i++)
    if (arr[i] <= pivot)       
      SWAP(arr[i], arr[idx++]);
  
  SWAP(arr[idx], arr[right]);  // move pivot to its final place
  return idx;
}

int quickSelect(int *arr, int left, int right, int k) {
  /* returns the k-th smallest element in arr[left..right], i.e. the
     element that would be at index k if the array were sorted */
  if (left >= right) 
    return arr[left];
  int q = partition(arr, left, right);
  int i = q - left + 1;    // i = number of elements ≤ pivot
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
  int n, k;
  (void)! scanf("%d %d", &n, &k);
  
  CREATE_ARRAY(int, arr, n);
  READ_ARRAY(arr, "%d", n);

  printf("%d\n", quickSelect(arr, 0, n-1, k));
  free(arr);
  return 0;
}