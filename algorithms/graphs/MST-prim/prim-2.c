/* 
  file: prim-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: implements Prim's algorithm to compute the minimum
    spanning tree of a graph. For this, we use a MIN fibonacci heap
    to keep track of the edges with the smallest weight. 
  complexity: O(E + V log V) using a Fibonacci heap.
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
// Generates and initializes the min priority queue
// All vertices are added to the priority queue with infinite
// distance from the growing minimum spanning tree and likewise
// infinite priority. An arbitrary vertex is selected as the
// source vertex and its distance and priority are set to 0
fibheap *initFibHeap(graph *G) {

  fibheap *F = fibNew(MIN, compareKeys, copyKey, 
                      free, vertexToString, NULL);
  
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    v->dist = DBL_MAX;
    fibPush(F, v, &v->dist);
  }
    // since the graph is implemented as a hash table,
    // the first vertex is arbitrary
  vertex *s = firstV(G);   
  s->dist = 0;
  fibUpdateKey(F, s, &s->dist);  
  return F;
}

//===================================================================
// Computes a minimum spanning tree of G using Prim's algorithm
dll *mstPrim(graph *G) {
 
  dll *mst = dllNew();
  fibheap *F = initFibHeap(G);    

  while (!fibIsEmpty(F)) {
    vertex *u = fibPop(F);    
    
      // add the vertex to the MST if it's not the source vertex
    if (u->parent) dllPushBack(mst, u);
    dll *edges = getNeighbors(G, u);

      // try to relax the edges to the non-tree neighbors of u
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
      if (fibContains(F, e->to) && e->weight < e->to->dist) {
        e->to->parent = u;
        e->to->dist = e->weight;
        fibUpdateKey(F, e->to, &e->weight);
      }
    }
  }
  fibFree(F);
  return mst;
}

//===================================================================
// Prints the edges forming the minimum spanning tree and its weight
void printMST(graph *G, dll *mst) {
 
  printf("----------------------------\n"
         "MST edges in insertion order \n"
         "----------------------------\n");
  double totalWeight = 0;
  vertex *u;
  while ((u = dllPop(mst))) {
    printf("  %s -- %s    %.2lf\n", 
           u->parent->label, u->label, u->dist);
    totalWeight += u->dist;
  }
  printf("----------------------------\n"
         "  MST weight: %.2lf\n\n", totalWeight);
}

//===================================================================

int main () {
  
  graph *G = newGraph(50, WEIGHTED); 
  readGraph(G);              
  showGraph(G);

  dll *mst = mstPrim(G);      
  printMST(G, mst);           

  freeGraph(G);
  dllFree(mst);
  return 0;
}