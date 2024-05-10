/* file: mcmf.c  
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: weighted MCBM using min/max-cost max-flow
   time complexity: O(V²E²)
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

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
  double cost;            // cost of the edge when flow is 1
  bool reverse;           // true if the edge is a reverse edge
  struct edge *rev;       // pointer to edge in the reverse direction
} edge;

struct node {
  int id;                 // id of the node
  edge **adj;             // adj list is an array of pointers to edges
  int adjCap;             // capacity of the adjacency list
  int nAdj;               // number of adjacent nodes
  int dist;               // distance from the source 
  int adjIdx;             // current adj list index in the DFS traversals
  bool visited;           // true if the node has been visited in DFS
};

typedef struct graph {
  int nNodes, nEdges;     // number of nodes and edges in the graph
  node **nodes;           // array of pointers to nodes
  edge **edges;           // array of pointers to edges
  int edgeCap;            // capacity of the edge array
  int totalCost;          // total cost of the matching
  int maxCard;            // maximum cardinality (flow) in the graph
  bool costType;          // true if min-cost, false if max-cost 
} graph;

//:::::::::::::::::::::::: graph functions ::::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a node with given id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
  n->dist = INF;
  n->visited = false;
  return n;
}

graph *newGraph(int left, int right, char *type) {
  /* creates a graph with n vertices */
  int n = left + right + 2;
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = n;
  G->nodes = safeCalloc(n, sizeof(node*));
  for (int i = 0; i < n; i++)
    G->nodes[i] = newNode(i);
  G->costType = strcmp (type, "max");
  return G;
}

void loopReset(graph *G) {
  /* resets the adjacency list indices and distances of all nodes */
  for (int i = 0; i < G->nNodes; i++){
    G->nodes[i]->dist = INF;
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

edge *addEdge(graph *G, int uId, int vId, double cost, 
  int cap, bool reverse) {
  /* adds an edge from u to v with given capacity and cost to the graph */
  edge *e = safeCalloc(1, sizeof(edge));
  node *u = G->nodes[uId];
  e->to = G->nodes[vId];
  e->from = u;
  e->cap = cap;
  e->reverse = reverse;
  e->cost = cost;
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
  G->edges[G->nEdges++] = e;   // add the edge to G
  return e;
}

void buildGraph(graph *G, int left, int right) {
  /* reads directed graph from stdin and builds the adjacency lists */
  int u, v; double c;  edge *e, *r;
  int n = G->nNodes;
  while (scanf("%d %d %lf", &u, &v, &c) == 3) {
    if (!G->costType) c = -c;           // negate cost if max-cost
    e = addEdge(G, u, v, c, 1, false);  // add original edge
    r = addEdge(G, v, u, -c, 0, true);  // add reverse edge
    // add pointers to the reverse edges
    e->rev = r; r->rev = e;
  }
  // add edges from super source to left nodes
  for (int i = 0; i < left; i++) {
    e = addEdge(G, n-2, i, 0, 1, false);
    r = addEdge(G, i, n-2, 0, 0, true);
    e->rev = r; r->rev = e;
  }
  // add edges from right nodes to super sink
  for (int i = left; i < left+right; i++) {
    e = addEdge(G, i, n-1, 0, 1, false);
    r = addEdge(G, n-1, i, 0, 0, true);
    e->rev = r; r->rev = e;
  } 
} 

//::::::::::::::::::::::::::::: Dinic :::::::::::::::::::::::::::::://

void relax(graph *G, edge *e) {
  /* relaxes the edge e */
  node *u = e->from, *v = e->to;
  if (e->cap - e->flow <= 0) return;    // edge is saturated
  if (u->dist == INF) return;           // u is unreachable
  if (v->dist > u->dist + e->cost)      // found a cheaper path
    v->dist = u->dist + e->cost;        // update estimate
}

bool bFord(graph *G, node *s, node *t) {
  /* runs Bellman-Ford to find cheapest paths from s to all nodes */
  s->dist = 0;
  for (int i = 0; i < G->nNodes - 1; i++) 
    for (int j = 0; j < G->nEdges; j++) 
      relax(G, G->edges[j]);
  return t->dist != INF;                // return if t is reachable
}

bool dfs(graph *G, node *s, node *t, int flow) {
  /* finds an augmenting path from s to t, if any */
  if (s->id == t->id || flow == 0) return true;
  s->visited = true;                    // mark node as visited
  // check each edge from current s node and prune those that don't lead to t
  // so that we don't have to check them again in the next DFS call
  for (int i = s->adjIdx; i < s->nAdj; i++) {
    s->adjIdx = i;                      // update current adj list index
    edge *e = s->adj[i];
    if (!e->to->visited && e->cap - e->flow > 0 &&
        e->to->dist == s->dist + e->cost) 
      if (dfs(G, e->to, t, MIN(flow, e->cap - e->flow))) { 
        G->totalCost += e->cost;        // update total cost
        e->flow += 1;                   // adjust flow on orignal/reverse edges 
        e->rev->flow -= 1;              // i.e. include/exclude edges in/from M
        s->visited = false;             // unmark node
        return true;                    // found a path
      } 
  }
  s->visited = false;                   // unmark node
  return false;
}

void dinic(graph *G) {
  /* finds the maximum flow from s to t using Dinic's algorithm */
  node *s = G->nodes[G->nNodes-2];      // super source
  node *t = G->nodes[G->nNodes-1];      // super sink
  while (bFord(G, s, t)) {              // while t is reachable
    while (dfs(G, s, t, INF))           // while an augmenting path exists
      G->maxCard++;                     // increment cardinality of M
    loopReset(G);  // reset distances and adj list indices for next bFord
  }
}

void printFlow(graph *G) {
  /* prints the flow on each edge of the graph G */
  printf("Maximum matching has cardinality %d", G->maxCard);
  if (!G->maxCard) return;
  if (G->costType) printf(" and min cost %d\n", G->totalCost);
  else printf(" and max cost %d\n", -G->totalCost);
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
  int l, r;                     // cardinality of left and right sets
  char s[4];                    // s = "max" or "min" for max/min cost
  scanf("%d %d %s", &l, &r, s); 

  graph *G = newGraph(l, r, s); // build an unweighted graph with
  buildGraph(G, l, r);          // l left nodes, r right nodes 
                               
  dinic(G);                     // find the maximum flow / matching
  printFlow(G);                 // print the maximum matching

  freeGraph(G);                 // free memory
  return 0;
}
