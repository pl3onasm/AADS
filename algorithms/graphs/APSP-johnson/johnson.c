/* 
  file: johnson.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: Johnson's algorithm for the all-pairs shortest 
    paths problem
  time complexity: O(|V|² log |V| + |V| |E|)
  note: make sure to use VERTEX_TYPE5 in the vertex.h file
    by defining it from the command line using
      $ gcc -D VERTEX_TYPE5 ...
*/

#include "../../../datastructures/graphs/graph/graph.h"
#include "../../../datastructures/pqueues/bpqueue.h"
#include "../../../lib/clib/clib.h"
#include <float.h>

//===================================================================
// Extends the graph G with a new source node connected to all
// other nodes in G with zero-weight edges; returns this new source
vertex *extendGraph(graph *G) {
  vertex *src = addVertexR(G, "srcExt");
  for (vertex *v = firstV(G); v; v = nextV(G)) 
    if (v != src) 
      addEdgeW(G, src, v, 0);
  return src;
}

//===================================================================
// Shows the shortest path from vertex with index i to vertex with
// index j using the predecessor matrix P
void showPath (vertex **V, size_t **P, size_t i, size_t j) {
  if (j == i) printf("%s", V[i]->label);
  else {
    showPath(V, P, i, P[i][j]);
    printf(" → %s", V[j]->label);
  }
}

//===================================================================
// Shows the shortest path from each vertex to every other vertex
// along with the distance between them. The paths are shown in
// the order of the vertices as they appear in the graph when
// displayed with the showGraph function
void showAllPaths (vertex **V, size_t nV, double **D, size_t **P) {
  printf("--------------------\n"
         " Shortest paths\n"
         "--------------------\n");

  for (size_t from = 0; from < nV; from++) 
    for (size_t to = 0; to < nV; to++) {

      if (from == to) continue;

      printf("%s → %s: ", V[from]->label, V[to]->label);
      printf(D[from][to] == DBL_MAX ? "INF" : "%.2f", D[from][to]);

      if (D[from][to] != DBL_MAX) {
        printf("\n  path: ");
        showPath(V, P, from, to);
      }
      printf("\n");
    }
  printf("--------------------\n");
}

//===================================================================
// Initializes the graph G with the source node src:
// sets the Bellman-Ford distance of all nodes to the source to 
// infinity and the distance of the source node to 0
void initSingleSource(graph *G, vertex *src) {
  for (vertex *v = firstV(G); v; v = nextV(G)) 
    v->bfDist = DBL_MAX;
  src->bfDist = 0;
}

//===================================================================
// Computes the shortest paths from added source node src to all
// other nodes in G' and checks for negative cycles; returns false
// if no negative cycles were found
bool runBellmanFord(graph *G, vertex *src) {

  initSingleSource(G, src);  

    // relax all edges n-1 times
  vertex *from;
  for (size_t i = 0; i < nVertices(G) - 1; i++)
    for (edge *e = firstE(G, &from); e; e = nextE(G, &from))
      if (from->bfDist + e->weight < e->to->bfDist)
        e->to->bfDist = from->bfDist + e->weight;

    // check for negative-weight cycles by checking if
    // the distances can still be improved
  for (edge *e = firstE(G, &from); e; e = nextE(G, &from))
    if (from->bfDist + e->weight < e->to->bfDist) 
      return true;

  return false;
}

//===================================================================
// Reweights the edges of the graph G using the Bellman-Ford
// distances of the vertices
void reweightEdges(graph *G) {
  vertex *from;
  for (edge *e = firstE(G, &from); e; e = nextE(G, &from)) 
    e->weight += from->bfDist - e->to->bfDist;
}

//===================================================================
// Copies the key (priority) of a node in the priority queue
void *copyKey (void *key) {
  double *copy = safeCalloc(1, sizeof(double));
  *copy = *(double *)key;
  return copy;
}

//===================================================================
// Comparison function for the priority queue
int compareKeys(void const *k1, void const *k2) {
  double d1 = *(double *)k1;
  double d2 = *(double *)k2;
  if (d1 < d2) return -1;
  if (d1 > d2) return 1;
  return 0;
}

//===================================================================
// String representation of the data in the priority queue
char *vertexToString(void const *key) {
  vertex *v = (vertex *)key;
  return v->label;
}

//===================================================================
// Generates and initializes the min priority queue
// All vertices are added to the priority queue with infinite
// distance from the source node and likewise infinite priority
// The distance and priority of the source node is set to 0
bpqueue *initPQ(graph *G, vertex *src) {

  bpqueue *pq = bpqNew(nVertices(G), MIN, compareKeys, copyKey, 
                       free, vertexToString);
  
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    v->dDist = v == src ? 0 : DBL_MAX;
    bpqPush(pq, v, &v->dDist);
  }
  return pq;
}

//===================================================================
// Tries to 'relax' the edge (u,v) with weight w; 
// Returns true if relaxation was successful
bool relax(vertex *u, vertex *v, double w) {
  
  if (v->dDist > u->dDist + w) {
    v->dDist = u->dDist + w;
    v->parent = u->index;
    return true;
  }
  return false;
}

//===================================================================
// Computes the shortest paths from src to all other vertices
void dijkstra(graph *G, vertex *src) {
      
    // genereate a new priority queue and initialize it
  bpqueue *pq = initPQ(G, src);

  while (! bpqIsEmpty(pq)) {
    vertex *u = bpqPop(pq);
    dll* edges = getNeighbors(G, u);
      
      // try to relax all the edges from u to its neighbors
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
      if (bpqContains(pq, e->to) && relax(u, e->to, e->weight)) 
          // update neighbor's priority in the pq if edge was relaxed
        bpqUpdateKey(pq, e->to, &e->to->dDist);
  } 
  bpqFree(pq);
}

//===================================================================
// Computes the shortest paths from each node in G to all other nodes
// by running Dijkstra on the graph G with reweighted edges
void runDijkstra(graph *G, vertex **V, size_t nV, 
                 double **D, size_t **P) {

    // compute shortest paths from each node in G using G.E'
  for (size_t from = 0; from < nV; from++) {
    dijkstra(G, V[from]);

    for (size_t to = 0; to < nV; to++) {
        // update distance matrix for edge from -> to
      D[from][to] = V[to]->dDist + V[to]->bfDist - V[from]->bfDist;
        // update predecessor matrix for edge from -> to
      P[from][to] = V[to]->parent;
      V[to]->dDist = DBL_MAX;
    }
  }
}

//===================================================================

int main () {
 
  graph *G = newGraph(50, WEIGHTED);  
  readGraph(G);          
  showGraph(G);    

  size_t nV = nVertices(G);
  vertex **V = sortVertices(G); 
    
    // extend the graph with a new source vertex
  vertex *src = extendGraph(G);
  
  bool negCycle = runBellmanFord(G, src);
  if (negCycle) {
    printf("Negative cycle detected\n");
    freeGraph(G);
    free(V); 
    return 0;
  }

    // create distance and predecessor matrices
  CREATE_MATRIX(double, D, nV, nV);
  CREATE_MATRIX(size_t, P, nV, nV);

  reweightEdges(G);

  runDijkstra(G, V, nV, D, P);

  showAllPaths(V, nV, D, P);
  
  freeGraph(G);
  free(V); 
  FREE_MATRIX(D, nV);
  FREE_MATRIX(P, nV);
  return 0;
}
