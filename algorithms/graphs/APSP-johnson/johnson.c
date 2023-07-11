/* file: johnson.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Johnson's algorithm for all-pairs shortest 
     paths using a binary min-heap
   assumption: nodes are numbered 0..n-1
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define LEFT(i)   (2*i + 1)
#define RIGHT(i)  (2*i + 2)
#define PARENT(i) ((i-1)/2)
#define INF DBL_MAX

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct list list;    // forward declaration

typedef enum {               // definition for boolean type
  false = 0,
  true = 1
} bool;

typedef struct node {
  // graph-related fields
  int id, parent;            // node id and parent id
  double dDist;              // distance to source in Dijkstra
  double bDist;              // distance to source in Bellman-Ford
  list *adj;                 // adjacency list

  // heap-related fields
  int heapIndex;             // index in the heap
  bool inHeap;               // true if node is in the heap
  double key;                // key used to sort the heap
} node;

struct list {                
  node *n;                   // pointer to a node in the graph
  list *next;                // pointer to next node in the list
  double w;                  // weight of incoming edge at node n
  double rw;                 // reweighted weight of incoming edge in G'
};                           

typedef struct heap {
  int nNodes;                // number of elements in the heap
  node **nodes;              // array of pointers to nodes
} heap;

typedef struct graph {
  int nNodes, nEdges;        // number of nodes and edges 
  node **nodes;              // array of pointers to nodes
  double **D;                // distance matrix for G
  int **P;                   // predecessor matrix for G
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

void **newMatrix (int m, int n, size_t eSize) {
  /* allocates a m x n matrix */
  void **M = (void**) safeCalloc(m, sizeof(void *));
  for (int i = 0; i < m; i++) 
    M[i] = safeCalloc(n, eSize);
  return M;
}

//::::::::::::::::::::::::: list functions ::::::::::::::::::::::::://

void freeList(list *L) {
  /* frees all memory allocated for the list */
  if (!L) return;
  freeList(L->next);
  free(L);
}

list *listInsert (list *L, node *n, double w) {
  /* inserts the node n at the beginning of the list L */
  list *new = safeCalloc(1, sizeof(list));
  new->n = n;
  new->next = L;
  new->w = new->rw = w;
  return new;
}

//::::::::::::::::::::::::: graph functions :::::::::::::::::::::::://

graph *newGraph(int n) {
  /* creates a new graph with nNodes nodes */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = n;
  G->nodes = (node **) newMatrix(n, n, sizeof(node));
  // create distance matrix, excluding source of G'
  G->D = (double **) newMatrix(n-1, n-1, sizeof(double));
  // create predecessor matrix, excluding source of G'
  G->P = (int **) newMatrix(n-1, n-1, sizeof(int));
  // initialize nodes
  for (int i = 0; i < n; i++) {
    G->nodes[i]->id = i;
    G->nodes[i]->parent = -1;
    G->nodes[i]->dDist = INF;
    G->nodes[i]->bDist = INF;
  }
  return G;
}

void buildGraph (graph *G) {
  /* builds the graphs G and G' from the input; G' is an extension of G
     with an extra node s connected to all other nodes with edges of 
     weight 0 */
  int u, v; double w;
  while (scanf("%d %d %lf", &u, &v, &w) == 3) {
    node *n = G->nodes[u];
    n->adj = listInsert(n->adj, G->nodes[v], w);
    G->nEdges++;
  }
  // we add edges with weight 0 from s to all other nodes
  int s = G->nNodes-1;  // we use the last node as source
  for (int i = 0; i < G->nNodes-1; i++) 
    G->nodes[s]->adj = listInsert(G->nodes[s]->adj, G->nodes[i], 0);
}

