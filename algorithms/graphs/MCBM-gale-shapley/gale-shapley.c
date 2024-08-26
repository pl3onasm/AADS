/* 
  file: gale-shapley.c 
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: preferred Maximum Cardinality Bipartite Matching 
               using Gale-Shapley's algorithm
  time complexity: O(EV)
  note 1: make sure to use VERTEX_TYPE7 in the vertex.h file
    by defining it from the command line using
    $ gcc -D VERTEX_TYPE7 ...
  note 2: matchings may differ as they are not necessarily unique;
    also, the group of vertices that propose to the other group
    is not fixed: another reason why matchings may differ
    In the output, the vertices displayed on the left are the  
    proposers and the ones on the right are the acceptors.
*/

#include "../../../datastructures/graphs/graph/graph.h"
#include "../../../datastructures/queues/queue.h"
#include "../../../lib/clib.h"

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
// Creates a lookup table for the preferences of vertices in the
// right set R, so that we can find, in constant time, the rank of
// a vertex in the left set L when it proposes to a vertex in R
size_t **createLookupTable(graph *G) {
  size_t left = 0, right = 0;
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    if (v->type == LEFT) v->index = left++;
    else v->index = right++;
  }

  CREATE_MATRIX(size_t, table, nVertices(G)/2, nVertices(G)/2, 0);
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    if (v->type == RIGHT) {
      size_t i = 0;
      dll *edges = getNeighbors(G, v);
      for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
        table[v->index][e->to->index] = i++;
    }  
  }
  return table;
}

//===================================================================
// Initializes a queue with the vertices from the left set: these
// are the ones that will propose to the vertices in the right set
queue *initQueue(graph *G) {
  queue *Q = newQueue(nVertices(G));

  for (vertex *v = firstV(G); v; v = nextV(G)) 
    if (v->type == LEFT) 
      enqueue(Q, v);
  
  return Q;
}

//===================================================================
// Gale-Shapley's algorithm for finding a maximum cardinality
// bipartite matching in a graph
void galeShapley(graph *G, size_t **table) {
  queue *Q = initQueue(G);            

    // while there are still proposers 
    // that have not been matched
  while (!isEmptyQueue(Q)) {          
    vertex *u = dequeue(Q);    // get the next proposer
    
    dll *edges = getNeighbors(G, u);
    vertex *v = ((edge *)dllNext(edges))->to; 

    if (! v->match) {          // acceptor not matched?
      u->match = v;            // assign the match
      v->match = u;

    } else if (table[v->index][u->index] 
               < table[v->index][v->match->index]) {
                
      v->match->match = NULL;  // acceptor rejects its current match
      enqueue(Q, v->match);    // so we add it back to the queue
      
      u->match = v;            // assign new match as the proposer
      v->match = u;            // ranks higher in the acceptor's list

    } else {                   // proposer is rejected
      enqueue(Q, u);           // add it back to the queue
    }
  }
  freeQueue(Q);
}

//===================================================================
// Shows the matching: the vertices on the left are the proposers
// and the vertices on the right are the acceptors
void showMatching(graph *G) {
  printf("--------------------\n"
         " Matching\n"
         "--------------------\n");
  for (vertex *v = firstV(G); v; v = nextV(G)) 
    if (v->type == LEFT) 
      printf("  %s -- %s\n", v->label, 
              v->match ? v->match->label : "NIL");
  printf("--------------------\n");
}

//===================================================================
// Reads the input from stdin and adds the vertices and edges to
// the graph; the input is given in the form of a preference list
// for each vertex, with the preferences separated by commas and
// ending with a period
void readInput(graph *G) {
  char from[50], to[50], ch;
    // read first vertex, delimiter is ':'
  while (scanf(" %[^ :] : ", from) == 1) {
      // keep reading the preferences of the current vertex, 
      // delimited by ',' and ending with '.', ignoring white space
    while (scanf("%[^ ,.]", to) == 1) {
      addVandE(G, from, to);
      if (scanf(" %c ", &ch) == 1 && ch == '.') break; 
    }
  }
}

//===================================================================

int main () {

  graph *G = newGraph(50, UNWEIGHTED); 
  readInput(G);
  showGraph(G);

  if (! isBipartiteGraph(G)) {
    printf("The graph is not bipartite\n");
    freeGraph(G);
    return 0;
  }

  if (nEdges(G) != nVertices(G) * nVertices(G) / 2) {
    printf("The graph is not complete\n");
    freeGraph(G);
    return 0;
  }

  size_t **table = createLookupTable(G);

  galeShapley(G, table);

  showMatching(G);

  FREE_MATRIX(table, nVertices(G)/2);
  freeGraph(G);
  return 0;
}

