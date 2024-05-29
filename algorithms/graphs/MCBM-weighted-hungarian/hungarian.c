/* 
  file: hungarian.c  
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: weighted MCBM using the Hungarian algorithm
  time complexity: O(V³), using the optimization as described
    in problem 25-2 of CLRS4 (implemented as a slack attribute), 
    which reduces the time complexity from O(V⁴) to O(V³)
  note: make sure to use VERTEX_TYPE8 in the vertex.h file  
    by defining it from the command line using
      $ gcc -D VERTEX_TYPE8 ...
*/

#include "../../../datastructures/graphs/graph/graph.h" 
#include "../../../datastructures/queues/queue.h"
#include <assert.h>
#include <float.h>
#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

//===================================================================
// Returns true if the graph is bipartite (two-colorable), i.e. if it 
// is possible to partition the vertices into two sets such that no 
// two adjacent vertices belong to the same set. This is done using a 
// BFS traversal and assigning a type to each vertex (LEFT or RIGHT)
bool isBipartite(graph *G) {
  vertex *v = firstV(G);
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
// Initializes the matching with a greedy algorithm in the equality
// subgraph: for each vertex in L, we match it with the first 
// unmatched vertex in R that is adjacent to it
size_t initMatching(graph *G) {
  size_t nMatched = 0;
  for (vertex *u = firstV(G); u; u = nextV(G)) {
    if (u->type == RIGHT) continue;
    dll *edges = getNeighbors(G, u);
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
      if (u->height + e->to->height - e->weight < 1e-6 
          && e->to->match == NULL) {
        nMatched++;
        u->match = e->to;
        e->to->match = u;
        break;
      }
    }
  }
  return nMatched;
}

//===================================================================
// Creates and initializes a queue with all unmatched vertices 
// in L; also initializes the slack attribute of the vertices in R 
// to the minimum slack of the edges incident to them
queue *initBFS(graph *G) {
  queue *Q = newQueue(nVertices(G));
  
  for (vertex *u = firstV(G); u; u = nextV(G)) {
    
    if (u->type == RIGHT) {
      dll *edges = getNeighbors(G, u);
        // determine the minimum slack of the edges incident to u and
        // set the minParent to the vertex that minimizes the slack
      u->slack = DBL_MAX;
      for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
        if (e->to->match == NULL) {
          double slack = u->height + e->to->height - e->weight;
          if (slack < u->slack) {
            u->slack = slack;
            u->minParent = e->to;
          }
        }
      } 
      u->parent = NULL;
    } else {
      if (u->match == NULL) {
        u->parent = u;    
        enqueue(Q, u);
      } else {
        u->parent = NULL;
      }
    }
  }
  return Q;
}

//===================================================================
// Prints the matching
void showMatching(graph *G) {
  printf("--------------------\n"
         "Matching\n"
          "--------------------\n");
  double cost = 0;
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    cost += v->height;
    if (v->type == LEFT) {
      printf("  %s -- %s\n", v->label, v->match->label);
    }
  }
  printf("--------------------\n"
         "Total max cost: %.2lf\n\n", cost);
}

//===================================================================
// Initializes the heights of the vertices: a vertex in the left set 
// has height equaling the maximum weight of the edges incident to 
// it; a vertex in the right set has height 0 and sigma set to INF 
void initHeights(graph *G) {
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    if (v->type == LEFT) {
      dll *edges = getNeighbors(G, v);
      v->height = -DBL_MAX;
      for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
        v->height = MAX(v->height, e->weight);
    } else {
      v->height = 0;
      v->slack = DBL_MAX;
    }
  }
}

//===================================================================
// Updates the matching by following the parent pointers in the
// M-augmenting path
void updateMatching(vertex *u, vertex *v) {
  while (true) {
    u->match = v;
    v->match = u;
    if (u->parent == u) break;
    v = u->parent;
    u = v->parent;
  } 
}

