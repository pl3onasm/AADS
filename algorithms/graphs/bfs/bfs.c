/* 
  file: bfs.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: breadth-first search 
    with a queue implemented as a circular array and a graph
    implemented as a hash table of vertices.
  note: make sure to use VERTEX_TYPE2 in the vertex.h file
    by defining it from the command line using -D VERTEX_TYPE2
*/

#include "../../../datastructures/graphs/htgraph/vertex.h"
#include "../../../datastructures/graphs/htgraph/graph.h"
#include "../../../datastructures/queues/queue.h"

//==================================================================
// prints the path from the source to the destination
void printPath(vertex *src, vertex *d) {
  if (src == d) 
    printf("%s", src->label);
  else if (! d->parent) 
    printf("There is no path from %s to %s.", src->label, d->label);
  else {
    printPath(src, d->parent);
    printf(", %s", d->label);
  }
}

//==================================================================
// Prints the result of the search
void printResult(graph *G, vertex *src, vertex *d) {
    printf("\nDistance from %s to %s: %d.\n", src->label, 
           d->label, d->dist == 0 ? (d == src ? 0 : -1) : d->dist);  
    printf("Path: ");
    printPath(src, d);
    printf("\n");
}

//==================================================================
// Builds a breadth-first search tree from the source node
void bfs(graph *G, vertex *src) {
  
  queue *q = newQueue(nVertices(G));
  enqueue(q, src);
  src->visited = true;

  while (! qIsEmpty(q)) {
      // get outgoing edges of the node at the front of the queue
    vertex *v = dequeue(q);
    dll *edges = getNeighbors(G, v);

      // for each neighbor, if it has not been visited, 
      // set its parent and its distance from the source
    for (edge *e = firstE(edges); e; e = nextE(edges)) 
      if (! e->to->visited) {
        e->to->parent = v;
        e->to->dist = v->dist + 1;
        e->to->visited = true;
        enqueue(q, e->to);
      }
  }
  freeQueue(q);
}

//==================================================================
// Reads source and destination nodes and builds the graph  

int main () {
    // source and destination nodes
  char s[50], d[50];              
  (void)! scanf("%s %s", s, d);
  
  graph *G = newGraph(50, DIRECTED, UNWEIGHTED);
  
  readGraph(G);
  showGraph(G);

  vertex *src = getVertex(G, s);
  vertex *dest = getVertex(G, d);

  bfs(G, src);
  
  printResult(G, src, dest);

  freeGraph(G);
  return 0;
}