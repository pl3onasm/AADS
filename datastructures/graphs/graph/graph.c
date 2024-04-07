/* 
  Generic graph implementation using a hash table
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "graph.h"
#include "../../../lib/clib/clib.h"
#include <ctype.h>

#define MAX_VERTEX_LABEL 50

//=================================================================
// FNV-1a hash function
// (http://www.isthe.com/chongo/tech/comp/fnv/index.html)
static uint64_t hash(void *key, uint64_t seed) {
  char *str = ((vertex *)key)->label;
  char ch;
  // FNV offset basis and magic seed
  uint64_t hash = 14695981039346656037ULL + seed;
  while ((ch = *str++)) {
    hash ^= ch;
    hash *= 1099511628211ULL;  // FNV prime
  }
  return hash;
}

//=================================================================
// Case sensitive comparison between keys
int cmpKey(void const *key1, void const *key2) {
  vertex *v1 = (vertex *)key1;
  vertex *v2 = (vertex *)key2;
  return strcmp(v1->label, v2->label);
}

//=================================================================
// Case sensitive comparison between values
int cmpVal(void const *val1, void const *val2) {
  edge *e1 = (edge *)val1;
  edge *e2 = (edge *)val2;
  return strcmp(e1->to->label, e2->to->label);
}

//=================================================================
// Show function for the hash table
void showStr(void *key) {
  vertex *v = (vertex *)key;
  printf("%s", v->label);
}

//=================================================================
// Creates a new graph node
vertex *newVertex(char *label) {
  vertex *node = safeCalloc(1, sizeof(vertex));
  node->label = safeCalloc(strlen(label) + 1, sizeof(char));
  strcpy(node->label, label);
  return node;
}

//=================================================================
// Deallocates a vertex
void freeVertex(void *v) {
  if (! v) return;
  vertex *vert = v;
  free(vert->label);
  free(vert);
}

//=================================================================
// Creates a new edge, from -> to
// An edge is the value of a key (vertex) in the hash table
edge *newEdge(vertex *to, double weight, weightType w) {
  edge *e = safeCalloc(1, sizeof(edge));
  e->to = to;
  e->weight = weight;
  e->wType = w;
  return e;
}

//=================================================================
// Show value function for the hash table
// In this case, the value is an edge
void showEdge(void *val) {
  edge *e = val;   
  if (!e) 
    return;
  if (e->wType == UNWEIGHTED)
    printf("%s",  e->to->label);
  else
    printf("%s(%.3g)", e->to->label, e->weight);
}

//=================================================================
// Shows a vertex and its adjacency list
void showVertex(graph *G, vertex *v) {
  if (! G || ! v) 
    return;
  htShowEntry(G->V, v);
}

//=================================================================
// Shows a vertex and its adjacency list by label
void showVertexL(graph *G, char *label) {
  G->v->label = label;
  htShowEntry(G->V, htGetKey(G->V, G->v));
}

//=================================================================
// Comparison function between vertices for qsort
static int cmpVertex(void const *v1, void const *v2) {
  return strcmp((*(vertex **)v1)->label, (*(vertex **)v2)->label);
}

//=================================================================
// Sorts the vertices in the graph by label
// Returns a sorted array of pointers to the vertices
vertex **sortGraph(graph *G) {
  if (! G) 
    return NULL;
  
  size_t n = nVertices(G);
  vertex **vertices = safeCalloc(n, sizeof(vertex *));
  size_t i = 0;

  for (vertex *v = firstV(G); v; v = nextV(G))
    vertices[i++] = v;

  qsort(vertices, n, sizeof(vertex *), cmpVertex);
  return vertices;
}

//=================================================================
// Shows the graph
void showGraph(graph *G) {
  printf("\n--------------------\n"
          " %s\n"
          " %s\n"
          " %s\n"
          " Vertices: %zu\n"
          " Edges: %zu\n"
          "--------------------\n", 
          G->V->label, 
          G->type == DIRECTED ? "Directed" : "Undirected",
          G->weight == UNWEIGHTED ? "Unweighted" : "Weighted",
          nVertices(G), 
          G->nEdges);

  // sort the vertices
  vertex **vertices = sortGraph(G);

  // show the vertices and their adjacency lists
  for (size_t i = 0; i < nVertices(G); i++) {
    printf("  ");
    showVertex(G, vertices[i]);
  }

  printf("--------------------\n\n");
  free(vertices);
}

//=================================================================
// Creates a new graph
graph *newGraph (size_t capacity, weightType weight) {

  graph *G = safeCalloc(1, sizeof(graph));
  G->u = safeCalloc(1, sizeof(vertex));
  G->v = safeCalloc(1, sizeof(vertex));
  G->e = safeCalloc(1, sizeof(edge));
  G->V = htNew(hash, cmpKey, cmpVal, capacity);
  // set show functions
  htSetShow(G->V, showStr, showEdge);
  // set ownership functions
  htOwnKeys(G->V, freeVertex);
  htOwnVals(G->V, free);
  G->V->label = "Graph";
  G->type = DIRECTED;
  G->weight = weight;
  return G;
}

//=================================================================
// Deallocates the graph
void freeGraph(graph *G) {
  if (! G) 
    return;
  htFree(G->V);
  free(G->u);
  free(G->v);
  free(G->e);
  free(G);
}

//=================================================================
// Sets the graph type to undirected
void setUndirected(graph *G) {
  if (! G) 
    return;
  G->type = UNDIRECTED;
}

//=================================================================
// Gets a vertex given its label
vertex *getVertex(graph *G, char *label) {
  if (! G || ! label)
    return NULL;
    
    // we use a dummy vertex to get the key;
    // this is needed because the hash function 
    // operates on vertices and not on plain strings
  G->v->label = label;
  return htGetKey(G->V, G->v);
}

//=================================================================
// Adds a vertex to the graph
void addVertex(graph *G, char *label) {
  G->v->label = label;
  if (htHasKey(G->V, G->v))
    return;
  vertex *vertex = newVertex(label);
  htAddKey(G->V, vertex);
}

//=================================================================
// Adds a vertex to the graph by label and returns a pointer to it
vertex *addVertexR(graph *G, char *label) {
  G->v->label = label;
  vertex *v = htGetKey(G->V, G->v);
  if (v) 
    return v;
  vertex *vertex = newVertex(label);
  htAddKey(G->V, vertex);
  return vertex;
}
  
//=================================================================
// Adds a weighted edge to the graph
// This adds a value to the key in the hash table
void addEdgeW(graph *G, vertex *from, vertex *to, double weight) {
  if (! G || ! from || ! to)
    return;

    // first check if the edge is already in the graph
  G->e->to = to;
  if (htHasKeyVal(G->V, from, G->e)) 
    return;

  edge *e = newEdge(to, weight, G->weight);
  htAddKeyVal(G->V, from, e);
  to->inDegree++;

  if (G->type == UNDIRECTED) {
    e = newEdge(from, weight, G->weight);
    htAddKeyVal(G->V, to, e);
    from->inDegree++;
  }
  G->nEdges++;
}

//=================================================================
// Gets an edge given the source and destination vertices
edge *getEdge(graph *G, vertex *from, vertex *to) {
  if (! G || ! from || ! to) 
    return NULL;
  G->e->to = to;
  return htGetVal(G->V, from, G->e);
}

//=================================================================
// Gets an edge given the source and destination vertices by label
edge *getEdgeL(graph *G, char *from, char *to) {
  if (! G || ! from || ! to) 
    return NULL;
  G->u->label = from;
  G->v->label = to;
  return getEdge(G, htGetKey(G->V, G->u), htGetKey(G->V, G->v));
}

//=================================================================
// Adds a weighted edge to the graph by label
void addEdgeWL(graph *G, char *from, char *to, double weight) {
  if (! G || ! from || ! to) 
    return;
  G->u->label = from;
  G->v->label = to;
  addEdgeW(G, htGetKey(G->V, G->u), htGetKey(G->V, G->v), weight);
}

//=================================================================
// Adds an unweighted edge to the graph
void addEdge(graph *G, vertex *from, vertex *to) {
  if (!G || ! from || ! to) 
    return;
  addEdgeW(G, from, to, 1);
}

//=================================================================
// Adds an unweighted edge to the graph by label
void addEdgeL(graph *G, char *from, char *to) {
  if (!G || ! from || ! to) 
    return;
  G->u->label = from;
  G->v->label = to;
  addEdge(G, htGetKey(G->V, G->u), htGetKey(G->V, G->v));
}

//=================================================================
// Returns true if the vertex exists in the graph
bool hasVertex(graph *G, char *label) {
  if (! G || ! label) 
    return false;
  G->v->label = label;
  return htHasKey(G->V, G->v);
}

//=================================================================
// Sets the pointer to the adjacency list of a vertex
// If the vertex has no adj list or is not in the graph,
// the pointer is set to NULL
// Returns true if the source vertex is in the graph
bool hasAdjList(graph *G, vertex *v, dll **adjList) {
  if (! G || ! v) 
    return false;
  return htHasKeyVals(G->V, v, adjList);
}

//=================================================================
// Returns the adjacency list of a vertex
dll *getNeighbors(graph *G, vertex *v) {
  if (! G || ! v) 
    return NULL;
  return htGetVals(G->V, v);
}

//=================================================================
// Returns the adjacency list of a vertex by label
dll *getNeighborsL(graph *G, char *label) {
  if (! G || ! label) 
    return NULL;
  G->v->label = label;
  return getNeighbors(G, htGetKey(G->V, G->v));
}

//=================================================================
// Returns true if the edge exists in the graph
bool hasEdge(graph *G, vertex *from, vertex *to) {
  if (! G || ! from || ! to) 
    return false;
  G->e->to = to;
  return htHasKeyVal(G->V, from, G->e);
}

//=================================================================
// Returns true if the edge is in the graph given the node labels
bool hasEdgeL(graph *G, char *from, char *to) {
  if (! G || ! from || ! to) 
    return false;
  G->u->label = from;
  G->v->label = to;
  return hasEdge(G, htGetKey(G->V, G->u), htGetKey(G->V, G->v));
}

//=================================================================
// Deletes an edge from the graph
// If the edge is not in the graph, nothing happens
void delEdge(graph *G, vertex *from, vertex *to) {
  if (! G || ! from || ! to) 
    return;
  
  G->e->to = to;
  if (htDelVal(G->V, from, G->e)) {
    G->nEdges--;
    to->inDegree--;
  }

  if (G->type == UNDIRECTED) {
    G->e->to = from;
    if (htDelVal(G->V, to, G->e)) 
      from->inDegree--;
  }
}

//=================================================================
// Deletes an edge from the graph by label
void delEdgeL(graph *G, char *from, char *to) {
  if (! G || ! from || ! to) 
    return;
  G->u->label = from;
  G->v->label = to;
  delEdge(G, htGetKey(G->V, G->u), htGetKey(G->V, G->v));
  if (G->type == UNDIRECTED) 
    delEdge(G, htGetKey(G->V, G->v), htGetKey(G->V, G->u));
}

//=================================================================
// Gets the 'first' edge in the graph and sets the iterator
// to the next edge; updates the vertex pointer to the source vertex
edge *firstE(graph *G, vertex **v) {
  if (! G)
    return NULL;
  G->z = firstV(G); 
  if (G->z && outDegree(G, G->z) == 0) 
    while ((G->z = nextV(G)) && outDegree(G, G->z) == 0)
  if (! G->z) 
    return NULL;
  G->adjList = getNeighbors(G, G->z);
  *v = G->z;
  return dllFirst(G->adjList);
}

//=================================================================
// Gets the next edge in the graph and sets the iterator to the
// next edge; updates the vertex pointer to the source vertex
// If there are no more edges, it returns NULL
edge *nextE(graph *G, vertex **v) {
  if (! G)
    return NULL;
  edge *e = dllNext(G->adjList);
  if (! e) {
    while ((G->z = nextV(G)) && outDegree(G, G->z) == 0);
    if (G->z) {
      G->adjList = getNeighbors(G, G->z);
      e = dllFirst(G->adjList);
    } 
  }
  *v = G->z;
  return e;
}

//=================================================================
// Case insensitive comparison for strings; 
// used for reading the graph
static int cmpStrCI(void const *str1, void const *str2) {
  char *s1 = (char *)str1;
  char *s2 = (char *)str2;
  while (*s1 && *s2) {
    int diff = tolower(*s1) - tolower(*s2);
    if (diff)
      return diff;
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

//=================================================================
// Adds source and destination vertices if they do not exist
// and adds an edge between them with the given weight
// If the graph is undirected, the edge is added in both directions
void addVandEW(graph *G, char *from, char *to, 
                     double weight) {
  vertex *u = addVertexR(G, from);
  vertex *v = addVertexR(G, to);
  addEdgeW(G, u, v, weight);
  if (G->type == UNDIRECTED) 
    addEdgeW(G, v, u, weight);
}

//=================================================================
// Same as addVandEW, but for an unweighted edge
void addVandE(graph *G, char *from, char *to) {
  addVandEW(G, from, to, 1);
}

//=================================================================
// Reads a graph from stdin
void readGraph(graph *G) {
  char from[MAX_VERTEX_LABEL], to[MAX_VERTEX_LABEL];
  double weight = 1;

    // check if the graph is set to undirected
  if (scanf ("%s", from) == 1 && 
     (cmpStrCI(from, "undirected") == 0)) {
    setUndirected(G);
  } else {
    // add the first edge
    if (G->weight == UNWEIGHTED)
      assert(scanf("%s", to) == 1);
    else 
      assert(scanf("%s %lf", to, &weight) == 2);
    addVandEW(G, from, to, weight);
  } 
  
    // read the rest of the graph
  if (G->weight == UNWEIGHTED) {
    while (scanf("%s %s", from, to) == 2) 
      addVandEW(G, from, to, 1);
  } else {
    while (scanf("%s %s %lf", from, to, &weight) == 3) 
      addVandEW(G, from, to, weight);
  }
}

//=================================================================
// Returns the 'first' vertex in the graph and sets the iterator
// to the next vertex
// NULL if the graph is empty
vertex *firstV(graph *G) {
  if (! G) return NULL;
  htEntry *entry = htFirst(G->V);
  return entry ? (vertex *)entry->key : NULL;
}

//=================================================================
// Returns the next vertex in the graph
// NULL if we have reached the end
vertex *nextV(graph *G) {
  if (! G) return NULL;
  htEntry *entry = htNext(G->V);
  return entry ? (vertex *)entry->key : NULL;
}

//=================================================================
// Returns a copy of the graph
graph *copyGraph(graph *G) {
  if (! G) return NULL;
  graph *copy = newGraph(nVertices(G), G->weight);
  copy->type = G->type;
  vertex *from; 

  for (edge *e = firstE(G, &from); e; e = nextE(G, &from)) 
    addVandEW(copy, from->label, e->to->label, e->weight);
  return copy;
}

//=================================================================
// Returns the transposed graph of G
// This is a copy of G but with the edges reversed
graph *transposeGraph(graph *G) {
  if (! G) return NULL;
  graph *T = newGraph(nVertices(G), G->weight);
  T->type = G->type;
  vertex *from;

  for (edge *e = firstE(G, &from); e; e = nextE(G, &from)) 
    addVandEW(T, e->to->label, from->label, e->weight);
  return T;
}

//=================================================================
#undef MAX_VERTEX_LABEL