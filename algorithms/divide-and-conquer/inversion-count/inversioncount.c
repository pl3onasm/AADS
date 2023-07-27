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

#include <stdio.h>
#include <stdlib.h>

void *safeMalloc (int n) {
  void *ptr = malloc(n);
  if (ptr == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

int merge(int *arr, int left, int mid, int right) {
  int *temp = safeMalloc((right - left + 1)*sizeof(int));
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

int *readIntVector (int size) {
  int *vect = safeMalloc(size * sizeof(int));
  for (int i = 0; i < size; i++)
    scanf("%d", vect + i);
  return vect;
}

int main(int argc, char **argv){
  int size; 
  scanf("%d\n", &size);
  int *vect = readIntVector(size); 
  printf("%d\n", inversionCount(vect, 0, size - 1));
  free(vect); 
  return 0; 
}