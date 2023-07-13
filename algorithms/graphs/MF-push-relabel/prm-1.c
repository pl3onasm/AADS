/* file: prm-1.c  
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: implements the generic push-relabel 
                maximum flow algorithm
   time complexity: O(V²E)
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
  node *from, *to;        // pointers to the endpoints of the edge (u->v)
  double cap;             // capacity of the edge
  double flow;            // flow on the edge
  bool reverse;           // to separate Gf from G
  struct edge *rev;       // pointer to edge in the reverse direction
} edge;

struct node {
  int id;                 // id of the node
  edge **adj;             // adj list is an array of pointers to edges
  int adjCap;             // capacity of the adjacency list
  int nAdj;               // number of adjacent nodes
  int height;             // height of the node in Gf
  int adjIdx;             // current adjacency list index 
  double excess;          // excess flow at the node
};

typedef struct graph {
  int nNodes, nEdges;     // number of nodes and edges in the graph
  node **nodes;           // array of pointers to nodes
  edge **edges;           // array of pointers to edges
  int edgeCap;            // capacity of the edge array
  double maxFlow;         // maximum flow in the graph
} graph;

typedef struct queue {
  int front, back, size;   // front and back of the queue, and its size
  int *array;              // array of elements in the queue
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

//:::::::::::::::::::::::: queue functions ::::::::::::::::::::::::://

bool isEmpty(queue *Q) {
  /* is true if the queue is empty */
  return Q->front == Q->back;
}

queue *newQueue(int n) {
  /* creates a queue with n elements */
  queue *Q = safeCalloc(1, sizeof(queue));
  Q->array = safeCalloc(n, sizeof(int));
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
  Q->array = safeRealloc(Q->array, 2 * Q->size * sizeof(int));
  for (int i = 0; i < Q->back; ++i)
    Q->array[i + Q->size] = Q->array[i];
  Q->back += Q->size;
  Q->size *= 2;
}

void enqueue (queue *Q, int n) {
  /* adds n to the back of the queue */
  Q->array[Q->back] = n;
  Q->back = (Q->back + 1) % Q->size;
  if (Q->back == Q->front) doubleQueueSize(Q);
}

int dequeue (queue *Q) {
  /* removes and returns the first element of the queue */
  if (isEmpty(Q)) {
    printf("Error: dequeue() called on empty queue.\n");
    exit(EXIT_FAILURE);
  }
  int n = Q->array[Q->front];
  Q->front = (Q->front + 1) % Q->size;
  return n;
}

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a node with given id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
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

edge *addEdge(graph *G, int uId, int vId, double cap, bool reverse) {
  /* adds an edge from u to v with capacity cap */
  edge *e = safeCalloc(1, sizeof(edge));
  node *u = G->nodes[uId];
  e->to = G->nodes[vId];
  e->from = u;
  e->cap = cap;
  e->reverse = reverse;        // true if the edge is exclusive to Gf
  // check if we need to resize the edge array
  if (G->edgeCap == G->nEdges) {
    G->edgeCap += 10;
    G->edges = safeRealloc(G->edges, G->edgeCap * sizeof(edge*));
  }
  // check if we need to resize the adjacency list
  if (u->adjCap == u->nAdj) {
    u->adjCap += 10;
    u->adj = safeRealloc(u->adj, u->adjCap * sizeof(int));
  }
  u->adj[u->nAdj++] = e;       // add the edge to the adj list
  G->edges[G->nEdges++] = e;   // add the original edge to G
  return e;
}

void buildGraph(graph *G) {
  /* reads directed graph from stdin and builds the adj lists */
  int u, v; double cap; edge *e, *r;
  while (scanf("%d %d %lf", &u, &v, &cap) == 3) {
    e = addEdge(G, u, v, cap, false); // add original edge
    r = addEdge(G, v, u, 0, true);    // add reverse edge
    // add pointers to the reverse edges
    e->rev = r; r->rev = e;
  }
}

//::::::::::::::::::::::: push and relabel ::::::::::::::::::::::::://

void initPreflow(graph *G, int s, queue *Q) {
  /* initializes the preflow at the source s */
  node *u = G->nodes[s];
  u->height = G->nNodes;          // set height of source to n
  for (int i = 0; i < u->nAdj; i++) {
    // set full flow on all edges from s
    edge *e = u->adj[i];
    e->flow = e->cap;             // set flow on edges
    e->rev->flow = -e->cap;       
    e->to->excess += e->cap;      // update excess at v
    enqueue(Q, e->to->id);        // equeue all neighbors of s
  }
}

bool push(graph *G, node *u, queue *Q) {
  /* pushes flow from u to its neighbors */
  for (int i = u->adjIdx; i < u->nAdj; i++) {
    u->adjIdx = i;                // save the current adj index
    edge *e = u->adj[i];
    if (e->cap - e->flow > 0 && u->height == e->to->height + 1) {
      double delta = MIN(u->excess, e->cap - e->flow);
      e->flow += delta;           // update flow on edges
      e->rev->flow -= delta;      
      u->excess -= delta;         // update excess at u
      e->to->excess += delta;     // update excess at v
      if (e->to->excess == delta) 
        enqueue(Q, e->to->id);    // enqueue v if it was inactive
      return true;
    }
  }
  u->adjIdx = 0;                  // reset the adjacency index
  return false;
}

void relabel(graph *G, node *u, queue *Q) {
  /* relabels u to the minimum height of its 
     neighbors in Gf plus one */
  int min = INF;
  for (int i = 0; i < u->nAdj; i++) {
    edge *e = u->adj[i];
    if (e->cap - e->flow > 0) 
      min = MIN(min, e->to->height);
  }
  u->height = min + 1;
}

void maxFlow (graph *G, int s, int t) {
  /* computes the maximum flow from s to t */
  queue *Q = newQueue(G->nNodes);   
  initPreflow(G, s, Q);
  
  while (!isEmpty(Q)) {
    int uId = dequeue(Q);           // get the next active node
    if (uId == t || uId == s)       // skip the source and sink
      continue;
    
    node *u = G->nodes[uId];
                                    // we know u has excess flow so we 
    if (!push(G, u, Q))             // can either try to push from u
      relabel(G, u, Q);             // or relabel it if pushing fails
      
    if (u->excess > 0)              // enqueue u if it still has excess
      enqueue(Q, uId);
  }
  G->maxFlow = G->nodes[t]->excess; // max flow is the excess at the sink
  freeQueue(Q);
} 

void printFlow(graph *G, int s, int t) {
  /* prints the flow on each edge of the graph G */
  printf("The maximum flow from node %d to node %d"
         " is %.2lf\nFlow graph:\n\n  from     to%13s\n\n",
          s, t, G->maxFlow, "flow");
  for (int i = 0; i < G->nEdges; ++i) {
    edge *e = G->edges[i];
    if (!e->reverse){               // print only original edges
      printf("%6d %6d", e->from->id, e->to->id);
      if (e->flow > 0) printf("%13.2lf\n", e->flow);
      else printf("%13c\n", '-');
    }
  }
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n, s, t;                      // number of nodes, source, sink
  scanf("%d %d %d", &n, &s, &t);

  graph *G = newGraph(n); 
  buildGraph(G);                    // read edges from stdin

  maxFlow(G, s, t);                 // compute max flow
  printFlow(G, s, t);               // print flow values

  freeGraph(G);                     // free memory
  return 0;
}
