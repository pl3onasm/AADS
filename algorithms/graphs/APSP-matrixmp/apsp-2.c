/* 
  file: apsp-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: bottom-up DP solution to the all-pairs 
    shortest path problem, optimized by repeated squaring
    The program reads a weighted directed graph from stdin
    and and computes the shortest path between all pairs of 
    vertices. 
  time complexity: Θ(n³ log n)
*/

#include "../../../datastructures/graphs/amatrix/graph.h"
#include "../../../lib/clib/clib.h"
#include <float.h>

//===================================================================
// Initializes the distance matrix D
double **initD (size_t n, double **W) {
  CREATE_MATRIX(double, D, n, n);
  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++) 
      D[i][j] = (i == j) ? 0 : (W[i][j] == 0) ? DBL_MAX : W[i][j];
  return D;
}

//===================================================================
// Initializes the predecessor matrix P
size_t **initP (size_t n, double **D) {
  CREATE_MATRIX(size_t, P, n, n);
  for (size_t i = 0; i < n; i++) 
    for (size_t j = 0; j < n; j++) 
      P[i][j] = (i == j || D[i][j] == DBL_MAX) ? SIZE_MAX : i;
  return P;
}

//===================================================================
// Prints the shortest path from vertex with index i to vertex with
// index j
void printPath (graph *G, size_t **P, size_t i, size_t j) {
  if (j == i) printf("%s", G->V[i]->label);
  else {
    printPath(G, P, i, P[i][j]);
    printf(" → %s", G->V[j]->label);
  }
}

//===================================================================
// Prints the shortest path from each vertex to every other vertex
// along with the distance between them
void printAllPaths (graph *G, double **D, size_t **P) {
  printf("--------------------\n"
         " Shortest paths\n"
         "--------------------\n");
  for (size_t i = 0; i < nVertices(G); i++) 
    for (size_t j = 0; j < nVertices(G); j++) {

      if (i == j) continue;   // skip self-paths

      printf("%s → %s: ", G->V[i]->label, G->V[j]->label);
      printf(D[i][j] == DBL_MAX ? "INF" : 
            (D[i][j] == -DBL_MAX) ? "-INF" : "%.2f", D[i][j]);

      if (P[i][j] != SIZE_MAX && abs(D[i][j]) != DBL_MAX) {
        printf("\n  path: ");
        printPath(G, P, i, j);
      }
      printf("\n");
    }
  printf("--------------------\n");
}

//===================================================================
// Tries to extend all paths by r edges
// Negative cycles can be detected by running the function a second
// time with the neg parameter set to true. Paths affected by a 
// negative cycle are marked with -INF
void extendPaths (double **D, size_t **P, size_t n, bool neg) {
  
    // for each path i⇝j
  for (int i = 0; i < n; i++)     
    for (int j = 0; j < n; j++) 
        // try to extend path i⇝j through k
      for (int k = 0; k < n; k++)  
          // check if paths i⇝k and k→j exist
        if (D[i][k] != DBL_MAX && D[k][j] != DBL_MAX) {

            // note we use D[k][j] instead of W[k][j] in apsp-1.c
            // this is where the squaring optimization comes in:
            // we no longer extend paths by one edge at a time
            // but by at most r edges at a time
          double newDist = D[i][k] + D[k][j]; 
          
            // is path i⇝k→j shorter than i⇝j?
          if (newDist < D[i][j]) { 
            D[i][j] = neg ? -DBL_MAX : newDist;
            P[i][j] = neg ? SIZE_MAX : P[k][j];
          }
        }
}

//===================================================================
// Computes the shortest paths between all pairs of vertices
void computeAPSP (double **D, size_t **P, size_t n, bool neg) {
  size_t r = 1;
  while (r < n) {
      // try to extend paths by at most r edges
    extendPaths(D, P, n, neg);   
      // double r 
    r <<= 1;                       
  }
}

//===================================================================

int main () {
  
  graph *G = newGraph(50, WEIGHTED);  
  readGraph(G);          
  showGraph(G);    

  size_t n = nVertices(G);

  double **D = initD(n, G->W);        
  size_t **P = initP(n, D);       

  computeAPSP(D, P, n, false);

    // mark negative cycles by checking if any
    // paths can still be shortened
  computeAPSP(D, P, n, true);       

  printAllPaths(G, D, P);        

  FREE_MATRIX(D, n);           
  FREE_MATRIX(P, n);         
  freeGraph(G);
  
  return 0;
}