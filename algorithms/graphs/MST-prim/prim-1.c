/* file: prim-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: implements Prim's algorithm to compute the minimum
     spanning tree of a graph. For this, we use a min priority queue 
     to keep track of the edges with the smallest weight. In this
     version, we use a binary min heap to implement the priority 
     queue.
   complexity: O(E log V) 
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define LEFT(i)   (2*i + 1)
#define RIGHT(i)  (2*i + 2)
#define PARENT(i) ((i-1)/2)
#define INF DBL_MAX

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct list list; // forward declaration

typedef enum {            // definition for boolean type
  false = 0,
  true = 1
} bool;

typedef struct node {
  // graph-related fields
  int id, parent;         // node id and id of parent in the MST
  list *adj;              // adjacency list
  bool reversed;          // true if the MST edge has reverse input order

  // heap-related fields
  double key;             // keeps track of the minimum weight
  int heapIndex;          // node index in the heap
  bool inHeap;            // true if node is in the heap
} node;

typedef struct graph {
  int nNodes, nEdges;     // number of nodes and edges in the graph
  node **vertices;        // array of pointers to nodes
} graph;

struct list {             // adjacency list, singly linked
  node *n;                
  list *next;
  double w;               // weight of the incident edge
  bool reversed;          // true if the edge has reverse input order
};

typedef struct heap {     // binary min heap
  node **nodes;           // array of pointers to nodes
  int nNodes, nodeCap;    // number of nodes and capacity
} heap;

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

//::::::::::::::::::::::::: list functions ::::::::::::::::::::::::://

list *newList() {
  /* creates an empty list */
  return NULL;
}

void freeList(list *L) {
  /* frees all memory allocated for the list */
  if (L) {
    freeList(L->next);
    free(L);
  }
}

list *listInsert (list *L, node *n, double w, bool reversed) {
  /* inserts the node n at the beginning of the list L */
  list *new = safeCalloc(1, sizeof(list));
  new->n = n;
  new->next = L;
  new->w = w;
  new->reversed = reversed;
  return new;
}

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a node with given id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
  n->parent = -1;   // -1 means no parent
  n->adj = newList();
  return n;
}

graph *newGraph(int n) {
  /* creates a graph with n vertices */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = n;
  G->vertices = safeCalloc(n, sizeof(node*));
  for (int i = 0; i < n; i++)
    G->vertices[i] = newNode(i);
  return G;
}

void freeGraph(graph *G) {
  /* frees all memory allocated for the graph */
  for (int i = 0; i < G->nNodes; i++) {
    freeList(G->vertices[i]->adj);
    free(G->vertices[i]);
  }
  free(G->vertices);
  free(G);
}

void buildGraph(graph *G) {
  /* reads undirected graph from stdin and builds the adjacency lists */
  int u, v; double w;
  while (scanf("%d %d %lf", &u, &v, &w) == 3) {
    node *n = G->vertices[u];
    n->adj = listInsert(n->adj, G->vertices[v], w, false);
    n = G->vertices[v];
    n->adj = listInsert(n->adj, G->vertices[u], w, true);
    G->nEdges++;
  }
}

//::::::::::::::::::::::::: heap functions ::::::::::::::::::::::::://

heap *newHeap(graph *G) {
  /* creates a heap with n nodes */
  int n = G->nNodes;
  heap *hp = safeCalloc(1, sizeof(heap));
  hp->nNodes = n;
  hp->nodeCap = n;
  hp->nodes = safeCalloc(n, sizeof(node*));
  // make pointer copies of the graph nodes 
  // and initialize the heap-related fields
  for (int i = 0; i < n; i++){
    hp->nodes[i] = G->vertices[i];
    hp->nodes[i]->heapIndex = i;
    hp->nodes[i]->key = INF;
    hp->nodes[i]->inHeap = true;
  }
  return hp;
}

void freeHeap (heap *H) {
  /* frees the heap */
  free(H->nodes);
  free(H);
}

void swap (heap *H, int i, int j) {
  /* swaps the nodes at indices i and j in the heap */
  node *tmp = H->nodes[i];
  H->nodes[i] = H->nodes[j];
  H->nodes[j] = tmp;
  // update the heapIndex fields
  H->nodes[i]->heapIndex = i;
  H->nodes[j]->heapIndex = j;
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

//::::::::::::::::::::::::: mst functions :::::::::::::::::::::::::://

int *mstPrim(graph *G) {
  /* computes a minimum spanning tree of G using Prim's algorithm */
  int *M = safeCalloc(G->nNodes, sizeof(int)), idx = 0;
  heap *H = newHeap(G);     // at the start all nodes are in the heap
  H->nodes[0]->key = 0;     // we start the MST from node 0

  while (H->nNodes > 0) {
    node *u = extractMin(H);  // get node incident to the MST with min key
    if (u->parent >= 0) M[idx++] = u->id;  // add it to the MST
   
    // iterate over u's non-tree neighbors v and update if necessary
    for (list *l = u->adj; l; l = l->next) {
      node *v = l->n;
      if (v->inHeap && l->w < v->key) {  
        v->parent = u->id;            // set v's parent to u
        v->reversed = l->reversed;    // set v's reversed flag
        decreaseKey(H, v->heapIndex, l->w);
          // update v's key and position in the heap
      }
    }
  }
  freeHeap(H);
  return M;
}

void printMST(graph *G, int *M) {
  /* prints the edges of the MST and its total weight */
  printf("MST edges in insertion order:\n");
  double totalWeight = 0;
  for (int i = 0; i < G->nNodes - 1; i++) {
    node *u = G->vertices[M[i]];
    // print the edge in the original input order
    if (u->reversed) printf("%d %d %.2lf\n", u->id, u->parent, u->key);
    else printf("%d %d %.2lf\n", u->parent, u->id, u->key);
    totalWeight += u->key;
  }
  printf("MST weight: %.2lf\n", totalWeight);
}

//::::::::::::::::::::::::::::: main ::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n;                    // n = number of nodes
  scanf("%d", &n); 

  graph *G = newGraph(n); 
  buildGraph(G);            // read edges from stdin

  int *M = mstPrim(G);      // compute MST
  printMST(G, M);           // print MST edges and total weight

  freeGraph(G);
  free(M);
  return 0;
}