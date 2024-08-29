/* 
  file: hck.c  
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: Maximum Cardinality Bipartite Matching (MCBM) 
               using Hopcroft-Karp's algorithm
  time complexity: O(E√V)
  note: make sure to use VERTEX_TYPE6 in the vertex.h file
  by defining it from the command line using
    $ gcc -D VERTEX_TYPE6 ...
*/

#include "../../../datastructures/graphs/graph/graph.h"
#include "../../../datastructures/queues/queue.h"

//===================================================================
// Returns true if the component of the graph starting at vertex v is
// bipartite.
bool isBipartiteComponent(graph *G, vertex *v) {
  
  v->type = LEFT;
  queue *q = newQueue(nVertices(G)); 
  enqueue(q, v); 

  while (!isEmptyQueue(q)) {
    vertex *u = dequeue(q);
    dll *edges = getNeighbors(G, u); 
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
bool isBipartiteGraph(graph *G) {
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    if (v->type == NIL && !isBipartiteComponent(G, v))
      return false;
  }
  return true;
}

//===================================================================
// Creates and initializes a queue with all unmatched vertices 
// in the left set 
queue *initQueue(graph *G, vertex *nil) {
  queue *Q = newQueue(nVertices(G));
  
  for (vertex *u = firstV(G); u; u = nextV(G)) {
    if (u->type == RIGHT || u == nil) continue;

    if (u->match == nil) {
      u->level = 0;
      enqueue(Q, u);
    } else u->level = SIZE_MAX;      // u is in the matching
  }
  nil->level = SIZE_MAX;
  return Q;
}

//===================================================================
// Breadth-first search to find shortest alternating paths from the
// left set L to the right set R; returns true if an augmenting path
// is found, false otherwise
bool bfs(graph *G, vertex *nil) {
  
  queue *Q = initQueue(G, nil);

  while (!isEmptyQueue(Q)) {
    vertex *u = dequeue(Q);
  
    if (u == nil) break;             // unmatched vertex in R found

    dll *edges = getNeighbors(G, u);
    
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
      vertex *v = e->to;

      if (v->match->level == SIZE_MAX) { 
        v->match->level = u->level + 1;
        enqueue(Q, v->match);
      }
    }
  }
  freeQueue(Q);
  return nil->level != SIZE_MAX;     // augmenting path found?
}

//===================================================================
// Depth-first search to take the symmetric difference of the
// augmenting path found in the BFS and the current matching
bool dfs(graph *G, vertex *u, vertex *nil) {
  
  if (u != nil) {
    dll *edges = getNeighbors(G, u);

    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
      vertex *v = e->to;

      if (v->match->level == u->level + 1) 
        if (dfs(G, v->match, nil)) {
          v->match = u;
          u->match = v;
          return true;
        }
    }
    u->level = SIZE_MAX;
    return false;
  }
  return true;
}

//===================================================================
// The Hopcroft-Karp algorithm for finding the maximum cardinality
// matching in a bipartite graph G
size_t hopcroftKarp(graph *G, vertex *nil) {
  
  size_t nMatched = 0;

  while (bfs(G, nil)) 
    for (vertex *u = firstV(G); u; u = nextV(G)) {
      
      if (u->type == LEFT && u->match == nil && dfs(G, u, nil)) 
        nMatched++;
    }
    
  return nMatched;
}

//===================================================================
// Shows the matching by iterating over all vertices and showing
// the edges between the left and right sets
void showMatching(graph *G, size_t nMatched, vertex *nil) {
  printf("--------------------\n"
         " Matching\n"
         " Cardinality: %zu\n"
         "--------------------\n",
          nMatched);
  for (vertex *u = firstV(G); u; u = nextV(G)) 
    if (u->match != nil && u->type == LEFT)
      printf("  %s -- %s\n", u->label, u->match->label);
  printf("--------------------\n");
}

//===================================================================
// Extends the graph by adding a sentinel node NIL
// Initializes the matched field of all vertices to this node
vertex *extendGraph(graph *G) {
  vertex *nil = addVertexR(G, "NIL");
  for (vertex *u = firstV(G); u; u = nextV(G)) 
    u->match = nil;
  return nil;
}

//===================================================================

int main () {

  graph *G = newGraph(50, UNWEIGHTED); 
  setUndirected(G);
  readGraph(G);
  showGraph(G);

  if (! isBipartiteGraph(G)) {
    printf("The graph is not bipartite.\n");
    freeGraph(G);
    return 0;
  }

  vertex *nil = extendGraph(G);

  size_t nMatched = hopcroftKarp(G, nil);

  showMatching(G, nMatched, nil);

  freeGraph(G);
  return 0;
}
