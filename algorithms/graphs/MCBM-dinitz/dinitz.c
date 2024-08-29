/* 
  file: dinitz.c  
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
// Returns true if the component of the graph starting at vertex v is
// bipartite.
bool isBipartiteComponent(network *N, vertex *v) {
  
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
// Returns true if the graph is bipartite (two-colorable), i.e. if it
// is possible to partition the vertices into two sets such that no
// two adjacent vertices belong to the same set. This is done using a
// BFS traversal and assigning a type to each vertex (LEFT or RIGHT)
// As the graph may be disconnected, the function iterates over all
// vertices to ensure that all connected components are bipartite
bool isBipartiteNetwork(network *N) {
  for (vertex *v = firstV(N); v; v = nextV(N)) {
    if (v->type == NIL && !isBipartiteComponent(N, v))
      return false;
  }
  return true;
}

//===================================================================
// Builds a new network N' from the input network N with edges
// directed from L to R. This network is also extended with a source 
// and sink: edges are added from the source to all vertices in L 
// and from all vertices in R to the sink, all with unit capacity
network *buildNewNetwork(network *N, vertex **src, vertex **sink) {
  network *newN = newNetwork(nVertices(N) + 2, UNWEIGHTED);
  *src = addVertexR(newN, "SOURCE");
  *sink = addVertexR(newN, "SINK");

  for (vertex *v = firstV(N); v; v = nextV(N)) {
    vertex *u = addVertexR(newN, v->label);
    if (v->type == LEFT) {
      addEdge(newN, *src, u, 1);
      dll *edges = getNeighbors(N, v);
      for (edge *e = dllFirst(edges); e; e = dllNext(edges))
        addVandE(newN, u->label, e->to->label, 1);
    } else if (v->type == RIGHT) 
      addEdge(newN, u, *sink, 1);
  }
  return newN;
}

//===================================================================
// Breadth-first search to build the level graph
// Returns true if there is a path from source to sink
bool bfs(network *N, vertex *src, vertex *sink) {
  queue *Q = newQueue(nVertices(N));
  src->level = 1;                      // set level of source
  enqueue(Q, src);                     // add source to queue

  while (!isEmptyQueue(Q)) {
    vertex *u = dequeue(Q);
    dll *edges = getNeighbors(N, u);    
   
      // iterate over all outgoing edges of u
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
      if (e->cap - e->flow > 0 && e->to->level == 0) {
        e->to->level = u->level + 1;   // set level of child node
        enqueue(Q, e->to);                
      }
    }
  }
  freeQueue(Q);
  return sink->level != 0;             // is sink reachable?
}

//===================================================================
// Depth-first search to find an augmenting path in the level graph
// Returns the bottleneck flow found in the path
size_t dfs(network *N, vertex *v, vertex *sink, size_t flow) {
  
  if (v == sink || flow == 0) 
    return flow;                       // sink reached or no flow
  
  dll *edges = getNeighbors(N, v);     

  for (edge *e = v->cont ? dllNext(edges) : dllFirst(edges); e; 
       e = dllNext(edges)) {

    v->cont = true;                    // set continue flag

    if (e->to->level == v->level + 1){ // child node in next level?

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

    // while there is a path from src to sink
  while (bfs(N, src, sink)) {
      // keep searching for M-augmenting paths until a
      // blocking flow is reached
    while ((flow = dfs(N, src, sink, SIZE_MAX))) 
      N->maxFlow += flow;               

      // reset levels and continue flags for next iteration
    for (vertex *v = firstV(N); v; v = nextV(N)) {
      v->level = 0;
      v->cont = false;  
    }
  }
}

//===================================================================
// Shows the matching by iterating over all edges and showing
// the edges with flow
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
  setNLabel(N, "Input Network N");
  showNetwork(N);
  
  if (!isBipartiteNetwork(N)) {
    printf("The network is not bipartite.\n");
    freeNetwork(N);
    return 0;
  }

  vertex *src, *sink;
  network *newN = buildNewNetwork(N, &src, &sink);
  setNLabel(newN, "New Network N'");
  showNetwork(newN);

  dinic(newN, src, sink);
  showMatching(newN, src, sink);

  freeNetwork(N); 
  freeNetwork(newN);
  return 0;
}