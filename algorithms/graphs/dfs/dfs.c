/* 
  file: dfs.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: depth-first search,
    with a queue implemented as a circular array and a graph
    implemented as a hash table of vertices.
  input: directed graph 
  output: discovery and finish times for each node and
          the edge types (tree, back, forward, cross)
  note: make sure to use VERTEX_TYPE1 in the vertex.h file
        and EDGE_TYPE1 in the edge.h file by defining them
        from the command line when compiling:
        $ gcc -D VERTEX_TYPE1 -D EDGE_TYPE1 ...
*/

#include "../../../datastructures/graphs/vgraph/graph.h"
#include "../../../datastructures/queues/queue.h"

//===================================================================
// Prints the discovery and finish times of the nodes in the graph G
// and the type of each edge
void printResults(graph *G) {

  printf("\n\nVERTEX DISCOVERY AND FINISH TIMES\n"
         "---------------------------------\n"
         "Label: discovery time, finish time\n\n");

  for (vertex *v = firstV(G); v; v = nextV(G)) 
    printf("   %s: %zu, %zu\n", v->label, v->dTime, v->fTime);

  printf("\n\nEDGE CLASSIFICATION\n"
         "-------------------\n"
         "Source -> Destination: edge type\n\n");

  vertex *src;
  for (edge *e = firstE(G, &src); e; e = nextE(G, &src)) {
      printf("   %s -> %s:  %c\n", 
              src->label, e->to->label, e->type);
  }
  printf("\n");
}

//===================================================================
// Visits the node u and its descendants, marking the edges on the
// way as tree, back, forward or cross edges
void dfsVisit(graph *G, vertex *u, size_t *time) {
  u->dTime = ++*time;
  u->color = GRAY;            // u is discovered: turns gray

  dll *edges = getNeighbors(G, u);
  
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
    vertex *v = e->to;
    
    if (v->color == WHITE) {  // if v is undiscovered
      e->type = TREE;         // then u->v is a tree edge
      v->parent = u;
      dfsVisit(G, v, time);
    } 

    else if (v->color == GRAY)
      e->type = BACK;         // v is an ancestor of u 
                              // in the same dfs tree
    else if (v->dTime > u->dTime)  
      e->type = FORWARD;      // v is a descendant of u 
                              // in the same dfs tree
    else 
      e->type = CROSS;        // rest of the cases; 
                              // v can be in another dfs tree
  }
  u->fTime = ++*time;
  u->color = BLACK;           // u is finished: turns black
}

//===================================================================
// Builds a depth-first search forest for the graph G
// Since the graph is implemented as a hash table, the
// graph traversal and thus the corresponding edge 
// classification may differ from one run to another
void dfs(graph *G, size_t *time) {
  
    // Discover all white nodes in the graph
  for (vertex *v = firstV(G); v; v = nextV(G)) 
    if (v->color == WHITE)      
      dfsVisit(G, v, time);
}

//===================================================================

int main (int argc, char *argv[]) {
  size_t time = 0;

  graph *G = newGraph(50, UNWEIGHTED);

  readGraph(G);    
  showGraph(G);

  dfs(G, &time);  
  printResults(G);

  freeGraph(G);
  return 0;
}