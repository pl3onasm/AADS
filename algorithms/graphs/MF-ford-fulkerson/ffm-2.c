/* 
  file: ffm-2.c  
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: implements Ford-Fulkerson with DFS and a
    heuristic called capacity scaling to find the maximum flow
  time complexity: O(E²log(maxCap))
  note: make sure to use VERTEX_TYPE2 in the vertex.h file
    by defining it from the command line using
      $ gcc -D VERTEX_TYPE2 ...
*/

#include "../../../datastructures/graphs/network/network.h"
#include "../../../datastructures/queues/queue.h"
#include <assert.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

//===================================================================
// Returns the largest power of 2 ≤ the network's max capacity
size_t threshold(network *N) {
  size_t p = 1;
  while (p < N->maxCap) p <<= 1;
  return p >> 1;
}

//===================================================================
// Tries to find an augmenting path from src to sink using DFS
double dfs(network *N, vertex *v, vertex *sink, 
           size_t flow, size_t delta) {

  if (v == sink) return flow;       // reached the sink
  
  if (v->visited) return 0;         // already visited

  size_t bneck;
  dll *edges = getNeighbors(N, v);    

  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {    
    if (e->cap - e->flow >= delta) {             
      v->visited = true;            
      if ((bneck = dfs(N, e->to, sink, 
           MIN(flow, e->cap - e->flow), delta))) {
        e->flow += bneck;             
        e->rev->flow -= bneck;            
        v->visited = false;         // reset flag for next DFS
        return bneck;
      }               
    }
  }
  v->visited = false;               // reset flag for next DFS
  return 0;
}

//===================================================================
// Computes the maximum flow from src to sink using DFS and 
// capacity scaling
void maxFlow(network *N, vertex *src, vertex *sink) {
  
  size_t flow;
  size_t delta = threshold(N);      
  printf("Threshold: %zu\n", delta);

  for (; delta > 0; delta >>= 1) {     
    while ((flow = dfs(N, src, sink, SIZE_MAX, delta))) {  
      N->maxFlow += flow;          
      printf("Flow: %zu\n", flow);
    }
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
  printf ("Max capacity: %zu\n", N->maxCap);
  
  vertex *src = getVertex(N, srcL);     
  vertex *sink = getVertex(N, sinkL);     

  if (!src || !sink) {
    fprintf(stderr, "Source or sink not found\n");
    freeNetwork(N);
    exit(EXIT_FAILURE);
  }

  maxFlow(N, src, sink); 
  showFlow(N, src, sink);     

  freeNetwork(N);                 
  return 0;
}
