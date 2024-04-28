/* 
  file: trc-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: transive closure algorithm, using the 
    Floyd-Warshall algorithm but with a boolean matrix.
*/

#include "../../../datastructures/graphs/amatrix/graph.h"
#include "../../../lib/clib/clib.h"
#include <float.h>
#include <stdbool.h>

//===================================================================
// Initializes the reachability matrix T
bool **initT (graph *G) {
  size_t n = nVertices(G);
  CREATE_MATRIX(bool, T, n, n);
  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++) 
      T[i][j] = G->W[i][j] == DBL_MAX && i != j ? false : true;
  return T;
}

//===================================================================
// Shows the reachability matrix T in sorted order by label, i.e.
// in the same order as the vertices are displayed by
// the showGraph function
void showInOrder (graph *G, bool **T) {
  vertex **vertices = sortVertices(G);

  for (size_t i = 0; i < nVertices(G); i++) {
    for (size_t j = 0; j < nVertices(G); j++) 
      printf("%3d", T[vertices[i]->idx][vertices[j]->idx]);
    printf("\n");
  } 
  free(vertices);
}

//===================================================================
// Shows the reachability matrix T 
void showMatrix (graph *G, bool **T, char *title) {
  printf("\n--------------------\n"
         " %s\n"
         "--------------------\n\n",
          title);
  showInOrder(G, T);
  printf("\n--------------------\n\n");
}

//===================================================================
// Computes the transitive closure of a graph using the 
// Floyd-Warshall algorithm
void computeClosure (bool **T, size_t n) {
  
    // for each intermediate vertex k
  for (size_t k = 0; k < n; k++)       
      // for each pair of vertices (i, j)
    for (size_t i = 0; i < n; i++)     
      for (size_t j = 0; j < n; j++) 
  
        T[i][j] = T[i][j] || (T[i][k] && T[k][j]);
}

//===================================================================

int main () {
 
  graph *G = newGraph(50, UNWEIGHTED);  
  readGraph(G);          
  showGraph(G);    

  bool **T = initT(G);

  showMatrix(G, T, "Initial matrix");

  computeClosure(T, nVertices(G)); 

  showMatrix(G, T, "Transitive closure");

  FREE_MATRIX(T, nVertices(G));      
  freeGraph(G);
  return 0;
}