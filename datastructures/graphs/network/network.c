/* 
  Generic flow network implementation using a hash table
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "network.h"
#include "../../../lib/clib/clib.h"
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define MAX_LABEL 50

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
void showStr(void const *key) {
  vertex *v = (vertex *)key;
  printf("%s", v->label);
}

//=================================================================
// Creates a new network node
vertex *newVertex(char *label) {
  vertex *v = safeCalloc(1, sizeof(vertex));
  strcpy(v->label, label);
  return v;
}

//=================================================================
// Deallocates a vertex
void freeVertex(void *v) {
  if (! v) return;
  vertex *vert = v;
  free(vert);
}

//=================================================================
// Creates a new edge, from -> to
// An edge is the value of a key (vertex) in the hash table
edge *newEdge(vertex *from, vertex *to, size_t cap, 
              bool residual) {
  edge *e = safeCalloc(1, sizeof(edge));
  e->from = from;
  e->to = to;
  e->cap = cap;
  e->residual = residual;
  return e;
}

//=================================================================
// Show value function for the hash table
// In this case, the value is an edge
void showEdge(void const *val) {
  edge *e = (edge *)val;
  if (!e) 
    return;
  if (! e->residual)
    printf("%s(%d)", e->to->label, e->flow);
}

//=================================================================
// Shows a vertex and its adjacency list
void showVertex(network *N, vertex *v) {
  if (! N || ! v) 
    return;
  dll *edges = getNeighbors(N, v);
  printf("%s[%zu]", v->label, dllSize(edges));
  printf(dllSize(edges) ? ": " : "");
  size_t nItems = 0;
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
    if (! e->residual) {
      printf("%s(", e->to->label);
      if (e->cap == SIZE_MAX)
        printf("inf)");
      else
        printf("%zu)", e->cap);
      nItems++;
      if (nItems < dllSize(edges))
        printf(", ");
    }
  }
  printf("\n");
}

//=================================================================
// Shows a vertex and its adjacency list by label
void showVertexL(network *N, char *label) {
  strcpy(N->v->label, label);
  htShowEntry(N->V, htGetKey(N->V, N->v));
}

//=================================================================
// Comparison function between vertices for qsort
static int cmpVertex(void const *v1, void const *v2) {
  return strcmp((*(vertex **)v1)->label, (*(vertex **)v2)->label);
}

//=================================================================
// Sorts the vertices in the network by label
// Returns a sorted array of pointers to the vertices
vertex **sortNetwork(network *N) {
  if (! N) 
    return NULL;
  
  size_t n = nVertices(N);
  vertex **vertices = safeCalloc(n, sizeof(vertex *));
  size_t i = 0;

  for (vertex *v = firstV(N); v; v = nextV(N))
    vertices[i++] = v;

  qsort(vertices, n, sizeof(vertex *), cmpVertex);
  return vertices;
}

//=================================================================
// Shows the network
void showNetwork(network *N) {
  printf("\n--------------------\n"
          " %s\n"
          " Vertices: %zu\n"
          " Edges: %zu\n"
          "--------------------\n", 
          N->V->label, 
          nVertices(N), 
          N->nEdges);

  // sort the vertices
  vertex **vertices = sortNetwork(N);

  // show the vertices and their adjacency lists
  for (size_t i = 0; i < nVertices(N); i++) {
    printf("  ");
    showVertex(N, vertices[i]);
  }

  printf("--------------------\n\n");
  free(vertices);
}

//=================================================================
// Shows the flow for the outgoing edges of a vertex
void showVertexFlow(network *N, vertex *v) {
  if (! N || ! v) 
    return;
  dll *edges = getNeighbors(N, v);
  printf("%s[%zu]", v->label, dllSize(edges));
  printf(dllSize(edges) ? ": " : "");
  size_t nItems = 0;
  for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
    if (! e->residual) {
      printf("%s(%d/", e->to->label, e->flow);
      if (e->cap == SIZE_MAX)
        printf("inf)");
      else
        printf("%zu)", e->cap);
      nItems++;
      if (nItems < dllSize(edges))
        printf(", ");
    }
  }
  printf("\n");
} 

//=================================================================
// Shows the flow in the network
void showFlow(network *N, vertex *src, vertex *sink) {
  if (! N) 
    return;
  printf("\n--------------------\n"
          " %s flow\n"
          " Source: %s\n"
          " Sink: %s\n"
          " Max flow: %zu\n"
          "--------------------\n", 
          N->V->label, 
          src->label,
          sink->label,
          N->maxFlow);

  // sort the vertices
  vertex **vertices = sortNetwork(N);

  // show the vertices and their adjacency lists
  for (size_t i = 0; i < nVertices(N); i++) {
    printf("  ");
    showVertexFlow(N, vertices[i]);
  }

  printf("--------------------\n\n");
  free(vertices);
}

//=================================================================
// Creates a new network
network *newNetwork (size_t capacity, weightType wType) {

  network *N = safeCalloc(1, sizeof(network));
  N->weight = wType;
  N->u = safeCalloc(1, sizeof(vertex));
  N->v = safeCalloc(1, sizeof(vertex));
  N->e = safeCalloc(1, sizeof(edge));
  N->V = htNew(hash, cmpKey, cmpVal, capacity);
    // set show functions
  htSetShow(N->V, showStr, showEdge);
    // set ownership functions
  htOwnKeys(N->V, freeVertex);
  htOwnVals(N->V, free);
  N->V->label = "Network";
  return N;
}

//=================================================================
// Deallocates the network
void freeNetwork(network *N) {
  if (! N) 
    return;
  htFree(N->V);
  free(N->u);
  free(N->v);
  free(N->e);
  free(N);
}

//=================================================================
// Gets a vertex given its label
vertex *getVertex(network *N, char *label) {
  if (! N || ! label)
    return NULL;
    
    // we use a dummy vertex to get the key;
    // this is needed because the hash function 
    // operates on vertices and not on plain strings
  strcpy(N->v->label, label);
  return htGetKey(N->V, N->v);
}

//=================================================================
// Adds a vertex to the network
void addVertex(network *N, char *label) {
  strcpy(N->v->label, label);
  if (htHasKey(N->V, N->v))
    return;
  vertex *vertex = newVertex(label);
  htAddKey(N->V, vertex);
}

//=================================================================
// Adds a vertex to the network by label 
// and returns a pointer to it
vertex *addVertexR(network *N, char *label) {
  strcpy(N->v->label, label);
  vertex *v = htGetKey(N->V, N->v);
  if (v) 
    return v;
  vertex *vertex = newVertex(label);
  htAddKey(N->V, vertex);
  return vertex;
}

//================================================================= 
// Corrects antiparallel edges by adding a new vertex and two edges
static void correctAntiparallel(network *N, vertex *from, 
                                vertex *to, size_t cap) {
  
  char label[MAX_LABEL] = "";
  strcpy(label, from->label);
  strcat(label, "2");
  vertex *v = addVertexR(N, label);
  addEdgeC(N, from, v, cap);
  from->inDegree++;
  addEdgeC(N, v, to, cap);
  to->inDegree++;
  if (cap != SIZE_MAX && cap > N->maxCap)
      N->maxCap = cap;
  printf("Corrected antiparallel edge %s -> %s\n"
         "by adding %s -> %s and %s -> %s\n", 
          from->label, to->label, from->label, 
          v->label, v->label, to->label);
}
  
//=================================================================
// Adds an edge to the network along with its residual counterpart
void addEdgeC(network *N, vertex *from, vertex *to, size_t cap) {
  if (! N || ! from || ! to || from == to)
    return;
  
  N->e->to = to;
  edge *e = htGetVal(N->V, from, N->e);

    // correct antiparallel edges  
  if (e && e->residual) {
    correctAntiparallel(N, from, to, cap);
    return;
  }

  if (e) return;

  e = newEdge(from, to, cap, false);
  htAddKeyVal(N->V, from, e);
  to->inDegree++;
    // add the residual edge
  edge *r = newEdge(to, from, 0, true);
  htAddKeyVal(N->V, to, r);
    // since the last edge belongs to the residual network,
    // correct dll size for residual edge
  dll *adjList = getNeighbors(N, to);
  adjList->size--;
    // set pointers to the reverse edges
  e->rev = r;
  r->rev = e;
  
  if (cap != SIZE_MAX && cap > N->maxCap)
    N->maxCap = cap;
  N->nEdges++;
}

//=================================================================
// Adds an edge to the network by label with a given capacity
void addEdgeCL(network *N, char *from, char *to, size_t cap) {
  if (!N || ! from || ! to) 
    return;
  strcpy(N->u->label, from);
  strcpy(N->v->label, to);
  addEdgeC(N, htGetKey(N->V, N->u), htGetKey(N->V, N->v), cap);
}

//=================================================================
// Adds an edge to the network with unit capacity
void addEdge(network *N, vertex *from, vertex *to) {
  addEdgeC(N, from, to, 1);
}

//=================================================================
// Adds an edge to the network by label with unit capacity
void addEdgeL(network *N, char *from, char *to) {
  addEdgeCL(N, from, to, 1);
}

//=================================================================
// Gets an edge given the source and destination vertices
edge *getEdge(network *N, vertex *from, vertex *to) {
  if (! N || ! from || ! to) 
    return NULL;
  N->e->to = to;
  return htGetVal(N->V, from, N->e);
}

//=================================================================
// Gets an edge given the source and destination vertices by label
edge *getEdgeL(network *N, char *from, char *to) {
  if (! N || ! from || ! to) 
    return NULL;
  strcpy(N->u->label, from);
  strcpy(N->v->label, to);
  return getEdge(N, htGetKey(N->V, N->u), htGetKey(N->V, N->v));
}

//=================================================================
// Returns true if the vertex exists in the network
bool hasVertex(network *N, char *label) {
  if (! N || ! label) 
    return false;
  strcpy(N->v->label, label);
  return htHasKey(N->V, N->v);
}

//=================================================================
// Sets the pointer to the adjacency list of a vertex
// If the vertex has no adj list or is not in the network,
// the pointer is set to NULL
// Returns true if the source vertex is in the network
bool hasAdjList(network *N, vertex *v, dll **adjList) {
  if (! N || ! v) 
    return false;
  return htHasKeyVals(N->V, v, adjList);
}

//=================================================================
// Returns the adjacency list of a vertex
dll *getNeighbors(network *N, vertex *v) {
  if (! N || ! v) 
    return NULL;
  return htGetVals(N->V, v);
}

//=================================================================
// Returns the adjacency list of a vertex by label
dll *getNeighborsL(network *N, char *label) {
  if (! N || ! label) 
    return NULL;
  strcpy(N->v->label, label);
  return getNeighbors(N, htGetKey(N->V, N->v));
}

//=================================================================
// Returns true if the edge exists in the network
bool hasEdge(network *N, vertex *from, vertex *to) {
  if (! N || ! from || ! to) 
    return false;
  N->e->to = to;
  edge *e = NULL;
  e = htGetVal(N->V, from, N->e);
  return e && ! e->residual;
}

//=================================================================
// Returns true if the edge is in the network given the vertex
// labels
bool hasEdgeL(network *N, char *from, char *to) {
  if (! N || ! from || ! to) 
    return false;
  strcpy(N->u->label, from);
  strcpy(N->v->label, to);
  return hasEdge(N, htGetKey(N->V, N->u), htGetKey(N->V, N->v));
}

//=================================================================
// Deletes an edge from the network
// If the edge is not in the network, nothing happens
void delEdge(network *N, vertex *from, vertex *to) {
  if (! N || ! from || ! to) 
    return;
  
  N->e->to = to;
  if (htDelVal(N->V, from, N->e)) {
    N->nEdges--;
    to->inDegree--;
  }
    
    // remove the residual edge
  N->e->to = from;
  htDelVal(N->V, to, N->e);
}

//=================================================================
// Deletes an edge from the network by label
void delEdgeL(network *N, char *from, char *to) {
  if (! N || ! from || ! to) 
    return;
  strcpy(N->u->label, from);
  strcpy(N->v->label, to);
  delEdge(N, htGetKey(N->V, N->u), htGetKey(N->V, N->v));
}

//=================================================================
// Gets the 'first' edge in the network and sets the iterator
// to the next edge
edge *firstE(network *N) {
  if (! N)
    return NULL;
  N->z = firstV(N); 
  if (N->z && outDegree(N, N->z) == 0) 
    while ((N->z = nextV(N)) && outDegree(N, N->z) == 0)
  if (! N->z) 
    return NULL;
  N->adjList = getNeighbors(N, N->z);
  return dllFirst(N->adjList);
}

//=================================================================
// Gets the next edge in the network and sets the iterator to the
// next edge; if there are no more edges, NULL is returned
edge *nextE(network *N) {
  if (! N)
    return NULL;
  edge *e = dllNext(N->adjList);
  if (! e) {
    while ((N->z = nextV(N)) && outDegree(N, N->z) == 0);
    if (N->z) {
      N->adjList = getNeighbors(N, N->z);
      e = dllFirst(N->adjList);
    } 
  }
  return e;
}

//=================================================================
// Adds source and destination vertices if they do not exist
// and adds an edge between them with the given capacity
void addVandEC(network *N, char *from, char *to, 
              size_t capacity) {
  vertex *u = addVertexR(N, from);
  vertex *v = addVertexR(N, to);
  addEdgeC(N, u, v, capacity);
}

//=================================================================
// Adds source and destination vertices if they do not exist
// and adds an edge between them with unit capacity
void addVandE(network *N, char *from, char *to) {
  addVandEC(N, from, to, 1);
}

//=================================================================
// Reads a network from stdin
void readNetwork(network *N) {
  char from[MAX_LABEL], to[MAX_LABEL];

  if (! N) 
    return;

  if (N->weight == WEIGHTED) {
    size_t capacity;
    while (scanf("%s %s", from, to) == 2) {
      if (! scanf("%zu", &capacity)) {  // infinite capacity
        assert(scanf("%*s") == 0);
        addVandEC(N, from, to, SIZE_MAX);
      } else
        addVandEC(N, from, to, capacity);
    }
  } else 
    while (scanf("%s %s", from, to) == 2) 
      addVandE(N, from, to);
  }

//=================================================================
// Returns the 'first' vertex in the network and sets the iterator
// to the next vertex
// NULL if the network is empty
vertex *firstV(network *N) {
  if (! N) return NULL;
  htEntry *entry = htFirst(N->V);
  return entry ? (vertex *)entry->key : NULL;
}

//=================================================================
// Returns the next vertex in the network
// NULL if we have reached the end
vertex *nextV(network *N) {
  if (! N) return NULL;
  htEntry *entry = htNext(N->V);
  return entry ? (vertex *)entry->key : NULL;
}

//=================================================================
// Returns a copy of the network
network *copynetwork(network *N) {
  if (! N) return NULL;
  network *copy = newNetwork(nVertices(N), N->weight);

  for (edge *e = firstE(N); e; e = nextE(N)) 
    addVandEC(copy, e->from->label, e->to->label, e->cap);
  return copy;
}

//=================================================================
#undef MAX_LABEL