/* file: mcbm-2.c  
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Maximum Cardinality Bipartite Matching (MCBM) 
                using Hopcroft-Karp's algorithm
   time complexity: O(E√V)
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef enum {            // definition for boolean type
  false = 0,
  true = 1
} bool;

typedef struct node {
  int id;                 // id of the node
  struct node **adj;      // adj list is an array of pointers to nodes
  int adjCap;             // capacity of the adjacency list
  int nAdj;               // number of adjacent nodes
  int dist;               // distance from source in the BFS tree
  struct node* matched;   // pointer to the matched node
} node;

typedef struct graph {
  node **nodes;           // array of pointers to nodes
  int nL, nR;             // number of nodes in the left and right partition
  node *nil;              // used to mark unmatched nodes and as a sentinel
  int nMatched;           // number of matched nodes
} graph;

typedef struct queue {
  int front, back, size;  // front and back of the queue, and its size
  node **array;           // array of pointers to nodes
} queue;

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

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a node with given id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
  n->dist = INF;
  return n;
}

graph *newGraph(int left, int right) {
  /* creates a graph with n vertices */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nil = newNode(-1);
  G->nL = left;
  G->nR = right;
  G->nodes = safeCalloc(left + right, sizeof(node*));
  for (int i = 0; i < left + right; i++)
    G->nodes[i] = newNode(i);
  return G;
}

void freeGraph(graph *G) {
  /* frees all memory allocated for the graph */
  for (int i = 0; i < G->nL + G->nR; i++) {
    free(G->nodes[i]->adj);
    free(G->nodes[i]);
  }
  free(G->nodes);
  free(G->nil);
  free(G);
}

void addEdge(graph *G, int uId, int vId) {
  /* adds an edge from u to v to the graph by 
     inserting v into the adjacency list of u */
  node *u = G->nodes[uId];
  node *v = G->nodes[vId];
  u->matched = G->nil;  
  v->matched = G->nil;
  // check if we need to resize the adjacency list
  if (u->adjCap == u->nAdj) {
    u->adjCap += 10; 
    u->adj = safeRealloc(u->adj, u->adjCap * sizeof(node*));
  }
  u->adj[u->nAdj++] = v; 
}

void buildGraph(graph *G) {
  /* reads graph from stdin and builds the adjacency lists */
  int u, v;
  while (scanf("%d %d", &u, &v) == 2) addEdge(G, u, v);   
} 

//:::::::::::::::::::::::: queue functions ::::::::::::::::::::::::://

bool isEmpty(queue *Q) {
  /* is true if the queue is empty */
  return Q->front == Q->back;
}

queue *newQueue(int n) {
  /* creates a queue with n elements */
  queue *Q = safeCalloc(1, sizeof(queue));
  Q->array = safeCalloc(n, sizeof(node*));
  Q->size = n;
  return Q;
}

void freeQueue(queue *Q) {
  /* frees all memory allocated for the queue */
  free(Q->array);
  free(Q);
}

void doubleQueueSize(queue *Q) {
  /* doubles the size of the queue */
  Q->array = safeRealloc(Q->array, 2 * Q->size * sizeof(node*));
  for (int i = 0; i < Q->back; ++i)
    Q->array[i + Q->size] = Q->array[i];
  Q->back += Q->size;
  Q->size *= 2;
}

void enqueue (queue *Q, node *n) {
  /* adds n to the back of the queue */
  Q->array[Q->back] = n; 
  Q->back = (Q->back + 1) % Q->size;
  if (Q->back == Q->front) doubleQueueSize(Q);
}

node *dequeue (queue *Q) {
  /* removes and returns the first element of the queue */
  if (isEmpty(Q)) {
    printf("Error: dequeue() called on empty queue.\n");
    exit(EXIT_FAILURE);
  }
  node *n = Q->array[Q->front];
  Q->front = (Q->front + 1) % Q->size;
  return n;
}

//::::::::::::::::::::::::: Hopcroft-Karp :::::::::::::::::::::::::://

queue *initQueue(graph *G) {
  /* initializes the queue Q with all unmatched nodes in L */
  queue *Q = newQueue(G->nL + 1);
  for (int i = 0; i < G->nL; i++){
    node *u = G->nodes[i];
    if (u->matched == G->nil) {
      u->dist = 0;
      enqueue(Q, u);
    } else u->dist = INF;   // u is already part of the matching
  }
  G->nil->dist = INF; 
  return Q;
}

bool bfs(graph *G) {
  /* finds shortest alternating paths from L to R
     using BFS while updating the distance labels (levels) */
  queue *Q = initQueue(G);
  while (!isEmpty(Q)) {
    node *u = dequeue(Q);
    if (u == G->nil) break; // reached the dummy node in R; level graph is complete
    for (int i = 0; i < u->nAdj; i++) {
      node *v = u->adj[i];
      if (v->matched->dist == INF) { // if edge v->u is in matching
        v->matched->dist = u->dist + 1;
        enqueue(Q, v->matched);
      }
    }
  }
  freeQueue(Q);
  return G->nil->dist != INF;        // true if we found an augmenting path
}

bool dfs(graph *G, node *u) {
  /* finds an augmenting path from u to a node in R
     using DFS while updating the matching if one is found */
  if (u != G->nil) {
    for (int i = 0; i < u->nAdj; i++) {
      node *v = u->adj[i];
      if (v->matched->dist == u->dist + 1) 
        if (dfs(G, v->matched)) {
          v->matched = u;
          u->matched = v;
          return true;
        }
    }
    u->dist = INF;
    return false;
  }
  return true;
}

void hopcroftKarp(graph *G) {
  /* finds the maximum cardinality matching in G */
  while (bfs(G)){
    for (int i = 0; i < G->nL; i++) {
      node *u = G->nodes[i];
      if (u->matched == G->nil && dfs(G, u)) 
        G->nMatched++;
    }
  }
}

void printMatching(graph *G) {
  /* prints the matching */
  printf("Maximum matching has cardinality %d.\n", G->nMatched);
  if (!G->nMatched) return;
  printf("Possible matching:\n");
  for (int i = 0; i < G->nL; i++) {
    node *u = G->nodes[i];
    if (u->matched != G->nil) 
      printf("%d → %d\n", u->id, u->matched->id);
  }
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int l, r;                    // cardinality of left and right sets
  scanf("%d %d", &l, &r);      // read cardinalities from stdin

  graph *G = newGraph(l, r);   // create an unweighted graph 
  buildGraph(G);               // by reading edges from stdin 

  hopcroftKarp(G);             // find the maximum cardinality matching

  printMatching(G);             

  freeGraph(G);                
  return 0;
}
