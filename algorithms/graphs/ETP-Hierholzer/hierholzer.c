/* 
  file: hierholzer.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: Hierholzer's algorithm for finding an Eulerian 
    tour in a graph. The algorithm works for both directed and
    undirected graphs.
  time complexity: O(|E|) where |E| is the number of edges in 
    the graph. 
  note 1: make sure to use VERTEX_TYPE2 in the vertex.h file and 
    EDGE_TYPE3 in the edge.h file by defining them from the  
    command line using
      $ gcc -D VERTEX_TYPE3 -D EDGE_TYPE3 ...
  note 2: tours may differ from one run to another since
    the graph is implemented as a hash table of vertices
*/

#include "../../../datastructures/graphs/graph/graph.h"
#include "../../../lib/clib/clib.h"

//===================================================================
// Hierholzer's algorithm for finding an Eulerian tour in a graph
void hierholzer(graph *G, vertex *v, dll *path) {

  dll *edges = getNeighbors(G, v);
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
    if (!e->taken) {
      e->from = v;
      e->taken = true;
      if (G->type == UNDIRECTED) 
        // avoid taking an undirected edge twice
        e->rev->taken = true;
      hierholzer(G, e->to, path);
      dllPush(path, e);
    }
  }
}

//===================================================================
// Performs a depth-first search starting from vertex v and returns
// the number of reachable vertices
size_t countReachable(graph *G, vertex *v) {
 
  size_t nVisited = 1;
  v->visited = true;

  dll *edges = getNeighbors(G, v);
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
    if (!e->to->visited) 
      nVisited += countReachable(G, e->to);

  return nVisited;
}

//===================================================================
// Checks if the graph is connected
bool isConnected(graph *G) {
  if (!G) return false;
    
    // number of reachable vertices from a random vertex
    // should be equal to the total number of vertices
  return countReachable(G, firstV(G)) == nVertices(G);
}

//===================================================================
// Determines whether an Eulerian path or cycle exists in the graph
// and if so, returns the starting vertex for the path or cycle
// NULL is returned if no Eulerian path or cycle exists
vertex *startVertex(graph *G) {

  if (!isConnected(G)) return NULL;

  size_t nOdd = 0;
  vertex *start = NULL;

  // for a directed graph, the difference between the in-degree
  // and out-degree of each vertex should be 0. The only exception 
  // is: one vertex may have out-degree - in-degree = 1, but then
  // there should be another vertex with in-degree - out-degree = 1
  if (G->type == DIRECTED) {
    for (vertex *v = firstV(G); v; v = nextV(G)) {
      if (outDegree(G, v) - v->inDegree == 1) {
        nOdd++;
        if (nOdd > 2) return NULL;
        start = v;
      }
      if (v->inDegree - outDegree(G, v) == 1) {
        nOdd++;
        if (nOdd > 2) return NULL;
      }
      if (abs(v->inDegree - outDegree(G, v)) > 1) 
        return NULL;
    }
  } else {
    // for an undirected graph, the degree of each vertex should be
    // even except for at most two vertices who may have odd degree
    for (vertex *v = firstV(G); v; v = nextV(G)) {
      if (degree(G, v) & 1) {
        nOdd++;
        if (nOdd > 2) return NULL;
        start = v;
      }
    }
  }
  if (nOdd && nOdd != 2) return NULL;
  return start ? start : firstV(G);
}

//===================================================================
// Displays the Eulerian tour
void showPath(dll *path) {
  if (!path) return;

  printf("\n---------------\n"
         " Eulerian tour\n"
         "---------------\n");

  for (edge *e = dllFirst(path); e; e = dllNext(path)) 
    printf("   %s -- %s\n", e->from->label, e->to->label);
  printf("\n");
}

//===================================================================

int main() {
  
  graph *G = newGraph(50, UNWEIGHTED);

  readGraph(G);
  showGraph(G);

  vertex *src = startVertex(G);

  if (!src) {
    printf("The graph has no Eulerian tour.\n");
    freeGraph(G);
    return 0;
  } 

  dll *path = dllNew();
  
  hierholzer(G, src, path);
  showPath(path);

  dllFree(path);
  freeGraph(G);
  return 0;
}