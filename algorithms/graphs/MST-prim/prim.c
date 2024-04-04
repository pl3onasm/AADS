/* 
  file: prim-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: implements Prim's algorithm to compute the minimum
    spanning tree of a graph. For this, we use a min priority queue 
    to keep track of the edges with the smallest weight. 
  complexity: O(E log V) using a binary heap. Using a Fibonacci heap
    reduces the complexity to O(E + V log V)
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
// Generates and initializes the min priority queue
// All vertices are added to the priority queue with infinite
// distance from the growing minimum spanning tree and likewise
// infinite priority
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
// Computes a minimum spanning tree of G using Prim's algorithm
vertex **mstPrim(graph *G) {
 
  vertex **mst = safeCalloc(nVertices(G), sizeof(vertex *));
  bpqueue *pq = initPQ(G);    
  
    // start with an arbitrary vertex and set 
    // its distance and priority to 0
  vertex *s = firstV(G);   
  s->dist = 0;
  bpqUpdateKey(pq, s, &s->dist);  
  size_t idx = 0;

  while (!bpqIsEmpty(pq)) {
    vertex *u = bpqPop(pq);    
    
      // add the vertex to the MST if parent is set
      // (i.e., if the vertex is not the source vertex)
    if (u->parent) mst[idx++] = u;  
    dll *edges = getNeighbors(G, u);

    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
        
      if (bpqContains(pq, e->to) && e->weight < e->to->dist) {
        e->to->parent = u;
        e->to->dist = e->weight;
        bpqUpdateKey(pq, e->to, &e->weight);
      }
    }
  }
  bpqFree(pq);
  return mst;
}

//===================================================================
// Prints the edges forming the minimum spanning tree and its weight
void printMST(graph *G, vertex **mst) {
 
  printf("----------------------------\n"
         "MST edges in insertion order \n"
         "----------------------------\n");
  double totalWeight = 0;
  for (size_t i = 0; i < nVertices(G) - 1; i++) {
    vertex *u = mst[i];
    vertex *v = u->parent;
    printf("%s -- %s\n", v->label, u->label);
    totalWeight += u->dist;
  }
  printf("----------------------------\n"
         "MST weight: %.2lf\n\n", totalWeight);
}

//===================================================================

int main (int argc, char *argv[]) {
  
  graph *G = newGraph(50, WEIGHTED); 
  readGraph(G);              
  showGraph(G);

  vertex **mst = mstPrim(G);      
  printMST(G, mst);           

  freeGraph(G);
  free(mst);
  return 0;
}