void freeGraph(graph *G) {
  /* frees the memory allocated to the graph G */
  for (int i = 0; i < G->nNodes; i++) {
    freeList(G->nodes[i]->adj);
    free(G->nodes[i]);
  }
  free(G->nodes);
  for (int i = 0; i < G->nNodes-1; i++) {
    free(G->D[i]);
    free(G->P[i]);
  }
  free(G->D);
  free(G->P);
  free(G);
}

void printD (graph *G) {
  /* prints a n x n distance matrix */
  printf("Distance matrix D:\n");
  for (int i = 0; i < G->nNodes-1; i++) {
    for (int j = 0; j < G->nNodes-1; j++) {
      if (G->D[i][j] == INF) printf("%5s ", "inf");
      else printf("%5.1lf ", G->D[i][j]);
    }
    printf("\n");
  }
}

void printP(graph *G) {
  /* prints a n x n predecessor matrix */
  printf("\nPredecessor matrix P:\n");
  for (int i = 0; i < G->nNodes-1; i++) {
    for (int j = 0; j < G->nNodes-1; j++) {
      if (G->P[i][j] == -1) printf("%3c ", '-');
      else printf("%3d ", G->P[i][j]);
    }
    printf("\n");
  }
}

void printPath (int **P, int i, int j) {
  /* prints the shortest path from i to j */
  if (j == i) printf("%d", i);
  else {
    printPath(P, i, P[i][j]);
    printf(" → %d", j);
  }
}

//::::::::::::::::::::::::: heap functions ::::::::::::::::::::::::://

void swap (heap *H, int i, int j) {
  /* swaps the nodes at indices i and j in the heap */
  node *tmp = H->nodes[i];
  H->nodes[i] = H->nodes[j];
  H->nodes[j] = tmp;
  // update the heapIndex fields
  H->nodes[i]->heapIndex = i;
  H->nodes[j]->heapIndex = j;
}

heap *newHeap(graph *G, int s) {
  /* creates a heap with n nodes */
  int n = G->nNodes-1;  // exclude the source node of G'
  heap *H = safeCalloc(1, sizeof(heap));
  H->nNodes = n;
  H->nodes = safeCalloc(n, sizeof(node*));
  // make pointer copies of the graph nodes 
  // and initialize the heap-related fields
  for (int i = 0; i < n; i++){
    node *u = G->nodes[i];
    H->nodes[i] = u;
    u->heapIndex = i;
    u->inHeap = true;
    u->key = u->dDist;
  }
  // swap the source node with the first node
  // all other nodes have key = INF at this point
  swap(H, 0, s);
  return H;
}

void freeHeap (heap *H) {
  /* frees the heap */
  free(H->nodes);
  free(H);
}

void minHeapify(heap *H, int i){
  /* restores the min heap property in a top-down manner */
  int min = i, l = LEFT(i), r = RIGHT(i);
  if (l < H->nNodes && H->nodes[l]->key < H->nodes[i]->key)
    min = l;
  if (r < H->nNodes && H->nodes[r]->key < H->nodes[min]->key)
    min = r;
  if (min != i) {
    swap(H, i, min); 
    minHeapify(H, min);
  }
}

void decreaseKey(heap *H, int i, double newKey){
  /* decreases the key of the node at index i to newKey */
  if (newKey > H->nodes[i]->key) {
    printf("Error: new key is greater than current key\n");
    exit(EXIT_FAILURE);
  }
  H->nodes[i]->key = newKey;
  while (i > 0 && H->nodes[PARENT(i)]->key > H->nodes[i]->key) {
    swap(H, i, PARENT(i));
    i = PARENT(i);
  }
}

node *extractMin(heap *H) {
  /* extracts the node with minimum key from the heap */
  node *min = H->nodes[0];
  H->nodes[0] = H->nodes[--H->nNodes];
  minHeapify(H, 0);
  min->inHeap = false;
  return min;
}

//:::::::::::::::::::::: johnson functions ::::::::::::::::::::::::://

