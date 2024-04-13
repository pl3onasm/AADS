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
  note 1: make sure to use VERTEX_TYPE2 in the vertex.h file and 
    EDGE_TYPE3 in the edge.h file by defining them from the  
    command line using
      $ gcc -D VERTEX_TYPE2 -D EDGE_TYPE3 ...
  note 2: tours may differ from one run to another since
    the graph is implemented as a hash table of vertices
*/

#include "../../../datastructures/graphs/graph/graph.h"
#include "../../../lib/clib/clib.h"

//===================================================================
// Makes a copy of an edge; used to make the dll store copies of the
// edges in the Eulerian path
void *copyEdge(void *data) {
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
size_t countReachable(graph *G, vertex *v, edge *taken) {
 
  size_t nVisited = 1;
  v->visited = true;

  dll *edges = getNeighbors(G, v);
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
    if (!e->to->visited && (!taken || e != taken)) 
      nVisited += countReachable(G, e->to, taken);

  return nVisited;
}

//===================================================================
// Checks if the edge to vertex v is a bridge by performing a single
// dfs call over the remaining vertices in the graph: if the count
// of reachable vertices from v is less than the total number of
// remaining vertices in the graph, then the edge is a bridge
bool isBridge(graph *G, vertex *v, edge *e, size_t remV) {

    // mark all vertices as unvisited before calling dfs
  for (vertex *u = firstV(G); u; u = nextV(G))
    u->visited = false;

  size_t nVisited = countReachable(G, v, e);
  
  return nVisited < remV;
}

//===================================================================
// Checks if the graph is connected
bool isConnected(graph *G) {
  if (!G) return false;
    
    // number of reachable vertices from a random vertex
    // should be equal to the total number of vertices
  return countReachable(G, firstV(G), NULL) == nVertices(G);
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
    edge *rev = G->type == UNDIRECTED ? getEdge(G, e->to, v) : NULL;

      // take the first non-bridge edge found
    if (!isBridge(G, e->to, rev, remV)) 
      break;
  }
  return e;
}

//===================================================================
// Finds an Eulerian path or cycle in a graph
void fleury(graph *G, size_t remV, vertex *v, dll *path) {

    // base case: no more edges to take
  if (remV == 1) return;

  dll *edges = getNeighbors(G, v);
  edge *e = NULL;
  
  if ((G->type == DIRECTED && v->inDegree == 0) ||
      (G->type == UNDIRECTED && v->inDegree <= 1))
      // update the number of remaining vertices since
      // v will become unreachable after taking its last edge              
    remV--;                 
  else 
    e = findEdge(G, v, remV);
  
    // if v has only one edge, or if all edges are bridges
    // then take the first edge in the list
  if (!e) e = dllFirst(edges);

  e->from = v;
  vertex *u = e->to;
  dllPushBack(path, e);
  delEdge(G, e->from, e->to);
  
  fleury(G, remV, u, path);
}

//===================================================================
// Determines whether an Eulerian path or cycle exists in the graph
// and if so, returns the starting vertex for the path or cycle
// NULL is returned if no Eulerian path or cycle exists
vertex *startVertex(graph *G) {

  if (!isConnected(G)) return NULL;

    // An Eulerian tour exists if the graph 
    // has less than 3 vertices with odd degree
  size_t nOdd = 0;
  vertex *start = NULL;
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    if (degree(G, v) & 1) {
      nOdd++;
      if (nOdd > 2) return NULL;
      start = v;
    }
  }
  return start ? start : firstV(G);
}

//===================================================================
// Displays the Eulerian tour
void showPath(dll *path) {
  if (!path) return;

  printf("\n---------------\n"
         " Eulerian tour\n"
         "---------------\n");

  edge *e;
  while ((e = dllPop(path))) {
    printf("   %s -- %s\n", e->from->label, e->to->label);
    free(e);
  }
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