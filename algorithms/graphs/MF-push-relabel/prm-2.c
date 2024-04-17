/* 
  file: prm-2.c  
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: implements the relabel-to-front algorithm
  time complexity: O(V³), which is better than the generic
    push-relabel algorithm for dense graphs
  note: make sure to use VERTEX_TYPE4 in the vertex.h file
    by defining it from the command line using
      $ gcc -D VERTEX_TYPE4 ...
*/

#include "../../../datastructures/graphs/network/network.h"
#include <assert.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

//===================================================================
// Initializes the preflow at the source node
void initPreflow(network *N, vertex *src) {

  src->height = nVertices(N);         // set height of source to |V|
  dll *edges = getNeighbors(N, src);

    // set full flow on all edges leaving the source
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
    
    e->flow = e->cap;                 // set flow on the edge
    e->rev->flow = -e->cap;       
    e->to->excess += e->cap;          // set excess at neighbor
  }
}

//===================================================================
// Pushes flow from u to v along edge e
void push(network *N, vertex *u, vertex *v, edge *e) {
  
    // determine the amount of flow to push: the minimum of 
    // the excess at u and the residual capacity of the edge
  double delta = MIN(u->excess, e->cap - e->flow);

  e->flow += delta;                   // update flow on edges
  e->rev->flow -= delta;      
  u->excess -= delta;                 // update excess at u
  v->excess += delta;                 // update excess at v
}

//===================================================================
// Relabels a vertex v by setting its height to one more than
// the minimum height of its neighbors in the residual network
void relabel(network *N, vertex *v) {
  
  size_t min = SIZE_MAX;
  dll *edges = getNeighbors(N, v);

  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
    if (e->cap - e->flow > 0) 
      min = MIN(min, e->to->height);
  
  v->height = min + 1;
}

//===================================================================
// Discharges the excess at a vertex v: pushes all excess flow
// from v to its neighbors in the residual network, relabeling v
// if needed
void discharge(network *N, vertex *v) {
  dll *edges = getNeighbors(N, v);
  edge *e;

  while (v->excess > 0) {
    if ((e = dllNext(edges))){        // consider next neighbor
      
      if (e->cap - e->flow > 0 && v->height == e->to->height + 1) 
        push(N, v, e->to, e);   
      
    } else {                          // no neighbors to push to 
      relabel(N, v);                  // relabel v
      dllResetIter(edges);            // reset iterator
    }
  }
}

//===================================================================
// Creates a list of all vertices in N except for src and sink
dll *createWorklist(network *N, vertex *src, vertex *sink) {
  
  dll *worklist = dllNew();
  for (vertex *v = firstV(N); v; v = nextV(N)) {
    if (v == src || v == sink)
      continue; 
    dllPush(worklist, v);
  }
  return worklist;
}

//===================================================================
// Computes the maximum flow in a network from src to sink using
// the relabel-to-front algorithm
void computeMaxflow (network *N, vertex *src, vertex *sink) {
  
  initPreflow(N, src);                
  dll *worklist = createWorklist(N, src, sink);   
  vertex *v;

  while ((v = dllNext(worklist))) {     
    size_t oldHeight = v->height;
    discharge(N, v);                  // discharge the excess at v
    
    if (v->height > oldHeight) {      // was v relabeled?
      dllDeleteCurr(worklist);        // remove v from worklist
      dllPush(worklist, v);           // push v to front of worklist
      dllSetIterFirst(worklist);      
    }
  }
  N->maxFlow = sink->excess;          // max flow is excess at sink
  dllFree(worklist);                  
} 

//===================================================================

int main () {
    // read source and sink labels
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

  computeMaxflow(N, src, sink); 
  showFlow(N, src, sink);     

  freeNetwork(N);                 
  return 0;
}