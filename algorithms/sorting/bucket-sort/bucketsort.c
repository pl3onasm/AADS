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

typedef struct node {
  double value;
  struct node *next;
} node;

void *safeCalloc (int n, int size) {
  /* allocates n elements of given size, initializing them to 0 */
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void printArray (double *arr, int n) {
  /* prints an array of size n */
  printf("[");
  for (int i = 0; i < n; i++) {
    printf("%.3lf", arr[i]);
    if (i < n-1) printf(", ");
  }
  printf("]\n");
}

void insertionSort(int i, node **arr) {
  /* insertion sort for a node list stored in arr[i] */
  node* sorted = NULL;      // head of the sorted list
  node* current = arr[i];   // head of the unsorted list
  while (current != NULL) {
    node* next = current->next;
    // if sorted is empty or value of current is smaller
    if (sorted == NULL || current->value < sorted->value) {
      // prepend current to sorted list
      current->next = sorted;
      sorted = current;
    } else { // else search for correct position to insert current
      node *s = sorted;
      while (s->next != NULL && current->value >= s->next->value)
        s = s->next;
      // insert current between s and s->next
      current->next = s->next;
      s->next = current;
    }
    current = next;
  }
  arr[i] = sorted;
}

void concatenate (node **buckets, double *arr, int n) {
  /* concatenates the lists in the buckets to arr */
  int k = 0;
  for (int i = 0; i < n; i++) {
    node *b = buckets[i];
    while (b != NULL) {
      arr[k++] = b->value;
      node *temp = b;
      b = b->next;
      free(temp);
    }
  }
  free(buckets);
}

void insert (node **buckets, int bucketNo, double value) {
  /* inserts given value in the node list at buckets[bucketNo] */
  node *new = safeCalloc(1, sizeof(node));
  new->value = value;
  if (buckets[bucketNo] == NULL) 
    new->next = NULL;
  else
    new->next = buckets[bucketNo];
  buckets[bucketNo] = new;
}

node **createBuckets (int n, double *arr) {
  /* creates an array of n empty node lists and puts
     the array elements in the corresponding buckets */
  node **buckets = safeCalloc(n, sizeof(node*));
  for (int i = 0; i < n; i++) 
    insert(buckets, (int) n*arr[i], arr[i]);
  return buckets;
}

void bucketSort (double *arr, int n) {
  /* sorts an array of numbers in range [0,1) 
     in expected linear time */
  node **buckets = createBuckets(n, arr);
  for (int i = 0; i < n; i++) 
    // sort each bucket
    insertionSort(i, buckets);
  // concatenate the buckets to form the sorted array
  concatenate(buckets, arr, n);
}

int main (int argc, char *argv[]){
  double example[] = {0.78, 0.312, 0.95, 0.26, 0.72, 
                      0.94, 0.745, 0.05, 0.823, 0.129,
                      0.087, 0.51, 0.918, 0.231, 0.68,
                      0.44, 0.166, 0.04, 0.98, 0.37,
                      0.45, 0.595, 0.89, 0.34, 0.56,
                      0.08, 0.182, 0.94, 0.46, 0.715,
                      0.205, 0.15, 0.05, 0.25, 0.127,
                      0.410, 0.21, 0.121, 0.23, 0.64,
                      0.861, 0.79, 0.621, 0.017, 0.58,
                      0.922, 0.4, 0.589, 0.48, 0.812,
                      0.91, 0.7, 0.63, 0.69, 0.82,
                      0.59, 0.83, 0.53, 0.8, 0.719};
  printf("Unsorted:\n");
  printArray(example, 60);
  bucketSort(example, 60);
  printf("Sorted:\n");
  printArray(example, 60);
  return 0;
}    