/* file: tgc-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: transive closure algorithm for the all-pairs
      shortest path problem, using the Floyd-Warshall algorithm.
      The program reads a directed graph from stdin and prints 
      the transitive closure matrix.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INF INT_MAX

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

int **initD (int n) {
  /* initializes a int matrix with 0's on the diagonal 
     and inf elsewhere */
  int **D = safeCalloc(n, sizeof(int*));
  for (int i = 0; i < n; i++){
    D[i] = safeCalloc(n, sizeof(int)); 
    for (int j = 0; j < n; j++) 
      D[i][j] = (i == j) ? 0 : INF;
  }
  return D;
}

void printD (int **M, int n) {
  /* prints a n x n distance matrix */
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (M[i][j] == INF) printf("%3c", '-');
      else printf("%3d", M[i][j]);
    }
    printf("\n");
  }
}

int **readGraph (int n){
  /* reads a weighted directed graph from stdin 
     and returns its adjacency matrix */
  int **D = initD(n);
  int i, j;
  while (scanf("%d %d", &i, &j) == 2) 
    D[i][j] = 1;
  printf("Input graph:\n"); 
  printD(D, n);
  return D;
}

//:::::::::::::::::::::: transitive closure :::::::::::::::::::::::://

void floydWarshall (int **D, int n) {
  /* computes the transitive closure of a directed graph 
     using the Floyd-Warshall algorithm */
  for (int k = 0; k < n; k++)       // for each intermediate node k
    for (int i = 0; i < n; i++)     // for each pair of nodes (i, j)
      for (int j = 0; j < n; j++) 
        if (D[i][k] != INF && D[k][j] != INF) {
          int d = D[i][k] + D[k][j];
          if (d < D[i][j]) D[i][j] = d;
        }
}

//::::::::::::::::::::::::::::: main ::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n;                       // n = number of nodes
  scanf("%d", &n);         
 
  int **D = readGraph(n);      // read graph and initialize D 

  floydWarshall(D, n);         // run adapted Floyd-Warshall

  printf("\nTransitive closure:\n");
  printD(D, n);                

  freeM(D, n);
  return 0;
}
