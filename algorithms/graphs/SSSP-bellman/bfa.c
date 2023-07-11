/* file: bfa.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Bellman-Ford algorithm for single-source shortest paths
   time complexity: O(VE)
   assumption: nodes are numbered 0..n-1
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#define INF DBL_MAX

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct node {
  int id, parent;        // node id and predecessor
  double dist;           // distance to the source 
} node;

typedef struct edge {
  node *to, *from;       // edge endpoints
  double w;              // edge weight
} edge;

typedef enum {
  false = 0,
  true = 1
} bool;

typedef struct graph {
  int nNodes, nEdges;    // number of nodes and edges
  edge **edges;          // array of pointers to edges
  node **nodes;          // array of pointers to nodes
  int edgeCap;           // capacity of the edges array
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

void checkEdgeCap(graph *G) {
  /* checks whether the edge array is full 
     and reallocates memory if necessary */
  if (G->nEdges == G->edgeCap) {
    G->edgeCap = G->edgeCap == 0 ? 10 : 2 * G->edgeCap;
    G->edges = safeRealloc(G->edges, G->edgeCap * sizeof(edge*));
  }
}

edge *newEdge(graph *G, int u, int v, double w) {
  /* creates a new edge */
  edge *e = safeCalloc(1, sizeof(edge));
  e->from = G->nodes[u];
  e->to = G->nodes[v];
  e->w = w;
  return e;
}

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a node with given id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
  n->parent = -1;   // -1 means no parent
  n->dist = INF;    // initially, all nodes are unreachable
  return n;
}

graph *newGraph(int n) {
  /* creates a graph with n vertices */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = n;
  G->nodes = safeCalloc(n, sizeof(node*));
  for (int i = 0; i < n; i++)
    G->nodes[i] = newNode(i);
  return G;
}

void buildGraph (graph *G){
  /* adds an edge to the graph */
  int u, v; double w;
  while (scanf("%d %d %lf", &u, &v, &w) == 3) {
    checkEdgeCap(G);
    G->edges[G->nEdges++] = newEdge(G, u, v, w);
  }
}

void freeGraph(graph *G) {
  /* frees the memory allocated for the graph */
  for (int i = 0; i < G->nNodes; i++)
    free(G->nodes[i]);
  free(G->nodes);
  for (int i = 0; i < G->nEdges; i++)
    free(G->edges[i]);
  free(G->edges);
  free(G);
}

//:::::::::::::::::::::::::: bellman-ford :::::::::::::::::::::::::://

void print (graph *G, int s) {
  /* prints the distances and parents of the nodes */
  printf("Node   Distance   Parent\n"); 
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->nodes[i];
    if (n->id == s) printf("%4s", "src");
    else printf("%4d", n->id);
    if (n->dist == INF)
      printf("%11s", "inf");
    else printf("%11.2lf", n->dist);
    if (n->parent == -1)
      printf("%9c\n", '-');
    else if (n->parent == s)
      printf("%9s\n", "src");
    else printf("%9d\n", n->parent);
  }
}

bool relax(graph *G, edge *e) {
  /* relaxes the edge e */
  if (e->to->dist > e->from->dist + e->w) {   // found a shorter path
    e->to->dist = e->from->dist + e->w;       // update estimate
    e->to->parent = e->from->id;
    return true;
  }
  return false;
}

void bFord(graph *G, int s) {
  /* runs the Bellman-Ford algorithm on graph G starting from node s
      and checks whether there is a negative-weight cycle */
  G->nodes[s]->dist = 0;
  for (int i = 0; i < G->nNodes - 1; i++)   // relax all edges n-1 times
    for (int j = 0; j < G->nEdges; j++) 
      relax(G, G->edges[j]);

  for (int i = 0; i < G->nEdges; i++)       // check for negative cycles
    if (relax(G, G->edges[i])) {
      printf("Negative-weight cycle found.\n");
      exit(EXIT_FAILURE);
    } 
}

//::::::::::::::::::::::::::::: main ::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n, s;                   // n = number of nodes 
  scanf("%d %d", &n, &s);     // s = source node

  graph *G = newGraph(n); 
  buildGraph(G);              // read edges from stdin

  bFord(G, s);                // run Bellman-Ford algorithm
  print(G, s);                // print results

  freeGraph(G);
  return 0;
}
