/* file: apsp-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: bottom-up slow solution to the all-pairs 
      shortest path problem
      The program reads a weighted directed graph from stdin
      and prints the distance and predecessor matrices.
      It then reads queries from stdin and prints the
      requested shortest paths.
   time complexity: Θ(n⁴)
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#define INF DBL_MAX

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

void freeM (void **M, int n) {
  /* frees a generic matrix */
  for (int i = 0; i < n; i++) 
    free(M[i]);
  free(M);
}

//:::::::::::::::::::::::: matrix functions :::::::::::::::::::::::://

double **initD (int n) {
  /* initializes a double matrix with 0's on the diagonal 
     and inf elsewhere */
  double **D = safeCalloc(n, sizeof(double*));
  for (int i = 0; i < n; i++){
    D[i] = safeCalloc(n, sizeof(double)); 
    for (int j = 0; j < n; j++) 
      D[i][j] = (i == j) ? 0 : INF;
  }
  return D;
}

int **initP (int n) {
  /* initializes a predecessor matrix with -1's */
  int **P = safeCalloc(n, sizeof(int*));
  for (int i = 0; i < n; i++) {
    P[i] = safeCalloc(n, sizeof(int));
    for (int j = 0; j < n; j++) 
      P[i][j] = -1;
  }
  return P;
}

double **readGraph (int n){
  /* reads a weighted directed graph from stdin 
     and returns its adjacency matrix */
  double **W = initD(n);
  int i, j; double w;
  while (scanf("%d %d %lf", &i, &j, &w) == 3) 
    W[i][j] = w;
  return W;
}

//:::::::::::::::::::::::: print functions ::::::::::::::::::::::::://

void printD (double **M, int n) {
  /* prints a n x n distance matrix */
  printf("Distance matrix D:\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (M[i][j] == INF) printf("%5s ", "inf");
      else printf("%5.1lf ", M[i][j]);
    }
    printf("\n");
  }
}

void printP(int **P, int n) {
  /* prints a n x n predecessor matrix */
  printf("\nPredecessor matrix P:\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (P[i][j] == -1) printf("%3c ", '-');
      else printf("%3d ", P[i][j]);
    }
    printf("\n");
  }
}

void printPath (int **P, int i, int j) {
  /* prints the shortest path from i to j */
  if (j == i) printf("%d", i);
  else {
    printPath(P, i, P[i][j]);
    printf(" → %d", j);
  }
}

//::::::::::::::::::::::::: apsp functions ::::::::::::::::::::::::://

void extendPaths (double **W, double **D,  
  int **P, int n) {
  /* tries to extend all paths by 1 edge */
  for (int i = 0; i < n; i++) 
    for (int j = 0; j < n; j++) 
      for (int k = 0; k < n; k++)  // try to extend path i⇝j through k
        if (D[i][k] != INF || W[k][j] != INF) { 
          // if paths i⇝k and k→j exist
          double newDist = D[i][k] + W[k][j];
          if (newDist < D[i][j]) { // if path i⇝k→j is shorter
            D[i][j] = newDist;     // "relax" the path i⇝j
            P[i][j] = k;           // update predecessor matrix
          }
        }
}

void apsp (double **W, double **D, int **P, int n) {
  /* computes the all-pairs shortest paths */
  for (int r = 1; r < n; ++r)      // extend paths by 1 edge at a time 
    extendPaths(W, D, P, n);       // to a maximum of n-1 edges                 
}

void answerQueries (double **D, int **P) {
  /* prints the queries and their answers */
  scanf("%*s");   // skip "queries" in input     
  int s = 0, g = 0, q = 0;
  printf("\nQuery results:\n");
  while (scanf("%d %d", &s, &g) == 2) {
    printf("%3d: ", ++q); 
    if (D[s][g] == INF) 
      printf("There is no path from %d to %d.", s, g);
    else {
      printf("Shortest path from %d to %d has length = %4.2lf\n" 
             "     Path: ", s, g, D[s][g]);
      printPath(P, s, g);
    }
    printf("\n");
  }
}

//::::::::::::::::::::::::::::: main ::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n;                       // n = number of nodes
  scanf("%*s %d", &n);         // skip "graph" and read n
 
  double **D = initD(n);       // initialize distance matrix
  int **P = initP(n);          // initialize predecessor matrix
  double **W = readGraph(n);   // read adjacency matrix

  apsp(W, D, P, n);            // compute all-pairs shortest paths

  printD(D, n);                // print distance matrix
  printP(P, n);                // print predecessor matrix
  answerQueries(D, P);         // solve queries

  freeM((void**) W, n);        // free memory
  freeM((void**) D, n);
  freeM((void**) P, n);
  return 0;
}
