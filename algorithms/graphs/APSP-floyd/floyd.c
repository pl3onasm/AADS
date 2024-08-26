/* 
  file: floyd.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: Floyd-Warshall algorithm for the all-pairs 
    shortest path problem. The program reads a weighted 
    directed graph from stdin and computes the shortest path
    between all pairs of vertices. The algorithm uses a
    distance matrix D and a predecessor matrix P to store
    the shortest path information.
  time complexity: Θ(|V|³)
  space complexity: Θ(|V|²)
*/

#include "../../../datastructures/graphs/amatrix/graph.h"
#include "../../../lib/clib.h"
#include <float.h>

//===================================================================
// Initializes the distance matrix D
double **initD (size_t n, double **W) {
  CREATE_MATRIX(double, D, n, n, 0);
  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++) 
      D[i][j] = (i == j) ? 0 : W[i][j];
  return D;
}

//===================================================================
// Initializes the predecessor matrix P
size_t **initP (size_t n, double **D) {
  CREATE_MATRIX(size_t, P, n, n, 0);
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
// along with the distance between them. The paths are printed in
// the order of the vertices as they appear in the graph when
// printed with the showGraph function
void printAllPaths (graph *G, double **D, size_t **P) {
  printf("--------------------\n"
         " Shortest paths\n"
         "--------------------\n");

  vertex **V = sortVertices(G);
  for (size_t from = 0; from < nVertices(G); from++) 
    for (size_t to = 0; to < nVertices(G); to++) {

      if (from == to) continue;   // skip self-paths
      
      size_t i = V[from]->idx;
      size_t j = V[to]->idx; 

      printf("%s → %s: ", V[from]->label, V[to]->label);
      printf(D[i][j] == DBL_MAX ? "INF" : 
            (D[i][j] == -DBL_MAX) ? "-INF" : "%.2f", D[i][j]);

      if (ABS(D[i][j]) != DBL_MAX) {
        printf("\n  path: ");
        printPath(G, P, i, j);
      }
      printf("\n");
    }
  printf("--------------------\n");
  free(V);
}

//===================================================================
// Computes the shortest path between all pairs of vertices 
// If any path can still be relaxed after a second run with 
// parameter neg set to true, then a negative cycle exists and 
// the paths affected by it are marked with -INF.
void computeAPSP (double **D, size_t **P, size_t n, bool neg) {
    // for each intermediate vertex k
  for (size_t k = 0; k < n; k++)    
      // for each pair of vertices (i, j)
    for (size_t i = 0; i < n; i++)  
      for (size_t j = 0; j < n; j++) 
          // check if paths i⇝k and k⇝j exist
        if (D[i][k] != DBL_MAX && D[k][j] != DBL_MAX) { 

          double newDist = D[i][k] + D[k][j];

            // is i⇝k⇝j shorter than i⇝j?
          if (newDist < D[i][j]) {    
            D[i][j] = neg ? -DBL_MAX : newDist; 
            P[i][j] = neg ? SIZE_MAX : P[k][j];      
          }
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

    // compute the all-pairs shortest paths
  computeAPSP(D, P, n, false);      

    // mark negative cycles by checking if any
    // paths can still be relaxed
  computeAPSP(D, P, n, true);

  printAllPaths(G, D, P);        

  FREE_MATRIX(D, n);           
  FREE_MATRIX(P, n);         
  freeGraph(G);
  return 0;
}
