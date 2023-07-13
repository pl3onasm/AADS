/* file: mcbm-3.c  
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: preferred Maximum Cardinality Bipartite Matching (MCBM) 
                using Gale-Shapley's algorithm
   time complexity: O(EV)
*/

#include <stdio.h>
#include <stdlib.h>

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef enum {            // definition for boolean type
  false = 0,
  true = 1
} bool;

typedef struct node {
  int id;                 // id of the node
  int *adj;               // adj list is an int array
  int nAdj;               // number of nodes in the adjacency list
  int adjIndex;           // index of the next adj candidate to be considered
  struct node* matched;   // pointer to the matched node
} node;

typedef struct graph {
  node **nodes;           // array of pointers to nodes
  int nL, nR;             // number of nodes in the left and right partition
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
  return n;
}

graph *newGraph(int left, int right) {
  /* creates a graph with n vertices */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nL = left;
  G->nR = right;
  G->nodes = safeCalloc(left + right, sizeof(node*));
  for (int i = 0; i < left + right; i++)
    G->nodes[i] = newNode(i);
  for (int i = 0; i < left; i++) 
    G->nodes[i]->adj = safeCalloc(right, sizeof(int));
  for (int i = left; i < left + right; i++) 
    G->nodes[i]->adj = safeCalloc(left, sizeof(int));
  return G;
}

void freeGraph(graph *G) {
  /* frees all memory allocated for the graph */
  for (int i = 0; i < G->nL + G->nR; i++) {
    free(G->nodes[i]->adj);
    free(G->nodes[i]);
  }
  free(G->nodes);
  free(G);
}

void buildGraph(graph *G) {
  /* reads graph from stdin */
  int v;
  // read left partition: we create adj lists 
  // for L where the index reflects the preference
  // and the value reflects the node id in R
  for (int i = 0; i < G->nL; i++){
    node *u = G->nodes[i];
    for (int j = 0; j < G->nR; j++) 
      if (scanf("%d", &v) == 1)
        u->adj[j] = v;	
    u->nAdj = G->nR;
  }
    
  // read right partition: we create adj lists
  // for R where the index reflects the node id
  // in L and the value reflects the preference 
  for (int i = G->nL; i < G->nL + G->nR; i++){
    node *u = G->nodes[i];
    for (int j = 0; j < G->nL; j++) 
      if (scanf("%d", &v) == 1) 
        u->adj[v] = j;
    u->nAdj = G->nL;
  }
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

//:::::::::::::::::::::::::: Gale-Shapley :::::::::::::::::::::::::://

queue *initQueue(graph *G){
  /* initializes the queue Q with all nodes in L */
  queue *Q = newQueue(G->nL);
  for (int i = 0; i < G->nL; i++) 
    enqueue(Q, G->nodes[i]);
  return Q;
}

void galeShapley (graph *G) {
  queue *Q = initQueue(G);
  while (!isEmpty(Q)) {
    node *u = dequeue(Q);
    if (u->adjIndex == u->nAdj) continue;  // u has no more preferences
    node *v = G->nodes[u->adj[u->adjIndex++]];
    if (!v->matched) {
      // v is unmatched, so match u and v
      u->matched = v;
      v->matched = u;
    } else if (v->adj[v->matched->id] > v->adj[u->id]) {
      // v prefers u to its current match
      v->matched->matched = NULL; // unmatch v->matched
      enqueue(Q, v->matched);     // add v->matched back to the queue
      u->matched = v;             // match u and v
      v->matched = u;
    } else enqueue (Q, u);        // v rejects u, add u back to the queue
  }
  freeQueue(Q);
}

void printMatching(graph *G) {
  /* prints the matching to stdout */
  printf("The below matching is stable.\n"); 
  printf("\n  Left   Right\n"); 
  for (int i = 0; i < G->nL; i++) {
    node *u = G->nodes[i];
    if (u->matched) printf("%6d %7d\n", u->id, u->matched->id);
    else printf("%6d %7d\n", u->id, -1);
  }
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int l, r;                    // cardinality of left and right sets
  scanf("%d %d", &l, &r);      // read cardinalities from stdin

  graph *G = newGraph(l, r);   // create an unweighted graph by
  buildGraph(G);               // reading preference lists from stdin

  galeShapley(G);              // find a stable matching

  printMatching(G);             

  freeGraph(G);                
  return 0;
}
