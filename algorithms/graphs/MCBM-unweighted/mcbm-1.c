/* 
  file: mcbm-1.c  
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: Maximum Cardinality Bipartite Matching (MCBM) 
               using Dinic's algorithm for maximum flow
  time complexity: O(E√V)
  note: make sure to use VERTEX_TYPE5 in the vertex.h file
    by defining it from the command line using
      $ gcc -D VERTEX_TYPE5 ...
*/

#include "../../../datastructures/graphs/network/network.h"
#include "../../../datastructures/queues/queue.h"

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
// Breadth-first search to build the level graph
// Returns true if there is a path from v to sink
bool bfs(network *N, vertex *v, vertex *sink) {
  queue *q = newQueue(nVertices(N));       
  enqueue(q, v);                       // enqueue source node
  v->level = 1;     

  while (!isEmptyQueue(q)) {
    vertex *u = dequeue(q);
    dll *edges = getNeighbors(N, u);    
   
      // iterate over all outgoing edges of u
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
      if (e->cap - e->flow > 0 && ! e->to->level) {
        e->to->level = u->level + 1;   // set level of child node
        enqueue(q, e->to);                
      }
    }
  }
  freeQueue(q);
  return sink->level != 0;             // is sink reachable?
}

//===================================================================
// Depth-first search to find blocking flows
// Returns the bottleneck flow found in the path
size_t dfs(network *N, vertex *v, vertex *sink, 
           size_t flow) {
  
  if (v == sink || flow == 0) 
    return flow;                       // sink reached or no flow
  
  dll *edges = getNeighbors(N, v);     // get neighbors of v
  edge *e;
  
  if (! v->cont) {
    e = dllFirst(edges);               // start at first edge
    v->cont = true;                    // set continue flag
  } else 
    e = dllNext(edges);                // continue from last edge

  for ( ; e; e = dllNext(edges)) {
    if (e->to->level == v->level + 1) {
      size_t bneck = dfs(N, e->to, sink, 
                         MIN(flow, e->cap - e->flow));
      if (bneck) {
        e->flow += bneck;              // update flow
        e->rev->flow -= bneck;             
        return bneck;
      } 
    } 
  }
  return 0;
}

//===================================================================
// Computes the maximum flow from src to sink using Dinic's algorithm
void dinic(network *N, vertex *src, vertex *sink) {
  size_t flow;
  
  while (bfs(N, src, sink)) {
    while ((flow = dfs(N, src, sink, SIZE_MAX))) 
      N->maxFlow += flow;               

      // reset levels and continue flags for next BFS
    for (vertex *v = firstV(N); v; v = nextV(N)) {
      v->level = 0;
      v->cont = false;  
    }
  }
}

//===================================================================
// Shows the matching by iterating over all edges and showing
// the edges with flow > 0
void showMatching(network *N, vertex *src, vertex *sink) {
  
  printf("--------------------\n"
         " Matching\n"
         " Cardinality: %zu\n"
         "--------------------\n",
          N->maxFlow);
  
  for (edge *e = firstE(N); e; e = nextE(N))
    if (e->flow > 0 && e->from != src && e->to != sink)
      printf("  %s -- %s\n", e->from->label, e->to->label);
  printf("--------------------\n");
}

//===================================================================

int main () {

  network *N = newNetwork(50, UNWEIGHTED);
  readNetwork(N);
  showNetwork(N);
  
  if (!isBipartite(N)) {
    printf("The network is not bipartite.\n");
    freeNetwork(N);
    return 0;
  }

  vertex *src = NULL, *sink = NULL;
  extendNetwork(N, &src, &sink);

  dinic(N, src, sink); 
  showMatching(N, src, sink);

  freeNetwork(N); 
  return 0;
}