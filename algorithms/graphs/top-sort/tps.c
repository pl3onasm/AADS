/*
  file: tps.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: topological sort of a directed acyclic 
               graph using depth-first search
  complexity: Θ(V + E)
  note: make sure to use VERTEX_TYPE3 in the vertex.h file
        by defining it from the command line when compiling:
        $ gcc -D VERTEX_TYPE3 ...
*/

#include "../../../datastructures/graphs/graph/graph.h"

//===================================================================
// Visits the node u and its descendants in depth-first order
void dfsVisit(graph *G, vertex *u, dll *sorted) {
  u->visited = true;              // u is discovered

  dll *edges = getNeighbors(G, u);
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
    if (! e->to->visited)         // undiscovered?
      dfsVisit(G, e->to, sorted);

  dllPush(sorted, u);             // adds u to the list
}

//===================================================================
// Sorts the vertices in the graph G in topological order
// Since the graph is implemented as a hash table, the
// sorting may differ from one run to another as a topological
// sort is not necessarily unique
dll *topSort(graph *G) {
  dll *sorted = dllNew();  
  
    // visit all undiscovered vertices in G
  for (vertex *v = firstV(G); v; v = nextV(G)) 
    if (! v->visited)
      dfsVisit(G, v, sorted);

  return sorted;
}

//===================================================================
// Shows the topological sort
void showTopSort(dll *sorted) {
  printf("TOPOLOGICAL SORT\n"
         "----------------\n");
  for (vertex *v = dllFirst(sorted); v; v = dllNext(sorted))
    printf("%s\n", v->label);
}

//===================================================================

int main (int argc, char *argv[]) {
  
  graph *G = newGraph(40, UNWEIGHTED);
  readGraph(G);  

  dll *sorted = topSort(G);     

  showTopSort(sorted);

  freeGraph(G);
  dllFree(sorted);
  return 0;
}