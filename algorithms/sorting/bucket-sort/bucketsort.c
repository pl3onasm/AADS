/* file: bucketsort.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: bucket sort, using a singly linked list
     to store the elements in each bucket, and insertion 
     sort to sort the elements within each bucket
   time complexity: O(n) provided that the elements are 
     uniformly distributed over the interval [0, 1)
*/

#include <stdlib.h>
#include <stdio.h>

typedef struct Node *List; 

typedef struct Node {
    float value;
    List next;
} node;

void *safeMalloc (int n) {
  /* allocates n bytes of memory and checks whether the allocation
     was successful */
  void *ptr = malloc(n);
  if (ptr == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void printArray (float *arr, int n) {
  /* prints an array of size n */
  printf("[");
  for (int i = 0; i < n; i++) {
    printf("%.3f", arr[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

void insertionSort(int i, List *arr) {
  /* insertion sort for a singly linked list */
  List sorted = NULL; 
  List current = arr[i];
  while (current != NULL) {
    List next = current->next;
    if (sorted == NULL || sorted->value >= current->value) {
      current->next = sorted;
      sorted = current;
    } else {
      List tmp = sorted;
      while (tmp->next != NULL && tmp->next->value < current->value) {
        tmp = tmp->next;
      }
      current->next = tmp->next;
      tmp->next = current;
    }
    current = next;
  }
  arr[i] = sorted;
}

void insert (List *buckets, int bucket, float value) {
  /* inserts value value in the list stored 
     in buckets[bucket] */
  List new = safeMalloc(sizeof(node));
  new->value = value;
  if (buckets[bucket] == NULL) {
    buckets[bucket] = new;
    new->next = NULL;
  } else {
    new->next = buckets[bucket];
    buckets[bucket] = new;
  }
}

void concatenate (List *buckets, float *arr, int n) {
  /* concatenates the lists in the buckets to arr */
  int k = 0;
  for (int i = 0; i < n; i++) {
    List list = buckets[i];
    while (list != NULL) {
      arr[k++] = list->value;
      List temp = list;
      list = list->next;
      free(temp);
    }
  }
  free(buckets);
}

List *createBuckets (int n, float *arr) {
  /* creates an array of n empty lists and puts the elements
     of arr in the corresponding bucket */
  List *buckets = safeMalloc(n * sizeof(List));
  for (int i = 0; i < n; i++) buckets[i] = NULL;
  for (int i = 0; i < n; i++) {
    int bucket = (int) (arr[i] * n);
    insert(buckets, bucket, arr[i]);
  }
  return buckets;
}

void bucketSort (float *arr, int n) {
  /* sorts an array of floats in range [0,1) 
     in expected linear time */
  List *buckets = createBuckets(n, arr);
  for (int i = 0; i < n; i++) 
    insertionSort(i, buckets);
  concatenate(buckets, arr, n);
}

int main (int argc, char *argv[]){
  float example[] = {0.78, 0.17, 0.95, 0.26, 0.72, 
                     0.94, 0.15, 0.05, 0.2, 0.129,
                     0.087, 0.21, 0.12, 0.231, 0.68,
                     0.44, 0.166, 0.04, 0.98, 0.37,
                     0.45, 0.67, 0.89, 0.34, 0.56,
                     0.08, 0.1, 0.94, 0.46, 0.715,
                     0.205, 0.15, 0.05, 0.25, 0.127,
                     0.120, 0.21, 0.121, 0.23, 0.64};
  printf("Unsorted:\n");
  printArray(example, 40);
  bucketSort(example, 40);
  printf("Sorted:\n");
  printArray(example, 40);
  return 0;
}    