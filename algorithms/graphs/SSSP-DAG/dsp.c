/* 
  file: dsp.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: DAG shortest paths algorithm
  time complexity: O(V+E)
  note: make sure to use VERTEX_TYPE4 in the vertex.h file
  by defining it from the command line using
    $ gcc -D VERTEX_TYPE4 ...
*/

#include "../../../datastructures/graphs/vgraph/graph.h"
#include <float.h>
#include <assert.h>

//===================================================================
// Visits the node u and its descendants in depth-first order
void dfsVisit(graph *G, vertex *u, dll *sorted) {

  dll *edges = getNeighbors(G, u);
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
    if (! e->to->parent) {     
      e->to->parent = u;          // first discovery of e->to
      dfsVisit(G, e->to, sorted);
    }

  dllPush(sorted, u);             // adds u to the list
}

//===================================================================
// Sorts the vertices in the graph G in topological order
dll *topSort(graph *G) {
  dll *sorted = dllNew();  
  
    // visit all undiscovered vertices in G
  for (vertex *v = firstV(G); v; v = nextV(G)) 
    if (! v->parent)
      dfsVisit(G, v, sorted);

  return sorted;
}

//===================================================================
// Tries to relax the edge (u, v) with weight w
void relax(vertex *u, vertex *v, double w) {
  /* relaxes the edge (u, v) */
  if (v->dist > u->dist + w) {
    v->dist = u->dist + w;
    v->parent = u;
  }
}

//===================================================================
// Initializes the vertices of the graph G
void initSingleSource(graph *G, vertex *src) {
  for (vertex *v = firstV(G); v; v = nextV(G)) 
    v->dist = DBL_MAX;
  src->dist = 0;
}

//===================================================================
// Computes shortest paths from source vertex to all other vertices
// by relaxing the edges in topological order
void shortestPaths(graph *G, vertex *src) {
  
  initSingleSource(G, src);
  dll *sorted = topSort(G);     
     
  for (vertex *v = dllFirst(sorted); v; v = dllNext(sorted)) {
    dll *edges = getNeighbors(G, v);
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
      relax(v, e->to, e->weight);
  }
  dllFree(sorted);
}

//===================================================================
// Shows the distances and parents of all vertices in the graph
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

int main () {
  
    // read source vertex
  char s[50];
  assert(scanf("%s", s) == 1);

  graph *G = newGraph(50, WEIGHTED);
  readGraph(G);
  showGraph(G);

  vertex *src = getVertex(G, s);
  if (!src) {
    fprintf(stderr, "Vertex %s not found\n", s);
    freeGraph(G);
    exit (EXIT_FAILURE);
  }              

  shortestPaths(G, src);        
  showDistances(G,src);                

  freeGraph(G);
  return 0;
}
