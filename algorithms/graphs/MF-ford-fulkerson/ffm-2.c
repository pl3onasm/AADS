/* file: ffm-2.c  
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: implements Ford-Fulkerson with DFS and a
     heuristic called capacity scaling to find the maximum flow
   time complexity: O(E²log(maxCap))
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define INF DBL_MAX

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct node node; // forward declaration of node

typedef enum {            // definition for boolean type
  false = 0,
  true = 1
} bool;

typedef struct edge {
  int from, to;           // ids of the endpoints of the edge (u->v)
  double cap;             // capacity of the edge
  double flow;            // flow on the edge
  bool reverse;           // true if the edge is exclusive to Gf
  struct edge *rev;       // pointer to edge in the reverse direction
} edge;

struct node {
  int id;                 // id of the node
  edge **adj;             // adj list is an array of pointers to edges
  int adjCap;             // capacity of the adjacency list
  int nAdj;               // number of adjacent nodes
  bool visited;           // true if the node has been visited in the DFS
};

typedef struct graph {
  int nNodes, nEdges;     // number of nodes and edges in the graph
  node **nodes;           // array of pointers to nodes
  edge **edges;           // array of pointers to edges
  int edgeCap;            // capacity of the edge array
  double maxCap;          // maximum capacity among all edges
  double maxFlow;         // maximum flow in the graph
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

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a node with given id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
  n->visited = false;
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
  e->from = uId;
  e->to = vId;
  e->cap = cap;
  e->reverse = reverse;        // true if the edge is exclusive to Gf
  // check if we need to resize the edge array
  if (G->edgeCap == G->nEdges) {
    G->edgeCap += 10;
    G->edges = safeRealloc(G->edges, G->edgeCap * sizeof(edge*));
  }
  // check if we need to resize the adjacency list
  node *u = G->nodes[uId];
  if (u->adjCap == u->nAdj) {
    u->adjCap += 10;
    u->adj = safeRealloc(u->adj, u->adjCap * sizeof(int));
  }
  u->adj[u->nAdj++] = e;       // add the edge to the adjacency list                
  G->edges[G->nEdges++] = e;   // add the original edge to G
  return e;
}

void buildGraph(graph *G) {
  /* reads directed graph from stdin and builds the adjacency lists */
  int u, v; double cap; edge *e, *r;
  while (scanf("%d %d %lf", &u, &v, &cap) == 3) {
    G->maxCap = MAX(G->maxCap, cap);   // update max capacity seen so far
    e = addEdge(G, u, v, cap, false);  // add original edge
    r = addEdge(G, v, u, 0, true);     // add reverse edge
    // add pointers to the reverse edges
    e->rev = r; r->rev = e;
  }
}

//:::::::::::::::::: DFS with capacity scaling ::::::::::::::::::::://

int pow2(int n) {
  /* returns the largest power of 2 ≤ the max capacity */
  int p = 1;
  while (p < n) p <<= 1;
  return p >> 1;
}

double dfs(graph *G, int s, int t, double flow, int delta) {
  /* tries to find an augmenting path from s to t using DFS */
  if (s == t) return flow;          // reached the sink
  node *u = G->nodes[s];
  if (u->visited) return 0;         // already visited
  double bneck;
  for (int i = 0; i < u->nAdj; ++i) {     
    edge *e = u->adj[i];  
    if (e->cap - e->flow >= delta) {             
      u->visited = true;            // mark as visited
      if ((bneck = dfs(G, e->to, t, 
           MIN(flow, e->cap - e->flow), delta))) {
        // path flow is determined by the minimum edge capacity
        e->flow += bneck;           // update flow on edges  
        e->rev->flow -= bneck;            
        u->visited = false;         // mark as unvisited
        return bneck;
      }               
    }
  }
  u->visited = false; 
  return 0;
}

void maxFlow(graph *G, int s, int t) {
  /* finds the maximum flow from s to t using capacity scaling */
  double flow;                      // flow on augmenting path
  int delta = pow2(G->maxCap);      // initial threshold
  for (; delta > 0; delta >>= 1) {     
    while ((flow = dfs(G, s, t, INF, delta)))   
      G->maxFlow += flow;           // augment the flow in G by path flow
  }
}

void printFlow(graph *G, int s, int t) {
  /* prints the flow on each edge of the graph G */
  printf("The maximum flow from node %d to node %d"
         " is %.2lf\nFlow graph:\n\n  from     to%13s\n\n",
          s, t, G->maxFlow, "flow");
  for (int i = 0; i < G->nEdges; ++i) {
    edge *e = G->edges[i];
    if (!e->reverse){   // only edges in G, not Gf
      printf("%6d %6d", e->from, e->to);
      if (e->flow > 0) printf("%13.2lf\n", e->flow);
      else printf("%13c\n", '-');
    }
  }
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n, s, t;                    // number of nodes, source, sink
  scanf("%d %d %d", &n, &s, &t);

  graph *G = newGraph(n);         // read edges from stdin
  buildGraph(G);                  // and get max capacity
                                  
  maxFlow(G, s, t);               // find maximum flow
  printFlow(G, s, t);             // print flow values

  freeGraph(G);                   // free memory
  return 0;
}