void bFord(graph *G) {
  /* computes the shortest paths from node s to all other nodes */
  node *s = G->nodes[G->nNodes-1];
  s->bDist = 0;    // set the source distance to 0
  for (int i = 0; i < G->nNodes-1; i++) 
    // relax all the edges n-1 times
    for (int j = 0; j < G->nNodes; j++) {
      node *u = G->nodes[j];
      for (list *a = u->adj; a; a = a->next)
        if (u->bDist + a->w < a->n->bDist) 
          a->n->bDist = u->bDist + a->w;
    }
  // check for negative cycles
  for (int j = 0; j < G->nNodes; j++)
    for (list *a = G->nodes[j]->adj; a; a = a->next) 
      if (G->nodes[j]->bDist + a->w < a->n->bDist) {
        printf("Negative cycle detected\n");
        exit(EXIT_FAILURE);
      }
}

bool relax(node *u, node *v, double w) {
  /* relaxes the edge (u,v) with weight w */
  if (v->dDist > u->dDist + w) {
    v->dDist = u->dDist + w;
    v->parent = u->id;
    return true;
  }
  return false;
}

void dijkstra(graph *G, int s) {
  /* computes the shortest paths from node s to all other nodes */
  G->nodes[s]->dDist = 0;    // set the source distance to 0
  heap *H = newHeap(G, s);   // create a new min-heap with source at root

  while (H->nNodes > 0) {
    node *u = extractMin(H);

    // relax all the neighbors of u that are in the heap (V-S)
    for (list *a = u->adj; a; a = a->next){
      node *v = a->n;
      if (v->inHeap && relax(u, v, a->rw))
        decreaseKey(H, v->heapIndex, v->dDist);
          // update v's key and position in the heap
    }
  }
  freeHeap(H);
}

void johnson(graph *G) {
  /* computes the distance matrix and the predecessor matrix of G */
  // compute the bellman-ford distances for all nodes 
  bFord(G);
  // reweight G.E to get G.E'
  for (int i = 0; i < G->nNodes; i++) 
    for (list *a = G->nodes[i]->adj; a; a = a->next) 
      a->rw += G->nodes[i]->bDist - a->n->bDist;
  // compute shortest paths from each node in G using G.E'
  // and update the distance and predecessor matrices
  for (int i = 0; i < G->nNodes-1; i++) {
    dijkstra(G, i);  // compute shortest paths from node i
    for (int j = 0; j < G->nNodes-1; j++) {
      // update G.D for edges i -> j
      G->D[i][j] = G->nodes[j]->dDist + 
        G->nodes[j]->bDist - G->nodes[i]->bDist;  
      // update G.P for edges i -> j
      G->P[i][j] = G->nodes[j]->parent;
      G->nodes[j]->dDist = INF;  // reset the distances
      G->nodes[j]->parent = -1;  // and parents
    }
  }
}

void answerQueries (graph *G) {
  /* prints the queries and their answers */
  scanf("%*s");   // skip "queries" in input     
  int s = 0, g = 0, q = 0;
  printf("\nQuery results:\n");
  while (scanf("%d %d", &s, &g) == 2) {
    printf("%3d: ", ++q); 
    if (G->D[s][g] == INF) 
      printf("There is no path from %d to %d.", s, g);
    else {
      printf("Shortest path from %d to %d has length = %4.2lf\n" 
             "     Path: ", s, g, G->D[s][g]);
      printPath(G->P, s, g);
    }
    printf("\n");
  }
}

//::::::::::::::::::::::::::::: main ::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n;                       // n = number of nodes
  scanf("%*s %d", &n);         // skip "graph" and read n
 
  graph *G = newGraph(n+1);    // graph G with n nodes + source for G'
  buildGraph(G);               // build G and G' from input

  johnson(G);                  // compute distance and predecessor matrices
  
  printD(G);                   // print distance matrix
  printP(G);                   // print predecessor matrix
  answerQueries(G);            // solve queries

  freeGraph(G);
  return 0;
}
