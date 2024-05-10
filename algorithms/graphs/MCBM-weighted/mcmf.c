/* 
  file: mcmf.c  
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: weighted MCBM using min/max-cost max-flow
  time complexity: O(V²E²)
  note 1: make sure to use VERTEX_TYPE6 in the vertex.h file  
    and EDGE_TYPE1 in the edge.h file by defining them from 
    the command line using
      $ gcc -D VERTEX_TYPE6 -D EDGE_TYPE1 ...
  maximum flow.
*/

#include "../../../datastructures/graphs/network/network.h"
#include "../../../datastructures/queues/queue.h"
#include <assert.h>
#include <float.h>
#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

//===================================================================
// Returns true if the graph is bipartite, i.e. if it is possible
// to partition the vertices into two sets such that no two adjacent
// vertices belong to the same set. This is done using a BFS 
// traversal and assigning a type to each vertex (LEFT or RIGHT)
bool isBipartite(network *N) {
  vertex *v = firstE(N)->from;
  v->type = LEFT;
  queue *q = newQueue(nVertices(N)); 
  enqueue(q, v); 

  while (!isEmptyQueue(q)) {
    vertex *u = dequeue(q);
    dll *edges = getNeighbors(N, u); 
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
      if (e->to->type == NIL) {
        e->to->type = u->type == LEFT ? RIGHT : LEFT;
        enqueue(q, e->to);            
      } else if (e->to->type == u->type) {
        freeQueue(q);
        return false;                
      }
    }
  }
  freeQueue(q);
  return true;                       
}

//===================================================================
// Extends the network with a source and sink vertex: adds edges
// from source to all vertices in set L with unit capacity, and
// from all vertices in set R to the sink with unit capacity
void extendNetwork(network *N, vertex **src, vertex **sink) {

  *src = addVertexR(N, "src");
  *sink = addVertexR(N, "sink");
  for (vertex *v = firstV(N); v; v = nextV(N)) {
    if (v == *src || v == *sink) continue;
    if (v->type == LEFT) addEdge(N, *src, v, 1);
    else addEdge(N, v, *sink, 1);
  }
}

//===================================================================
// Initializes the graph G with the source node s: sets the distance
// of all nodes to the source to infinity and the distance of the
// source node to 0
void initSingleSource(network *N, vertex *src) {
  for (vertex *v = firstV(N); v; v = nextV(N)) {
    v->dist = DBL_MAX;
    v->cont = false;
  }
  src->dist = 0;
}

//===================================================================
// Runs the Bellman-Ford algorithm to find the shortest path from
// the source to the sink; returns true if the sink is reachable
bool bellmanFord(network *N, vertex *src, vertex *sink, int cType) {

  initSingleSource(N, src);          

    // relax all edges n-1 times 
  for (size_t i = 0; i < nVertices(N) - 1; i++) 
    for (edge *e = firstE(N); e; e = nextE(N))
      if (e->cap - e->flow > 0 && e->from->dist != DBL_MAX)
        e->to->dist = MIN(e->to->dist, 
                          e->from->dist + cType * e->weight);
  
  return sink->dist != DBL_MAX;        // is sink reachable?
}

//===================================================================
// Depth-first search to find augmenting paths in the residual
// graph; returns true if a path is found
size_t dfs(network *N, vertex *v, vertex *sink, size_t flow,
         int cType, double *totalCost) {
  
  if (v == sink || flow == 0)
    return flow;                       // sink reached or no flow
            
  dll *edges = getNeighbors(N, v);     
  edge *e;
  
  if (! v->cont) {
    e = dllFirst(edges);               // start at first edge
    v->cont = true;                    // set continue flag
  } else 
    e = dllNext(edges);                // continue from last edge

  for (; e; e = dllNext(edges)) {
    if (e->to->dist == v->dist + cType * e->weight) {
      
      size_t bneck = dfs(N, e->to, sink, MIN(flow, e->cap - e->flow), 
                      cType, totalCost);

      if (bneck) {
        *totalCost += e->weight;       // update total cost
        e->flow += bneck;              // update flow
        e->rev->flow -= bneck;             
        return bneck;
      } 
    } 
  }
  return 0;
}

//===================================================================
// Computes the min/max-cost matching by running the min/max-cost
// max-flow algorithm
double computeMCM(network *N, vertex *src, vertex *sink, int cType) {
  double totalCost = 0;
  size_t flow;

  while (bellmanFord(N, src, sink, cType)) {
      // find the augmenting paths and update the total cost
    while ((flow = dfs(N, src, sink, SIZE_MAX, cType, &totalCost)))
        // update cardinality of the matching
      N->maxFlow += flow;
  }
  return totalCost;
}

//===================================================================
// Shows the matching by iterating over all edges and showing
// the edges with flow > 0
void showMatching(network *N, vertex *src, vertex *sink,
                  int cType, double totalCost) {

  printf("--------------------\n"
         " Matching\n"
         " Cardinality: %zu\n"
         " %s cost: %.2f\n"
         "--------------------\n",
          N->maxFlow, 
          cType == 1 ? "Min" : "Max",
          totalCost);

  for (edge *e = firstE(N); e; e = nextE(N))
    if (e->flow > 0 && e->from != src && e->to != sink)
      printf("  %s -- %s\n", e->from->label, e->to->label);
  printf("--------------------\n");
}

//===================================================================
// Reads the cost type from stdin: either MIN or MAX
int readCostType() {
  char type[20];
  assert(scanf("%s ", type) == 1);
  if (strcmp(type, "MIN") == 0) return 1;
  else if (strcmp(type, "MAX") == 0) return -1;
  else {
    fprintf(stderr, "Invalid cost type: %s\n"
                    "Valid types are: MIN, MAX\n", type);
    exit(EXIT_FAILURE);
  }
}

//===================================================================

int main () {
    // min or max cost?
  int cType = readCostType(); 

  network *N = newNetwork(50, WEIGHTED);
  readNetwork(N);
  showNetwork(N);
  
  if (!isBipartite(N)) {
    printf("The network is not bipartite.\n");
    freeNetwork(N);
    return 0;
  }

  vertex *src = NULL, *sink = NULL;
  extendNetwork(N, &src, &sink);

  showNetwork(N);

  double totalCost = computeMCM(N, src, sink, cType);
  showMatching(N, src, sink, cType, totalCost);

  freeNetwork(N); 
  return 0;
}