//===================================================================
// Adjusts the heights of the vertices in the forest and the slack
// of the vertices in the right set not in the forest; the slack is
// the minimum distance separating an unexplored vertex in R from
// being adjacent to an explored vertex in L
void relabel(graph *G, queue **Q) {
    // first, find the minimum slack of the vertices in R that are
    // not in the forest but are adjacent to a vertex in the forest
  double delta = DBL_MAX;
  for (vertex *u = firstV(G); u; u = nextV(G)) 
    if (u->type == RIGHT && u->parent == NULL 
        && u->minParent->parent != NULL)
      delta = MIN(delta, u->slack);   
  
    // adjust the heights of the vertices in the forest
    // and the slack of the vertices in R not in the forest
  for (vertex *u = firstV(G); u; u = nextV(G)) {
    if (u->type == LEFT && u->parent)
      u->height -= delta;
    if (u->type == RIGHT) {
      if (u->parent) 
        u->height += delta;
      else 
        u->slack -= delta;
    }
  } 
}

//===================================================================
// Updates the queue with the vertices in L that correspond to the
// vertices in R with zero slack; if such a vertex in R is unmatched,
// we found an M-augmenting path and update the matching and return
// true; otherwise, we enqueue the corresponding vertex in L
bool checkSlack(graph *G, queue *Q) {
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    if (v->type == RIGHT && v->slack == 0 ) {
      v->slack = DBL_MAX;          // reset the slack
      vertex *u = v->minParent;
      if (! v->match) {
          // we found an M-augmenting path
        updateMatching(u, v);
        freeQueue(Q);
        return true;
      }
      enqueue(Q, u);
    }
  }
  return false;
}

//===================================================================
// Adjusts the slack of the vertices in R not in the forest and
// adjacent to the vertex v
void adjustSlack(graph *G, vertex *v) {
  dll *edges = getNeighbors(G, v);
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
    if (e->to->parent == NULL) {
      double slack = e->to->height + v->height - e->weight;
      if (slack < e->to->slack) {
        e->to->slack = slack;
        e->to->minParent = v;
      }
    }
}

//===================================================================
// Runs a BFS to find an M-augmenting path in the directed equality 
// subgraph. Returns true and updates the matching if such a path is 
// found; otherwise, returns false
bool runBFS(graph *G, queue *Q) {
  vertex *u = dequeue(Q);
  dll *edges = getNeighbors(G, u);
  
    // check each edge (u, v) in the directed equality subgraph
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
    vertex *v = e->to;

    if (v->parent == NULL) {
      if (v->type == LEFT && u->match == v) {
        adjustSlack(G, v);
        v->parent = u;
        enqueue(Q, v);
      }
      
      if (v->type == RIGHT && 
          u->height + v->height - e->weight < 1e-6) {
        if (v->match == NULL) {	
            // we found an M-augmenting path
          updateMatching(u, v);
          return true;
        } else {
          v->parent = u;
          enqueue(Q, v);
        }
      }
    }
  }
  return false;
}

//===================================================================
// Runs the Hungarian algorithm to find the maximum cardinality
// bipartite matching in a weighted graph by finding M-augmenting
// paths in the equality subgraph and updating the matching
void hungarian(graph *G) {

  initHeights(G);
  size_t nMatched = initMatching(G);

    // while the matching is not perfect
    // run a BFS to find an M-augmenting path
  while (nMatched < nVertices(G) / 2) {
    queue *Q = initBFS(G);

    while (true) {
      if (isEmptyQueue(Q)) {
        relabel(G, &Q);
        if (checkSlack(G, Q)) break;
      }
      if (runBFS(G, Q)) break;
    }
    nMatched++;
  }
}

//===================================================================

int main() {

  graph *G = newGraph(50, WEIGHTED);
  setUndirected(G);

  readGraph(G);
  showGraph(G);

  if (!isBipartite(G)) {
    printf("Graph is not bipartite\n");
    freeGraph(G);
    return 0;
  }

  if (nEdges(G) != nVertices(G) * nVertices(G) / 4) {
    printf("The graph is not complete\n");
    freeGraph(G);
    return 0;
  }

  hungarian(G);
  showMatching(G);

  freeGraph(G);
  return 0;
}