/* file: floyd.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Floyd-Warshall algorithm for the all-pairs 
      shortest path problem. The program reads a weighted 
      directed graph from stdin and prints the distance and 
      predecessor matrices. It then reads queries from stdin 
      and prints the requested shortest paths.
   time complexity: Θ(n³)
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

int **initP (int n, double **D) {
  /* initializes a predecessor matrix with -1's if there 
     is no path from i to j, and i otherwise */
  int **P = safeCalloc(n, sizeof(int*));
  for (int i = 0; i < n; i++) {
    P[i] = safeCalloc(n, sizeof(int));
    for (int j = 0; j < n; j++) 
      P[i][j] = (i == j || D[i][j] == INF) ? -1 : i;
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

void floydWarshall (double **D, int **P, int n) {
  /* computes all-pairs shortest paths using Floyd-Warshall */
  for (int k = 0; k < n; k++)       // for each intermediate node k
    for (int i = 0; i < n; i++)     // for each pair of nodes (i, j)
      for (int j = 0; j < n; j++) 
        if (D[i][k] != INF && D[k][j] != INF) {
          // if paths i⇝k and k⇝j exist
          double newDist = D[i][k] + D[k][j];
          if (newDist < D[i][j]) {  // if i⇝k⇝j is shorter than i⇝j
            D[i][j] = newDist;      // "relax" the path i⇝j
            P[i][j] = P[k][j];      // update the predecessor matrix
          }
        }
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
 
  double **D = readGraph(n);   // read graph and initialize D to W
  int **P = initP(n, D);       // initialize predecessor matrix

  floydWarshall(D, P, n);      // compute all-pairs shortest paths

  printD(D, n);                // print distance matrix
  printP(P, n);                // print predecessor matrix
  answerQueries(D, P);         // solve queries

  freeM((void**) D, n);
  freeM((void**) P, n);
  return 0;
}
