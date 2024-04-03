/* file: dijkstra-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Dijkstra's shortest paths algorithm
     using a priority queue implemented with a binary heap
*/

#define VERTEX_TYPE2
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
// Initializes the priority queue
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
// Computes the shortest paths from node s to all other nodes
void dijkstra(graph *G, vertex *src) {
    
    // genereate a new priority queue and initialize it
  bpqueue *pq = initPQ(G);
    // set the distance and priority of the source node to 0
  src->dist = 0;
  bpqUpdateKey(pq, src, &src->dist);

  while (! bpqIsEmpty(pq)) {
    vertex *u = bpqPop(pq);
    dll* edges = getNeighbors(G, u);
      
      // relax all the neighbors of u that are in the pq
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
      if (bpqContains(pq, e->to) && relax(u, e->to, e->weight)) 
          // update neighbor's priority in the pq if edge was relaxed
        bpqUpdateKey(pq, e->to, &e->to->dist);
  } 
  bpqFree(pq);
}

//===================================================================
// Shows the results of the shortest paths computation from src
void showDistances(graph *G, vertex *src) {
  printf("\nShortest paths\n"
         "Source: %s\n"
         "---------------------------------\n",
          src->label);
  printf("Vertex: Parent, Distance from src\n"
         "---------------------------------\n");
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    printf("%s: %s, %.2lf\n", v->label, 
           v->parent ? v->parent->label : "NIL", v->dist);
  }
  printf("\n");
}

//===================================================================

int main (int argc, char *argv[]) {
    
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
    return 1;
  }

  dijkstra(G, src);              
  showDistances(G, src);                 

  freeGraph(G);
  return 0;
}
