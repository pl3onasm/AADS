/* file: mcbm-1.c  
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Maximum Cardinality Bipartite Matching (MCBM) 
                using Dinic's algorithm for maximum flow
   time complexity: O(E√V)
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define INF INT_MAX

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct node node; // forward declaration of node

typedef enum {            // definition for boolean type
  false = 0,
  true = 1
} bool;

typedef struct edge {
  node *from, *to;        // pointers to the endnodes of the edge (u->v)
  short cap;              // capacity of the edge (0 or 1)
  short flow;             // flow on the edge 
  bool reverse;           // true if the edge is a reverse edge
  struct edge *rev;       // pointer to edge in the reverse direction
} edge;

struct node {
  int id;                 // id of the node
  edge **adj;             // adj list is an array of pointers to edges
  int adjCap;             // capacity of the adjacency list
  int nAdj;               // number of adjacent nodes
  int level;              // level of the node in the level graph
  int adjIdx;             // current adj list index in the DFS traversals
};

typedef struct graph {
  int nNodes, nEdges;     // number of nodes and edges in the graph
  node **nodes;           // array of pointers to nodes
  edge **edges;           // array of pointers to edges
  int edgeCap;            // capacity of the edge array
  int maxCard;            // maximum cardinality (flow) in the graph
} graph;

typedef struct queue {
  int front, back, size;   // front and back of the queue, and its size
  node **array;            // array of elements in the queue
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
  n->level = -1;
  return n;
}

graph *newGraph(int left, int right) {
  /* creates a graph with n vertices */
  int n = left + right + 2;
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = n;
  G->nodes = safeCalloc(n, sizeof(node*));
  for (int i = 0; i < n; i++)
    G->nodes[i] = newNode(i);
  return G;
}

void loopReset(graph *G) {
  /* resets all level fields and current adjacency list indices */
  for (int i = 0; i < G->nNodes; i++){
    G->nodes[i]->level = -1;
    G->nodes[i]->adjIdx = 0;
  }
}

void freeGraph(graph *G) {
  /* frees all memory allocated for the graph */
  for (int i = 0; i < G->nNodes; i++){
    free(G->nodes[i]->adj);
    free(G->nodes[i]);
  }
  free(G->nodes);
  for (int i = 0; i < G->nEdges; i++)
    free(G->edges[i]);
  free(G->edges);
  free(G);
}

edge *addEdge(graph *G, int uId, int vId, int cap, bool reverse) {
  /* adds an edge from u to v with capacity cap */
  edge *e = safeCalloc(1, sizeof(edge));
  node *u = G->nodes[uId];
  e->to = G->nodes[vId];
  e->from = u;
  e->cap = cap;
  e->reverse = reverse;
  // check if we need to resize the edge array
  if (G->edgeCap == G->nEdges) {
    G->edgeCap += 10;
    G->edges = safeRealloc(G->edges, G->edgeCap * sizeof(edge*));
  }
  // check if we need to resize the adjacency list
  if (u->adjCap == u->nAdj) {
    u->adjCap += 10; 
    u->adj = safeRealloc(u->adj, u->adjCap * sizeof(edge*));
  }
  u->adj[u->nAdj++] = e;       // add the edge to the adj list                 
  G->edges[G->nEdges++] = e;   // add the original edge to G
  return e;
}

void buildGraph(graph *G, int left, int right) {
  /* reads directed graph from stdin and builds the adjacency lists */
  int u, v; edge *e, *r;
  int n = G->nNodes;
  while (scanf("%d %d", &u, &v) == 2) {
    e = addEdge(G, u, v, 1, false);   // add original edge
    r = addEdge(G, v, u, 0, true);    // add reverse edge
    // add pointers to the reverse edges
    e->rev = r; r->rev = e;
  }
  // add edges from super source to left nodes
  for (int i = 0; i < left; i++) {
    e = addEdge(G, n-2, i, 1, false);
    r = addEdge(G, i, n-2, 0, true);
    e->rev = r; r->rev = e;
  }
  // add edges from right nodes to super sink
  for (int i = left; i < left+right; i++) {
    e = addEdge(G, i, n-1, 1, false);
    r = addEdge(G, n-1, i, 0, true);
    e->rev = r; r->rev = e;
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

//::::::::::::::::::::::::::::: Dinic :::::::::::::::::::::::::::::://

bool bfs(graph *G, node *s, node *t) {
  /* builds a BFS tree/level graph from s and returns if t is reachable */
  queue *q = newQueue(G->nNodes); 
  enqueue(q, s);                        // enqueue source node
  s->level = 0;                         // set source level to 0
  while (!isEmpty(q)) {
    node *n = dequeue(q);
   
    // check each edge from n
    for (int i = 0; i < n->nAdj; i++) {
      edge *e = n->adj[i];
      if (e->cap - e->flow > 0 && e->to->level == -1) {
        e->to->level = n->level + 1;    // set level of child node
        enqueue(q, e->to);                
      }
    }
  }
  freeQueue(q);
  return t->level != -1;                // return if t is reachable
}

bool dfs(graph *G, node *s, node *t, int flow) {
  /* finds an augmenting path from s to t and returns if one still exists */
  if (s->id == t->id) return flow;      // reached sink
  // check each edge from current s node and prune those that don't lead to t
  // so that we don't have to check them again in the next DFS call
  for (int i = s->adjIdx; i < s->nAdj; i++) {
    s->adjIdx = i;                      // update current adj list index
    edge *e = s->adj[i];
    if (e->cap - e->flow > 0 && e->to->level == s->level + 1) 
      if (dfs(G, e->to, t, MIN(flow, e->cap - e->flow))){
        e->flow += 1;                   // adjust flow on orignal/reverse edges 
        e->rev->flow -= 1;              // i.e. include/exclude edges in/from M
        return true;
      } 
  }
  return false;
}

void dinic(graph *G) {
  /* finds the maximum flow/cardinality matching in G */
  node *s = G->nodes[G->nNodes-2];      // super source
  node *t = G->nodes[G->nNodes-1];      // super sink
  while (bfs(G, s, t)) {
    while(dfs(G, s, t, INF)) 
      G->maxCard++;                     // increase cardinality
    loopReset(G);  // reset all levels and current adjList indices
  }
}

void printFlow(graph *G) {
  /* prints the flow on each edge of the graph G */
  printf("Maximum matching has cardinality %d.\n", G->maxCard);
  if (!G->maxCard) return;
  printf("Possible matching:\n");
  for (int i = 0; i < G->nEdges; i++) {
    edge *e = G->edges[i];
    if (e->from->id != G->nNodes-2 && e->to->id != G->nNodes-1
        && e->flow && !e->reverse) 
      printf("%d → %d\n", e->from->id, e->to->id);
  }
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int l, r;                    // cardinality of left and right sets
  scanf("%d %d", &l, &r);

  graph *G = newGraph(l, r);   // build an unweighted graph with
  buildGraph(G, l, r);         // super source and sink at the end

  dinic(G);                    // find the maximum flow / matching
  printFlow(G);                // print the maximum matching

  freeGraph(G);                // free memory
  return 0;
}
