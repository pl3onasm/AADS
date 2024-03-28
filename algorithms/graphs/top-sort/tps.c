/*
  file: tps.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: topological sort of a directed acyclic 
               graph using depth-first search
  complexity: Θ(V + E)
  note: make sure to use VERTEX_TYPE1 in the vertex.h file
        and EDGE_TYPE1 in the edge.h file by defining them
        from the command line when compiling:
        $ gcc -D VERTEX_TYPE1 -D EDGE_TYPE1 ...
*/

#include "../../../datastructures/graphs/vgraph/graph.h"

//===================================================================
// Visits the node u and its descendants in depth-first order
void dfsVisit(graph *G, vertex *u, dll *sorted, size_t *time) {
  u->dTime = ++*time;
  u->color = GRAY;                // u is discovered: turns gray

  dll *edges = getNeighbors(G, u);
  for (edge *e = firstE(edges); e; e = nextE(edges)) {
    if (e->to->color == WHITE) {  // undiscovered?
      e->to->parent = u;
      dfsVisit(G, e->to, sorted, time);
    } 
  }
  u->fTime = ++*time;
  dllPush(sorted, u);
}

//===================================================================
// Builds a depth-first forest for the graph G
void dfs(graph *G, dll *sorted, size_t *time) {

  for (vertex *v = firstV(G); v; v = nextV(G)) {
    if (v->color == WHITE)
      dfsVisit(G, v, sorted, time);
  }
}

//===================================================================
// Sorts the vertices in the graph G in topological order
dll *topSort(graph *G) {
  dll *sorted = dllNew();       // stores the topological sort
  size_t time = 0;     
  dfs(G, sorted, &time);
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