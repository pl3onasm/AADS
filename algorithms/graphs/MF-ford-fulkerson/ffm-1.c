/* 
  file: ffm-1.c  
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: implements the Edmonds-Karp version of the 
    max-flow algorithm. This is Ford-Fulkerson with BFS.
  time complexity: O(VE²) 
  note: make sure to use VERTEX_TYPE1 in the vertex.h file
    by defining it from the command line using
      $ gcc -D VERTEX_TYPE1 ...

*/

#include "../../../datastructures/graphs/network/network.h"
#include "../../../datastructures/queues/queue.h"
#include <assert.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

//===================================================================
// Tries to find an augmenting path from src to sink using BFS
// Returns the flow of the path
size_t bfs(network *N, vertex *src, vertex *sink) {
    
  size_t flow = SIZE_MAX;  
  queue *Q = newQueue(nVertices(N));
  enqueue(Q, src); 

  while (!isEmptyQueue(Q)) {
    vertex *v = dequeue(Q); 
    dll *edges = getNeighbors(N, v);
   
      // discover the neighbors of v and set the path edges
      // along the way while keeping track of the bottleneck flow
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
      if (! e->to->pathEdge && e->cap - e->flow > 0) {
        e->to->pathEdge = e;
        flow = MIN(flow, e->cap - e->flow);
        if (e->to == sink) {
          freeQueue(Q);
          return flow;
        }
        enqueue(Q, e->to);
      }
    }
  }
  freeQueue(Q);
  return 0;
}

//===================================================================
// Finds the maximum flow from src to sink using Edmonds-Karp
void edmondsKarp(network *N, vertex *src, vertex *sink) {
  size_t flow; 
  while ((flow = bfs(N, src, sink))) {
    N->maxFlow += flow;    
      // update the flow along the found path
    for (edge *e = sink->pathEdge; ; e = e->from->pathEdge) {
      e->flow += flow;      
      e->rev->flow -= flow;
      if (e->from == src) break;
    }
      // reset the path edges for the next BFS
    for (vertex *v = firstV(N); v; v = nextV(N)) 
      v->pathEdge = NULL;
  }
}

//===================================================================

int main (int argc, char *argv[]) {
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

  edmondsKarp(N, src, sink); 
  showFlow(N, src, sink);     

  freeNetwork(N);                 
  return 0;
}
