/* 
  file: apsp-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: bottom-up slow solution to the all-pairs 
    shortest path problem
    The program reads a weighted directed graph from stdin
    and and computes the shortest path between all pairs of 
    vertices. 
  time complexity: Θ(n⁴)
*/

#include "../../../datastructures/graphs/amatrix/graph.h"
#include "../../../lib/clib/clib.h"
#include <float.h>

//===================================================================
// Creates and initializes the distance matrix D: all entries are
// DBL_MAX except for the main diagonal, which is 0
double **initD (size_t n) {
  CREATE_MATRIX(double, D, n, n);
  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++) 
      D[i][j] = (i == j) ? 0 : DBL_MAX;
  return D;
}

//===================================================================
// Creates and initializes the predecessor matrix P: all entries are
// SIZE_MAX
size_t **initP (size_t n) {
  CREATE_MATRIX(size_t, P, n, n);
  for (size_t i = 0; i < n; i++) 
    for (size_t j = 0; j < n; j++) 
      P[i][j] = SIZE_MAX;
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
// Tries to extend all paths by 1 edge; if a shorter path is found,
// the distance matrix D and the predecessor matrix P are updated
// Negative cycles can be detected by running the function a second
// time with the neg parameter set to true. Paths affected by a 
// negative cycle are marked with -INF
void extendPaths (double **W, double **D,  
                  size_t **P, size_t n, bool neg) {
  
    // for each pair of vertices (i, j)
  for (size_t i = 0; i < n; i++) 
    for (size_t j = 0; j < n; j++) 
        // try to extend path i⇝j through k
      for (size_t k = 0; k < n; k++)  
          // check if paths i⇝k and k→j exist
        if (D[i][k] != DBL_MAX && W[k][j] != DBL_MAX) {
          
          double newDist = D[i][k] + W[k][j];

            // is path i⇝k→j shorter than i⇝j?
          if (newDist < D[i][j]) {  
            D[i][j] = neg ? -DBL_MAX : newDist;     
            P[i][j] = neg ? SIZE_MAX : k;
          }
        }
}

//===================================================================
// Computes the shortest paths between all pairs of vertices
void computeAPSP (double **W, double **D, size_t **P, 
                  size_t n, bool neg) {
  
    // extend paths by 1 edge at a time to a maximum of n-1 edges
  for (size_t r = 1; r < n; ++r)       
    extendPaths(W, D, P, n, neg);             
}

//===================================================================

int main (int argc, char *argv[]) {
  
 graph *G = newGraph(50, WEIGHTED);  
  readGraph(G);          
  showGraph(G);    

  size_t n = nVertices(G);

  double **D = initD(n);         
  size_t **P = initP(n);       

  computeAPSP(G->W, D, P, n, false);

    // mark negative cycles by checking if any
    // paths can still be shortened
  computeAPSP(G->W, D, P, n, true);       

  printAllPaths(G, D, P);        

  FREE_MATRIX(D, n);           
  FREE_MATRIX(P, n);         
  freeGraph(G);
  
  return 0;
}
