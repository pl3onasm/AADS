/* 
  file: dijkstra.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: Dijkstra's shortest paths algorithm
  time complexity: O(E log V) using a binary heap, 
    O(E + V log V) using a Fibonacci heap
  note: make sure to use VERTEX_TYPE2 in the vertex.h file
    by defining it from the command line using
      $ gcc -D VERTEX_TYPE2 ...
*/

#include "../../../datastructures/pqueues/bpqueue.h"
#include "../../../datastructures/graphs/vgraph/graph.h"
#include "../../../lib/clib/clib.h"
#include <float.h>

//===================================================================
// Copies the key (priority) of a node in the priority queue
void *copyKey (void *key) {
  double *k = safeCalloc(1, sizeof(double));
  *k = *(double *)key;
  return k;
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
bpqueue *initPQ(graph *G) {

  bpqueue *pq = bpqNew(nVertices(G), MIN, compareKeys, copyKey, 
                       free, vertexToString);
  
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    v->dist = DBL_MAX;
    bpqPush(pq, v, &v->dist);
  }
  return pq;
}

//===================================================================
// Computes the shortest paths from vertex src to all other nodes
void dijkstra(graph *G, vertex *src) {
    
    // genereate a new priority queue and initialize it
  bpqueue *pq = initPQ(G);
    // set the distance and priority of the source node to 0
  src->dist = 0;
  bpqUpdateKey(pq, src, &src->dist);

  while (! bpqIsEmpty(pq)) {
    vertex *u = bpqPop(pq);
    dll* edges = getNeighbors(G, u);
      
      // try to relax all the neighbors of u that are in the pq
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
      if (bpqContains(pq, e->to) && relax(u, e->to, e->weight)) 
          // update neighbor's priority in the pq if edge was relaxed
        bpqUpdateKey(pq, e->to, &e->to->dist);
  } 
  bpqFree(pq);
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
         "---------------------------------\n",
          src->label);
  printf("Vertex: Parent, Distance from src\n"
         "---------------------------------\n");
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
    
    // read the source node 
  char s[50];
  assert(scanf("%s", s) == 1);    

  graph *G = newGraph(50, WEIGHTED); 
  readGraph(G);              
  showGraph(G);

  vertex *src = getVertex(G, s);

  if (! src) {
    fprintf(stderr, "Source node %s not found.\n", s);
    freeGraph(G);
    exit(EXIT_FAILURE);
  }

  dijkstra(G, src);              
  showDistances(G, src);                 

  freeGraph(G);
  return 0;
}

