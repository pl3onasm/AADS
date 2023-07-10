/* file: kruskal.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: implements Kruskal's algorithm to compute the minimum
     spanning tree of a graph. For this, we use a disjoint set data
     structure to keep track of the components.
     Path compression and union by rank are used to improve the
     performance of the disjoint set operations.
   complexity: O(E log V) 
*/

#include <stdio.h>
#include <stdlib.h>

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct set {
  int parent, rank;     // parent and rank of the set
} set;

typedef struct edge {
  int u, v;             // u and v are the endpoints of the edge
  double weight;        // weight of the edge
} edge;

typedef struct graph {
  int nNodes;           // number of nodes in the graph 
  int nEdges, edgeCap;  // number of edges and capacity
  edge **edges;         // array of pointers to edges
} graph;

//::::::::::::::::::::::: memory management :::::::::::::::::::::::://

void *safeCalloc (int n, int size) {
  /* allocates n elements of size size, initializing them to 0, and
     checks whether the allocation was successful */
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *safeRealloc (void *ptr, int newSize) {
  /* reallocates memory and checks whether the allocation was successful */
  ptr = realloc(ptr, newSize);
  if (ptr == NULL) {
    printf("Error: realloc(%d) failed. Out of memory?\n", newSize);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

//::::::::::::::::::::::::: edge functions ::::::::::::::::::::::::://

edge *newEdge(int u, int v, double w) {
  /* creates a new edge */
  edge *e = safeCalloc(1, sizeof(edge));
  e->u = u;
  e->v = v;
  e->weight = w;
  return e;
}

void checkEdgeCap(graph *G) {
  /* checks whether the edge array is full 
     and reallocates memory if necessary */
  if (G->nEdges == G->edgeCap) {
    G->edgeCap = G->edgeCap == 0 ? 10 : 2 * G->edgeCap;
    G->edges = safeRealloc(G->edges, G->edgeCap * sizeof(edge*));
  }
}
 
int compareEdges(const void *a, const void *b) {
  /* compares two edges by weight */
  edge *e1 = *(edge**)a;
  edge *e2 = *(edge**)b;
  if (e1->weight < e2->weight) return -1;
  if (e1->weight > e2->weight) return 1;
  return 0;
}

void printEdge(edge *e) {
  /* prints an edge */
  printf("(%d, %d, %.2lf)\n", e->u, e->v, e->weight);
}

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

graph *newGraph(int n) {
  /* creates a graph with n vertices */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = n;
  return G;
}

void freeGraph(graph *G) {
  /* frees all memory allocated for the graph */
  for (int i = 0; i < G->nEdges; i++) 
    free(G->edges[i]);
  free(G->edges);
  free(G);
}

void buildGraph(graph *G) {
  /* reads edges from stdin and adds them to the graph G */
  int u, v; double w;
  while (scanf("%d %d %lf", &u, &v, &w) == 3) {
    checkEdgeCap(G);
    G->edges[G->nEdges++] = newEdge(u, v, w);
  }
}

//::::::::::::::::::::::::: disjoint sets :::::::::::::::::::::::::://

set *newSet(int root) {
  /* creates a new set */
  set *s = safeCalloc(1, sizeof(set));
  s->parent = root;
  s->rank = 0;
  return s;
}

int find(set **sets, int n) {
  /* finds the root of the set containing node n */
  if (sets[n]->parent != n)
    sets[n]->parent = find(sets, sets[n]->parent);
  return sets[n]->parent;
}

void unionSets(set **sets, int x, int y) {
  /* merges the sets with roots x and y */
  if (sets[x]->rank > sets[y]->rank)  
    sets[y]->parent = x;    // x becomes the new root
  else {
    sets[x]->parent = y;    // y becomes the new root
    if (sets[x]->rank == sets[y]->rank)  
      sets[y]->rank++;     
  }
}

void freeSets(set **sets, int n) {
  /* frees all memory allocated for the sets */
  for (int i = 0; i < n; i++)
    free(sets[i]);
  free(sets);
}

//::::::::::::::::::::::::: mst functions :::::::::::::::::::::::::://

edge **mstKruskal(graph *G) {
  /* computes a minimum spanning tree using Kruskal's algorithm */
  int n = G->nNodes, idx = 0;
  edge **mst = safeCalloc(n, sizeof(edge*));
  set **sets = safeCalloc(n, sizeof(set*));

  // initialize n sets: one for each node
  for (int i = 0; i < n; i++) sets[i] = newSet(i);

  // sort the edges by increasing weight
  qsort(G->edges, G->nEdges, sizeof(edge*), compareEdges);

  // add edges to the minimum spanning tree
  for (int i = 0; i < G->nEdges; i++) {
    if (idx == n-1) break;      // stop if the MST is complete
    edge *e = G->edges[i];     
    int ru = find(sets, e->u);  // find the roots of the sets containing u and v
    int rv = find(sets, e->v);
    if (ru != rv) {  // only add e if u and v are in different sets so that no cycle is formed
      mst[idx++] = e;           // add e to the MST
      unionSets(sets, ru, rv);  // merge the sets containing u and v
    }
  }
  freeSets(sets, n);
  return mst;
}

void printMST(graph *G, edge **mst) {
  /* prints the edges of the MST and its weight */
  double min = 0;
  printf("MST edges in insertion order:\n");
  for (int i = 0; i < G->nNodes-1; i++) {
    min += mst[i]->weight;
    printEdge(mst[i]);
  }
  printf("MST weight: %.2lf\n", min);
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n;                       // n = number of nodes
  scanf("%d", &n); 

  graph *G = newGraph(n); 
  buildGraph(G);               // read edges from stdin

  edge **mst = mstKruskal(G);  // compute minimum spanning tree

  printMST (G, mst);

  freeGraph(G);
  free(mst);
  return 0;
}