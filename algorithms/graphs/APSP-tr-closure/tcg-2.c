/* file: tcg-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: transive closure algorithm for the all-pairs
      shortest path problem, alternative version in which
      the algorithm is further adapted to show only the
      presence of a path between two nodes, not its length
      (i.e. the transitive closure of a graph is a binary
      matrix in which the entry (i, j) is 1 if there is a
      path from node i to node j, and 0 otherwise)
   complexity: O(V³)
*/

#include <stdio.h>
#include <stdlib.h>

//::::::::::::::::::::::: memory management :::::::::::::::::::::::://

void *safeCalloc (int n, int size) {
  /* allocates n elements of size size, initializing them to 0, and
     checks whether the allocation was successful */
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
      printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
      exit(EXIT_FAILURE);
  }
  return ptr;
}

void freeM (int **M, int n) {
  /* frees a n x n matrix */
  for (int i = 0; i < n; i++) 
    free(M[i]);
  free(M);
}

//:::::::::::::::::::::::: matrix functions :::::::::::::::::::::::://

int **initT (int n) {
  /* initializes a int matrix with 1's on the diagonal 
     and 0 elsewhere */
  int **T = safeCalloc(n, sizeof(int*));
  for (int i = 0; i < n; i++){
    T[i] = safeCalloc(n, sizeof(int)); 
    for (int j = 0; j < n; j++) 
      T[i][j] = (i == j) ? 1 : 0;
  }
  return T;
}

void printM (int **M, int n) {
  /* prints a n x n distance matrix */
  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++)
      printf("%3d", M[i][j]);
    printf("\n");
  }
}

int **readGraph (int n){
  /* reads a weighted directed graph from stdin 
     and returns its adjacency matrix */
  int **T = initT(n);
  int i, j;
  while (scanf("%d %d", &i, &j) == 2) 
    T[i][j] = 1;
  printf("Input graph:\n");
  printM(T, n);
  return T;
}

//:::::::::::::::::::::: transitive closure :::::::::::::::::::::::://

void closure (int **T, int n) {
  /* computes the transitive closure of a directed graph */
  for (int k = 0; k < n; k++)       // for each intermediate node k
    for (int i = 0; i < n; i++)     // for each pair of nodes (i, j)
      for (int j = 0; j < n; j++) 
        T[i][j] = T[i][j] || (T[i][k] && T[k][j]);
}

//::::::::::::::::::::::::::::: main ::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n;                       // n = number of nodes
  scanf("%d", &n);         
 
  int **T = readGraph(n);      // initialize T and read/print graph

  closure(T, n);               // compute transitive closure

  printf("\nTransitive closure:\n");
  printM(T, n);                

  freeM(T, n);
  return 0;
}
