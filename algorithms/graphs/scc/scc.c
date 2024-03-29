/* 
  file: scc.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: computes the strongly connected 
    components of a directed graph
  complexity: Θ(V + E)
  note: make sure to use VERTEX_TYPE3 in the vertex.h file
        by defining it from the command line when compiling:
        $ gcc -D VERTEX_TYPE3 ...
*/

#include "../../../datastructures/graphs/vgraph/graph.h"

//===================================================================
// Visits the node u and its descendants in depth-first order
void dfsVisit(graph *G, vertex *u, dll *list) {
  u->visited = true;         // u is discovered

  dll *adj = getNeighbors(G, u);
  for (edge *e = dllFirst(adj); e; e = dllNext(adj)) 
    if (! e->to->visited)    // undiscovered?
      dfsVisit(G, e->to, list);
  
  dllPush(list, u);
}

//===================================================================
// Sorts the vertices in the graph G in topological order
dll *topSort(graph *G) {
  dll *sorted = dllNew();
  
    // visit all undiscovered vertices in G
  for (vertex *v = firstV(G); v; v = nextV(G)) 
    if (! v->visited)
      dfsVisit(G, v, sorted);

  return sorted;
}

//===================================================================
// Builds a depth-first forest for the transposed graph Gᵀ in the
// order given by the list Gs 
// Returns a list of strongly connected components
dll *dfsGt(graph *Gt, dll *Gs) {
  dll *scc = dllNew();
  for (vertex *v = dllFirst(Gs); v; v = dllNext(Gs)) {
    vertex *u = getVertex(Gt, v->label);
    if (! u->visited) {
      dll *component = dllNew();
      dfsVisit(Gt, u, component);  
      dllPush(scc, component);        
    }
  }
  return scc;
}

//===================================================================
// Decomposes the graph into strongly connected components
// using the topological sort Gs on G to guide the DFS on Gᵀ
void decompose(graph *Gt, dll *Gs) {
  printf("\n\nStrongly connected components\n"
          "-----------------------------\n");
  dll *scc = dfsGt(Gt, Gs);
  size_t i = 0;
  for (dll *comp = dllFirst(scc); comp; comp = dllNext(scc)) {
    printf("%zu: {", ++i);
    for (vertex *v = dllFirst(comp); v; v = dllNext(comp))
      printf(dllEnd(comp) ? "%s}\n" : "%s, ", v->label);
    dllFree(comp);
  }
  dllFree(scc);
}

//===================================================================

int main (int argc, char *argv[]) {

  graph *G = newGraph(40, UNWEIGHTED); 
  readGraph(G);
  showGraph(G);

    // create a new graph Gᵀ: 
    // same vertices as G, but reversed edges
  graph *Gt = transposeGraph(G);  

    // get the topological sort of G
  dll *Gs = topSort(G);    

    // decompose G into strongly connected components
    // and show the components
  decompose(Gt, Gs);        

  freeGraph(G);
  freeGraph(Gt);
  dllFree(Gs);
  return 0;
}