/*
  file: prm-1.c  
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: implements the generic push-relabel algorithm
  time complexity: O(V²E)
  note: make sure to use VERTEX_TYPE4 in the vertex.h file
    by defining it from the command line using
      $ gcc -D VERTEX_TYPE4 ...
*/

#include "../../../datastructures/graphs/network/network.h"
#include "../../../datastructures/queues/queue.h"
#include <assert.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

//===================================================================
// Initializes the preflow at the source node
void initPreflow(network *N, vertex *src, queue *Q) {

  src->height = nVertices(N);         // set height of source to |V|
  dll *edges = getNeighbors(N, src);

    // set full flow on all edges leaving the source
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
    
    e->flow = e->cap;                 // set flow on the edge
    e->rev->flow = -e->cap;       
    e->to->excess += e->cap;          // set excess at neighbor
    enqueue(Q, e->to);                // enqueue neighbor
  }
}

//===================================================================
// Tries to push flow from v to a neighbor; returns true if
// flow was pushed, false otherwise
bool push(network *N, vertex *v, queue *Q) {
  
  dll *edges = getNeighbors(N, v);
  edge *e;
  while ((e = dllNext(edges))) {      // continue from last edge
    
    if (e->cap - e->flow > 0 && v->height == e->to->height + 1) {

        // determine the amount of flow to push: the minimum of
        // the excess at v and the remaining capacity on the edge
      size_t delta = MIN(v->excess, e->cap - e->flow);

      e->flow += delta;               // update flow on edge
      e->rev->flow -= delta;      
      v->excess -= delta;             // update excess at v
      e->to->excess += delta;         // update excess at neighbor
      if (e->to->excess == delta)     // if neighbor had no excess
        enqueue(Q, e->to);            // before the push, enqueue it
      return true;
    }
  }
  return false;
}

//===================================================================
// Relabels a vertex v by setting its height to one more than
// the minimum height of its neighbors in the residual network
void relabel(network *N, vertex *v) {
  
  size_t min = SIZE_MAX;

  dll *edges = getNeighbors(N, v);
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
    if (e->cap - e->flow > 0) 
      min = MIN(min, e->to->height);
  }
  v->height = min + 1;
}

//===================================================================
// Computes the maximum flow in a network from src to sink using
// the push-relabel method
void computeMaxflow (network *N, vertex *src, vertex *sink) {
  
  queue *Q = newQueue(nVertices(N));  // a queue for active nodes  
  initPreflow(N, src, Q);
  
  while (!isEmptyQueue(Q)) {
    vertex *v = dequeue(Q);           // get the next active node
    
    if (v == src || v == sink)        // skip source and sink
      continue;
  
                                      // we know v has excess flow 
    if (!push(N, v, Q))               // so we try to push from v, 
      relabel(N, v);                  // if that fails, we relabel v
      
    if (v->excess > 0)                // if v still has excess
      enqueue(Q, v);                  // we re-enqueue v
  }

  N->maxFlow = sink->excess;          // max flow is excess at sink
  freeQueue(Q);
} 

//===================================================================

int main () {
    // read source and sink labels
  char srcL[50], sinkL[50];                    
  assert(scanf("%s %s ", srcL, sinkL) == 2);

  network *N = newNetwork(50, UNWEIGHTED);
  readNetwork(N);                  
  showNetwork(N);
  
  vertex *src = getVertex(N, srcL);     
  vertex *sink = getVertex(N, sinkL);     

  if (!src || !sink) {
    fprintf(stderr, "Source or sink not found\n");
    freeNetwork(N);
    exit(EXIT_FAILURE);
  }

  computeMaxflow(N, src, sink); 
  showFlow(N, src, sink);     

  freeNetwork(N);                 
  return 0;
}
