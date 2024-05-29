/* file: bfa.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Bellman-Ford algorithm for single-source shortest paths
   time complexity: O(VE)
   note: make sure to use VERTEX_TYPE2 in the vertex.h file
    by defining it from the command line using
      $ gcc -D VERTEX_TYPE2 ...
*/

#include "../../../datastructures/graphs/graph/graph.h"
#include <assert.h>
#include <float.h>

//===================================================================
// Prints the distances and parents of all vertices in the graph
// By following the parent pointers, the shortest path from the
// source node to any other node can be reconstructed
void showDistances(graph *G, vertex *src) {
  printf("\nShortest paths\n"
         "Source: %s\n"
         "---------------------------------\n"
         "Vertex: Parent, Distance from src\n"
         "---------------------------------\n",
         src->label);
         
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    printf("  %s: %s, ", v->label, 
           v->parent ? v->parent->label : "NIL");
    if (v->dist == DBL_MAX)
      printf("%s\n", "INF");
    else
      printf("%.2lf\n", v->dist);
  }
  printf("---------------------------------\n\n");
}

//===================================================================
// Tries to 'relax' the edge (u, v) with weight w
void relax(vertex *u, vertex *v, double w) {
  if (v->dist > u->dist + w) {
    v->dist = u->dist + w;
    v->parent = u;
  }
}

//===================================================================
// Initializes the graph G with the source node s: sets the distance
// of all nodes to the source to infinity and the distance of the
// source node to 0
void initSingleSource(graph *G, vertex *s) {
  for (vertex *v = firstV(G); v; v = nextV(G)) 
    v->dist = DBL_MAX;
  s->dist = 0;
}

//===================================================================
// Runs the Bellman-Ford algorithm on graph G starting from node s
// and checks for negative-weight cycles
void bellmanFord(graph *G, vertex *s) {

  initSingleSource(G, s);          

    // relax all edges |V|-1 times 
  vertex *from;
  for (size_t i = 0; i < nVertices(G) - 1; i++) 
    for (edge *e = firstE(G, &from); e; e = nextE(G, &from))
      relax(from, e->to, e->weight);              

    // check for negative-weight cycles by checking if 
    // any edge can still be relaxed 
  for (edge *e = firstE(G, &from); e; e = nextE(G, &from))      
    if (e->to->dist > from->dist + e->weight) {
      printf("Negative-weight cycle found.\n");
      freeGraph(G);
      exit(EXIT_SUCCESS);
    } 
}

//===================================================================

int main (int argc, char *argv[]) {
    // read the label of the source vertex
  char srcL[50];
  assert(scanf("%s", srcL) == 1);    

  graph *G = newGraph(50, WEIGHTED);  
  readGraph(G); 
  showGraph(G);

  vertex *src = getVertex(G, srcL);

  if (! src) {
    fprintf(stderr, "Source node %s not found.\n", srcL);
    freeGraph(G);
    exit(EXIT_FAILURE);
  }  

  bellmanFord(G, src);                
  showDistances(G, src);        

  freeGraph(G);
  return 0;
}
