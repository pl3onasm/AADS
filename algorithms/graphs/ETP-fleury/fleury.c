/* 
  file: fleury.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: Fleury's algorithm for finding an Eulerian 
    tour in a graph. The algorithm works for both directed and
    undirected graphs. 
  time complexity: O(E^2) where E is the number of edges in the
    graph. The algorithm is not efficient for dense graphs.
  note 1: make sure to use VERTEX_TYPE3 in the vertex.h file and 
    EDGE_TYPE3 in the edge.h file by defining them from the  
    command line using
      $ gcc -D VERTEX_TYPE3 -D EDGE_TYPE3 ...
  note 2: tours may differ from one run to another since
    the graph is implemented as a hash table of vertices
*/

#include "../../../datastructures/graphs/graph/graph.h"
#include "../../../lib/clib/clib.h"

//===================================================================
// Makes a copy of an edge; used to make the dll store copies of the
// edges in the Eulerian tour and needed since edges are deleted
void *copyEdge(void const *data) {
  edge *e = (edge *)data;
  if (!e) return NULL;

  edge *copy = safeCalloc(1, sizeof(edge));
  copy->from = e->from;
  copy->to = e->to;

  return copy;
}

//===================================================================
// Performs a depth-first search starting from vertex v and returns
// the number of reachable vertices
size_t countReachable(graph *G, vertex *v) {
 
  size_t nVisited = 1;
  v->visited = true;

  dll *edges = getNeighbors(G, v);
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
    if (!e->to->visited && !e->taken)
      nVisited += countReachable(G, e->to);

  return nVisited;
}

//===================================================================
// Checks if the edge to vertex v is a bridge by performing a single
// dfs call over the remaining vertices in the graph: if the count
// of reachable vertices from v is less than the total number of
// remaining vertices in the graph, then the edge is a bridge
bool isBridge(graph *G, vertex *v, size_t remV) {

    // mark all vertices as unvisited before calling dfs
  for (vertex *u = firstV(G); u; u = nextV(G))
    u->visited = false;
  
  return countReachable(G, v) < remV;
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
// Finds the first non-bridge edge in vertex v's adjacency list;
// if all edges are bridges, NULL is returned
edge *findEdge(graph *G, vertex *v, size_t remV) {
  
  dll *edges = getNeighbors(G, v);
  edge *e = NULL;

  for (e = dllFirst(edges); e; e = dllNext(edges)) {
      // block the reverse edge if the graph is undirected so as
      // to obtain the correct number of reachable vertices when
      // calling countReachable
    if (G->type == UNDIRECTED)
      e->rev->taken = true;

    bool bridge = isBridge(G, e->to, remV);

    if (G->type == UNDIRECTED)
      e->rev->taken = false;

      // take the first non-bridge edge found
    if (!bridge) break;
  }
  return e;
}

//===================================================================
// Finds an Eulerian path or cycle in a graph
void fleury(graph *G, size_t remV, vertex *v, dll *path) {

    // while there are still reachable vertices
  while (remV > 1) {
    dll *edges = getNeighbors(G, v);
    edge *e = NULL;
    
    if ((G->type == DIRECTED && v->inDegree == 0) ||  // (1)
        (G->type == UNDIRECTED && v->inDegree == 1))  // (2)
        // update the number of remaining vertices since either:
        // (1) v has become unreachable in the previous step, or
        // (2) will become unreachable in the current step              
      remV--;                 
    else 
      e = findEdge(G, v, remV);
    
      // if v has only one edge, or if all edges are bridges
      // then take the first edge in the adjacency list
    if (!e) e = dllFirst(edges);

    e->from = v;
    vertex *u = e->to;

    dllPushBack(path, e);
    delEdge(G, v, u);
    v = u;
  }
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
      if (outDegree (G, v) - v->inDegree == 1) {
        nOdd++;
        start = v;
      }
      if (v->inDegree - outDegree (G, v) == 1) 
        nOdd++;
      if (abs(v->inDegree - outDegree (G, v)) > 1) 
        return NULL;
      if (nOdd > 2) return NULL;
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
  dllCopyData(path, copyEdge, free);
  
  fleury(G, nVertices(G), src, path);
  showPath(path);

  dllFree(path);
  freeGraph(G);
  return 0;
}