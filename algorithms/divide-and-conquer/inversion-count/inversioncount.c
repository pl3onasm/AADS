/* file: inversioncount.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: 
     We want to find the number of inversions in the array. 
     For this we simply sort the array with mergesort, and count the
     number of inversions while merging. This approach gives a  
     solution in O(nlogn), since it's just mergesort with a counter.
*/

#include "../../../lib/clib/clib.h"

int merge(int *arr, int left, int mid, int right) {
  /* merges two sorted arrays, and counts the number of inversions */
  CREATE_ARRAY(int, temp, right - left + 1);
  int l = left, r = mid + 1, t = 0, count = 0;   
  while (l <= mid && r <= right) {
    if (arr[l] <= arr[r]){ 
      // no inversions in this case 
      temp[t++] = arr[l++];
    } else {
      // total number of inversions to add is the number of
      // elements currently left in the left half 
      temp[t++] = arr[r++];
      count += mid - l + 1;
    }
  }
  while (l <= mid) temp[t++] = arr[l++]; 
  while (r <= right) temp[t++] = arr[r++];
  for (int i = left; i <= right; i++) arr[i] = temp[i - left];
  free(temp);
  return count;
}

int inversionCount(int *arr, int left, int right) { 
  /* uses mergesort to count the number of inversions 
     in the array */
  int count = 0; 
  if (left < right) {
    int mid = left + (right - left)/2;
    count += inversionCount(arr, left, mid);
    count += inversionCount(arr, mid + 1, right);
    count += merge(arr, left, mid, right);
  }
  return count;
}

int main(int argc, char **argv){
  size_t size; 
  (void)! scanf("%lu\n", &size);
  
  CREATE_ARRAY(int, vect, size);
  READ_ARRAY(vect, "%d ", size);

  printf("%d\n", inversionCount(vect, 0, size - 1));
  free(vect); 
  return 0; 
}