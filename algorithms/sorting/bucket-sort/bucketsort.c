/* 
  file: bucketsort.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: bucket sort, sorting an array of numbers  
    uniformly distributed in [0,1) 
  time complexity: O(n) provided that the elements are 
    uniformly distributed over the interval [0, 1)
*/

#include "../../../datastructures/lists/dllist/dll.h"
#include "../../../lib/clib/clib.h"
#include <math.h>

//===================================================================
// Comparison function for double values avoiding precision errors
int compDoubles (void const *a, void const *b) {
  if (fabs(*(double **)a - *(double **)b) < 0.000001) return 0;
  return *(double **)a - *(double **)b > 0 ? 1 : -1;
}

//===================================================================
// Puts each element of the array in the appropriate bucket 
// and inserts it in sorted order inside the bucket
void sortBuckets (dll **buckets, double *arr, size_t len) {
  
  for (size_t i = 0; i < len; i++) 
    dllInsert(buckets[(size_t)(len * arr[i])], &arr[i]);
}

//===================================================================
// Creates an array of doubly linked lists, one for each bucket
dll **createBuckets (double *arr, size_t len) {
  
  dll **buckets = safeCalloc(len, sizeof(dll *));
  
  for (size_t i = 0; i < len; i++) {
    buckets[i] = dllNew();
    dllSetCmp(buckets[i], compDoubles);
  }
  return buckets;
}

//===================================================================
// Concatenates the buckets to form the sorted array
double *concatenate (dll **buckets, double *arr, size_t len) {
  
  double *sorted = safeCalloc(len, sizeof(double));
  size_t s = 0;
  
  for (size_t i = 0; i < len; i++) {
    dll *bucket = buckets[i];
    for (double *num = dllFirst(bucket); num; num = dllNext(bucket)) 
      sorted[s++] = *num;
    dllFree(buckets[i]);
  }
  free(buckets);
  return sorted;
}

//===================================================================
// Sorts an array of numbers uniformly distributed in [0,1) in
// expected linear time
double *bucketSort (double *arr, size_t len) {
  
    // create n buckets 
  dll **buckets = createBuckets(arr, len);
  
    // fill the buckets with the elements of the array
    // sorted in ascending order
  sortBuckets(buckets, arr, len);

    // concatenate the buckets to return the sorted array
  return concatenate(buckets, arr, len);
}

//===================================================================

int main (){
    
  READ(double, arr, "%lf", len);

  double *sorted = bucketSort(arr, len);

  PRINT_ARRAY(sorted, "%lf", len);

  free(arr);
  free(sorted);
  
  return 0;
}    