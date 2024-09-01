/* 
  file: dijkstra-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: Dijkstra's shortest paths algorithm
  time complexity: O(E + V log V) using a Fibonacci heap
  note: make sure to use VERTEX_TYPE2 in the vertex.h file
    by defining it from the command line using
      $ gcc -D VERTEX_TYPE2 ...
*/

#include "../../../datastructures/heaps/fibheaps/fibheap.h"
#include "../../../datastructures/graphs/graph/graph.h"
#include "../../../lib/clib.h"
#include <float.h>

//===================================================================
// Copies the key (priority) of a node in the priority queue
void *copyKey (void const *key) {
  double *copy = safeCalloc(1, sizeof(double));
  *copy = *(double *)key;
  return copy;
}

//===================================================================
// Comparison function for the priority queue
int compareKeys(void const *k1, void const *k2) {
  double d1 = *(double *)k1;
  double d2 = *(double *)k2;
  if (d1 < d2) return -1;
  if (d1 > d2) return 1;
  return 0;
}

//===================================================================
// String representation of the data in the priority queue
char *vertexToString(void const *key) {
  vertex *v = (vertex *)key;
  return v->label;
}

//===================================================================
// Tries to 'relax' the edge (u, v) with weight w
// Returns true if relaxation was successful
bool relax(vertex *u, vertex *v, double w) {
  if (v->dist > u->dist + w) {
    v->dist = u->dist + w;
    v->parent = u;
    return true;
  }
  return false;
}

//===================================================================
// Generates and initializes the min priority queue
// All vertices are added to the priority queue with infinite
// distance from the source node and likewise infinite priority
// The distance and priority of the source node is set to 0
fibheap *initFibHeap(graph *G, vertex *src) {

  fibheap *F = fibNew(MIN, compareKeys, copyKey, 
                      free, vertexToString, NULL);
  
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    v->dist = v == src ? 0 : DBL_MAX;
    fibPush(F, v, &v->dist);
  }
  return F;
}

//===================================================================
// Computes the shortest paths from vertex src to all other nodes
void dijkstra(graph *G, vertex *src) {
    
    // genereate a new priority queue and initialize it
  fibheap *F = initFibHeap(G, src);
  
  while (! fibIsEmpty(F)) {
    vertex *u = fibPop(F);
    dll* edges = getNeighbors(G, u);
      
      // try to relax all the edges from u to its neighbors
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
      if (fibContains(F, e->to) && relax(u, e->to, e->weight)) 
          // update neighbor's priority in the pq if edge was relaxed
        fibUpdateKey(F, e->to, &e->to->dist);
  } 
  fibFree(F);
}

//===================================================================
// Shows the results of the shortest paths computation from src
// displaying the parent and the distance from the source vertex for
// each vertex in the graph; by following the parent pointers, the
// shortest path from the source vertex to any other vertex can be
// reconstructed
void showDistances(graph *G, vertex *src) {
  printf("\nShortest paths\n"
         "Source: %s\n"
         "---------------------------------\n"
         "Vertex: Parent, Distance from src\n"
         "---------------------------------\n",
         src->label);
         
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    printf("  %s: %s, ", v->label, 
           v->parent ? v->parent->label : "NIL");
    if (v->dist == DBL_MAX)
      printf("%s\n", "INF");
    else
      printf("%.2lf\n", v->dist);
  }
  printf("---------------------------------\n\n");
}

//===================================================================

int main () {
    
    // read the label of the source vertex
  char srcL[50];
  assert(scanf("%s", srcL) == 1);    

  graph *G = newGraph(50, WEIGHTED); 
  readGraph(G);              
  showGraph(G);

  vertex *src = getVertex(G, srcL);

  if (! src) {
    fprintf(stderr, "Source node %s not found.\n", srcL);
    freeGraph(G);
    exit(EXIT_FAILURE);
  }

  dijkstra(G, src);              
  showDistances(G, src);                 

  freeGraph(G);
  return 0;
}

