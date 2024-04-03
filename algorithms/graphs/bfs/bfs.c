/* 
  file: bfs.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: breadth-first search 
    with a queue implemented as a circular array and a graph
    implemented as a hash table of vertices.
  note: make sure to use VERTEX_TYPE2 in the vertex.h file
    by defining it from the command line using
    $ gcc -D VERTEX_TYPE2 ...
*/

#include "../../../datastructures/graphs/vgraph/graph.h"
#include "../../../datastructures/queues/queue.h"

//===================================================================
// prints the path from the source to the destination
void printPath(vertex *src, vertex *d) {
  if (src == d) 
    printf("%s", src->label);
  else if (! d->parent) 
    printf("no path from %s to %s.", src->label, d->label);
  else {
    printPath(src, d->parent);
    printf(", %s", d->label);
  }
}

//===================================================================
// Prints the result of the search
void printResult(graph *G, vertex *src, vertex *d) {
    if (! d->parent) 
      printf("\nDistance from %s to %s: INF.\n", 
              src->label, d->label);
    else 
      printf("\nDistance from %s to %s: %g\n", 
              src->label, d->label, d->dist);
    printf("Path: ");
    printPath(src, d);
    printf("\n");
}

//===================================================================
// Builds a breadth-first search tree from the source node
void bfs(graph *G, vertex *src) {
  
  queue *q = newQueue(nVertices(G));
  enqueue(q, src);

  while (! isEmptyQueue(q)) {
      // get outgoing edges of the node at the front of the queue
    vertex *v = dequeue(q);
    dll *edges = getNeighbors(G, v);

      // for each neighbor, if it has not been visited, 
      // set its parent and its distance from the source
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
      if (! e->to->parent) {
        e->to->parent = v;
        e->to->dist = v->dist + 1;
        enqueue(q, e->to);
      }
  }
  freeQueue(q);
}

//===================================================================
 
int main () {
    // source and destination nodes
  char s[50], d[50];              
  (void)! scanf("%s %s", s, d);
  
  graph *G = newGraph(50, UNWEIGHTED);
  
  readGraph(G);
  showGraph(G);

  vertex *src = getVertex(G, s);
  vertex *dest = getVertex(G, d);

  if (! src || ! dest) {
    fprintf(stderr, "Error: source or destination node not found.\n");
    freeGraph(G);
    return 1;
  }

  bfs(G, src);
  
  printResult(G, src, dest);

  freeGraph(G);
  return 0;
}