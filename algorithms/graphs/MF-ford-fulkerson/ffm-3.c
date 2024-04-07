/* 
  file: ffm-3.c  
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: implements the Dinitz version of the 
    max-flow algorithm. This is Ford-Fulkerson with 
    level graphs and DFS.
  time complexity: O(V²E)
  note: make sure to use VERTEX_TYPE3 in the vertex.h file
    by defining it from the command line using
      $ gcc -D VERTEX_TYPE3 ...
*/

#include "../../../datastructures/graphs/network/network.h"
#include "../../../datastructures/queues/queue.h"
#include <assert.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

//===================================================================
// Breadth-first search to build the level graph
// Returns true if there is a path from v to sink
bool bfs(network *N, vertex *v, vertex *sink) {
  queue *q = newQueue(nVertices(N));       
  enqueue(q, v);                       // enqueue source node
  v->level = 0;     

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
  return sink->level != 0;             // return if t is reachable
}

//===================================================================
// Depth-first search to find blocking flows
// Returns the bottleneck flow found in the path
double dfs(network *N, vertex *v, vertex *sink, 
           size_t flow, bool newLevels) {
  
  if (v == sink) return flow;          // reached sink
  
  dll *edges = getNeighbors(N, v);     // get neighbors of v
  edge *e;
  
  if (newLevels) {
    e = dllFirst(edges);               // start at first edge
    newLevels = false;
  } else e = dllNext(edges);           // continue from last edge

  for ( ; e; e = dllNext(edges)) {
    if (e->cap - e->flow > 0 && e->to->level == v->level + 1) {
      size_t bneck = dfs(N, e->to, sink, 
                         MIN(flow, e->cap - e->flow), newLevels);
      if (bneck > 0) {
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
    bool newLevels = true;
    while ((flow = dfs(N, src, sink, SIZE_MAX, newLevels))) {
      N->maxFlow += flow;                      
      // reset all levels to 0 for next BFS 
    for (vertex *v = firstV(N); v; v = nextV(N)) 
      v->level = 0;
    }
  }
}

//===================================================================

int main () {
    // read source and sink 
  char srcL[50], sinkL[50];                    
  assert(scanf("%s %s", srcL, sinkL) == 2);

  network *N = newNetwork(50);    
  readNetwork(N);                  
  showNetwork(N);
  
  vertex *src = getVertex(N, srcL);     
  vertex *sink = getVertex(N, sinkL);     

  if (!src || !sink) {
    fprintf(stderr, "Source or sink not found\n");
    freeNetwork(N);
    exit(EXIT_FAILURE);
  }

  dinic(N, src, sink); 
  showFlow(N, src, sink);     

  freeNetwork(N);                 
  return 0;
}
