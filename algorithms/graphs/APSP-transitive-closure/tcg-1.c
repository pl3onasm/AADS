/* 
  file: tcg-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: transive closure algorithm, using the 
    Floyd-Warshall algorithm.
*/

#include "../../../datastructures/graphs/amatrix/graph.h"
#include "../../../lib/clib/clib.h"
#include <float.h>

//===================================================================
// Initializes the distance matrix D
size_t **initD (graph *G) {
  size_t n = nVertices(G);
  CREATE_MATRIX(size_t, D, n, n);
  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++) 
      D[i][j] = G->W[i][j] == DBL_MAX && i != j ? SIZE_MAX : 1;
  return D;
}

//===================================================================
// Prints the distance matrix D in sorted order by label, i.e.
// in the same order as the vertices are shown in the graph by
// the showGraph function
void showInOrder (graph *G, size_t **D) {
  vertex **vertices = sortVertices(G);

  for (size_t i = 0; i < nVertices(G); i++) {
    for (size_t j = 0; j < nVertices(G); j++) {
      size_t d = D[vertices[i]->idx][vertices[j]->idx];
      printf("%4zu", d == SIZE_MAX ? 0 : d);
    }
    printf("\n");
  } 
  free(vertices);
}

//===================================================================
// Shows the initial distance matrix D
void showMatrix (graph *G, size_t **D, char *title) {
  printf("\n--------------------\n"
         " %s\n"
         "--------------------\n\n",
          title);
  showInOrder(G, D);
  printf("\n--------------------\n\n");
}

//===================================================================
// Computes the transitive closure of a graph using the 
// Floyd-Warshall algorithm
void computeClosure (size_t **D, size_t n) {
  
    // for each intermediate vertex k
  for (size_t k = 0; k < n; k++)       
      // for each pair of vertices (i, j)
    for (size_t i = 0; i < n; i++)     
      for (size_t j = 0; j < n; j++) 
          // if path i -> k and path k -> j exist
        if (D[i][k] != SIZE_MAX && D[k][j] != SIZE_MAX) {
          
          size_t d = D[i][k] + D[k][j];

          if (d < D[i][j]) D[i][j] = d;
        }
}

//===================================================================

int main () {
 
  graph *G = newGraph(50, UNWEIGHTED);  
  readGraph(G);          
  showGraph(G);    

  size_t **D = initD(G);

  showMatrix(G, D, "Initial matrix");

  computeClosure(D, nVertices(G));   

  showMatrix(G, D, "Transitive closure");

  FREE_MATRIX(D, nVertices(G));   
  freeGraph(G);
  return 0;
}