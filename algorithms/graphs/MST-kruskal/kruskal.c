/* 
  file: kruskal.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: implements Kruskal's algorithm to compute the minimum
    spanning tree of a graph. For this, we use a union-find data
    structure to keep track of the connected components and a binary
    min heap to store the edges of the graph and extract the one with
    the smallest weight at each step.
  time complexity: O(E log V)
  note: make sure to use VERTEX_TYPE2 in the vertex.h file and 
    EDGE_TYPE2 in the edge.h file by defining them from the command 
    line using
      $ gcc -D VERTEX_TYPE2 -D EDGE_TYPE2 ...
*/

#include "../../../datastructures/graphs/graph/graph.h"
#include "../../../datastructures/union-find/unionFind.h"
#include "../../../datastructures/heaps/binheaps/binheap.h"

//===================================================================
// Comparison function for the edges
int compareEdges(void const *e1, void const *e2) {
  edge *edge1 = (edge *)e1;
  edge *edge2 = (edge *)e2;
  if (edge1->weight < edge2->weight) return -1;
  if (edge1->weight > edge2->weight) return 1;
  return 0;
}

//===================================================================
// String representation of the data in the union-find structure
char *vertexToString(void const *key) {
  vertex *v = (vertex *)key;
  return v->label;
}

//===================================================================
// Creates a binary heap from the edges of the graph
binheap *initBinHeap(graph *G) {
  binheap *H = bhpNew(nEdges(G), MIN, compareEdges);
  
  vertex *from;
  for (edge *e = firstE(G, &from); e; e = nextE(G, &from)) {
    if (e->reversed) continue;
    e->from = from;
    bhpPush(H, e);
  }
  return H;
}

//===================================================================
// Creates a new union-find data structure with |V| sets:
// one for each vertex in the graph
unionFind *initUnionFind(graph *G) {
  unionFind *sets = ufNew(nVertices(G), vertexToString);
  
  for (vertex *v = firstV(G); v; v = nextV(G)) 
    ufAddSet(sets, v);
  return sets;
}

//===================================================================
// Computes a minimum spanning tree using Kruskal's algorithm
dll *mstKruskal(graph *G) {

  dll *mst = dllNew();
  binheap *H = initBinHeap(G);
  unionFind *sets = initUnionFind(G);
  
    // add |V|-1 edges to the MST without forming a cycle
  while (dllSize(mst) < nVertices(G) - 1) { 
      // get the lightest edge      
    edge *e = bhpPop(H);
  
      // only add e to the MST if the edge connects
      // two different sets, so that no cycle is formed
    if (! ufSameSet(sets, e->from, e->to)) {
      dllPushBack(mst, e);
      ufUnify(sets, e->from, e->to);
    }
  }
  ufFree(sets);
  bhpFree(H);
  return mst;
}

//===================================================================
// Prints the edges forming the minimum spanning tree and its weight
void printMST(graph *G, dll *mst) {
 
  printf("----------------------------\n"
         "MST edges in insertion order \n"
         "----------------------------\n");
  double totalWeight = 0;
  edge *e;
  while ((e = dllPop(mst))) {
    printf("  %s -- %s    %.2lf\n", 
           e->from->label, e->to->label, e->weight); 
    totalWeight += e->weight;
  }
  printf("----------------------------\n"
         "  MST weight: %.2lf\n\n", totalWeight);
}

//===================================================================

int main () {
  
  graph *G = newGraph(50, WEIGHTED); 
  readGraph(G);              
  showGraph(G);

  dll *mst = mstKruskal(G);     
  printMST(G, mst);           

  freeGraph(G);
  dllFree(mst);
  return 0;
}