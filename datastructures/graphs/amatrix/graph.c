/* 
  Generic graph implementation using an adjacency matrix
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "graph.h"
#include "../../../lib/clib/clib.h"
#include <ctype.h>
#include <float.h>

#define MAX_VERTEX_LABEL 50

//=================================================================
// Creates a new graph
graph *newGraph (size_t capacity, weightType weight) {

  graph *G = safeCalloc(1, sizeof(graph));
  G->capacity = capacity;
  G->W = safeCalloc(capacity, sizeof(double *));
  for (size_t i = 0; i < capacity; i++) {
    G->W[i] = safeCalloc(capacity, sizeof(double));
    for (size_t j = 0; j < capacity; j++)
      G->W[i][j] = DBL_MAX;
  }
  G->V = safeCalloc(capacity, sizeof(vertex *));
  G->indexMap = sstMapNew(CASE_SENSITIVE, capacity);
  sstMapCopyKeys(G->indexMap);
  G->label = "GRAPH";
  G->type = DIRECTED;
  G->weight = weight;
  return G;
}

//=================================================================
// Deallocates a vertex
void freeVertex(void *v) {
  if (! v) return;
  vertex *vert = (vertex *)v;
  free(vert->label);
  free(vert);
}

//=================================================================
// Deallocates the graph
void freeGraph(graph *G) {
  if (! G) 
    return;
  for (size_t i = 0; i < G->capacity; i++) {
    free(G->W[i]);
    freeVertex(G->V[i]);
  }
  free(G->W);
  free(G->V);
  sstMapFree(G->indexMap);
  free(G);
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
// Shows a vertex
void showVertex(graph *G, vertex *v) {
  if (! G || ! v) 
    return;
  if (v->outDegree == 0) {
    printf("%s[0]\n", v->label);
    return;
  }
  printf("%s[%zu]: ", v->label, v->outDegree);
  size_t count = 0;
  if (G->weight == WEIGHTED) {
    for (size_t i = 0; i < G->nVertices; i++) {
      if (G->W[v->idx][i] != DBL_MAX) {
        printf("%s(%g)", G->V[i]->label, G->W[v->idx][i]);
        if (++count < v->outDegree)
          printf(", ");
      }
    }
  } else {
    for (size_t i = 0; i < G->nVertices; i++) {
      if (G->W[v->idx][i] != DBL_MAX) {
        printf("%s", G->V[i]->label);
        if (++count < v->outDegree)
          printf(", ");
      }
    }
  }
  printf("\n");
}

//=================================================================
// Comparison function between vertices for qsort
static int cmpVertex(void const *v1, void const *v2) {
  return strcmp((*(vertex **)v1)->label, (*(vertex **)v2)->label);
}

//=================================================================
// Sorts the vertices in the graph by label
// Returns a sorted array of pointers to the vertices
vertex **sortVertices(graph *G) {
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
          G->label, 
          G->type == DIRECTED ? "Directed" : "Undirected",
          G->weight == UNWEIGHTED ? "Unweighted" : "Weighted",
          nVertices(G), 
          G->nEdges);

  // sort the vertices
  vertex **vertices = sortVertices(G);

  // show the vertices and their adjacency lists
  for (size_t i = 0; i < nVertices(G); i++) {
    printf("  ");
    showVertex(G, vertices[i]);
  }
  
  printf("--------------------\n\n");
  free(vertices);
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
    
  size_t index;
  if (! sstMapHasKeyVal(G->indexMap, label, &index)) 
    return NULL;
  
  return G->V[index];
}

//=================================================================
// Stores the vertex in the graph; resizes the graph if necessary
static void storeVertex(graph *G, vertex *v) {
  if (! G || ! v) 
    return;

  if (G->nVertices == G->capacity) {
    G->capacity *= 2;
    for (size_t i = 0; i < G->nVertices; i++) {
      G->W[i] = safeRealloc(G->W[i], G->capacity * sizeof(double));
      for (size_t j = G->nVertices; j < G->capacity; j++)
        G->W[i][j] = DBL_MAX;
    }
    G->W = safeRealloc(G->W, G->capacity * sizeof(double *));
    for (size_t i = G->nVertices; i < G->capacity; i++) {
      G->W[i] = safeCalloc(G->capacity, sizeof(double));
      for (size_t j = 0; j < G->capacity; j++)
        G->W[i][j] = DBL_MAX;
    }

    G->V = safeRealloc(G->V, G->capacity * sizeof(vertex *));
      // initialize the new vertices to NULL
    for (size_t i = G->nVertices; i < G->capacity; i++)
      G->V[i] = NULL;
  }

  G->V[G->nVertices++] = v;
}

//=================================================================
// Adds a vertex to the graph
void addVertex(graph *G, char *label) {
  if (! G || ! label) 
    return;
  if (sstMapHasKey(G->indexMap, label)) 
    return;
  vertex *v = newVertex(label);
  sstMapAddKey(G->indexMap, label, G->nVertices);
  v->idx = G->nVertices;
  storeVertex(G, v);
}

//=================================================================
// Adds a vertex to the graph by label and returns a pointer to it
vertex *addVertexR(graph *G, char *label) {
  if (! G || ! label) 
    return NULL;
  if (sstMapHasKey(G->indexMap, label)) 
    return G->V[sstMapGetVal(G->indexMap, label)];
  vertex *v = newVertex(label);
  sstMapAddKey(G->indexMap, label, G->nVertices);
  v->idx = G->nVertices;
  storeVertex(G, v);
  return v;
}

//=================================================================
// Gets the weight of an edge
double getWeight(graph *G, vertex *from, vertex *to) {
  if (! G || ! from || ! to) 
    return 0;
  return G->W[from->idx][to->idx];
}

//=================================================================
// Gets the weight of an edge by label
double getWeightL(graph *G, char *from, char *to) {
  if (! G || ! from || ! to) 
    return 0;
  size_t i, j;
  if (! sstMapHasKeyVal(G->indexMap, from, &i) || 
      ! sstMapHasKeyVal(G->indexMap, to, &j)) 
    return 0; 

  return G->W[i][j];
}
  
//=================================================================
// Adds a weighted edge to the graph; if the edge already exists,
// nothing happens
void addEdgeW(graph *G, vertex *from, vertex *to, double weight) {
  if (! G || ! from || ! to)
    return;

  if (from->idx >= G->capacity || to->idx >= G->capacity) 
    return;
  if (G->W[from->idx][to->idx] != DBL_MAX)
    return;

  G->W[from->idx][to->idx] = weight;
  G->nEdges++;
  to->inDegree++;
  from->outDegree++;

  if (G->type == UNDIRECTED) {
    G->W[to->idx][from->idx] = weight;
    from->inDegree++;
    to->outDegree++;
  } 
}

//=================================================================
// Adds a weighted edge to the graph by label
void addEdgeWL(graph *G, char *from, char *to, double weight) {
  if (! G || ! from || ! to) 
    return;

  size_t i, j;
  if (! sstMapHasKeyVal(G->indexMap, from, &i) || 
      ! sstMapHasKeyVal(G->indexMap, to, &j)) 
    return;

  addEdgeW(G, G->V[i], G->V[j], weight);
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
  addEdgeWL(G, from, to, 1);
}

//=================================================================
// Returns true if the vertex exists in the graph
bool hasVertex(graph *G, char *label) {
  if (! G || ! label) 
    return false;
  return sstMapHasKey(G->indexMap, label);
}

//=================================================================
// Returns true if the edge exists in the graph
bool hasEdge(graph *G, vertex *from, vertex *to) {
  if (! G || ! from || ! to) 
    return false;
  return G->W[from->idx][to->idx] != DBL_MAX;
}

//=================================================================
// Returns true if the edge is in the graph given the vertex labels
bool hasEdgeL(graph *G, char *from, char *to) {
  if (! G || ! from || ! to) 
    return false;
  
  size_t i, j;
  if (! sstMapHasKeyVal(G->indexMap, from, &i) || 
      ! sstMapHasKeyVal(G->indexMap, to, &j)) 
    return false;

  return G->W[i][j] != DBL_MAX;
}

//=================================================================
// Deletes an edge from the graph
// If the edge is not in the graph, nothing happens
void delEdge(graph *G, vertex *from, vertex *to) {
  if (! G || ! from || ! to) 
    return;
  
  if (G->W[from->idx][to->idx] == DBL_MAX)
    return;
  G->W[from->idx][to->idx] = DBL_MAX;
  G->nEdges--;  
  to->inDegree--;
  from->outDegree--;

  if (G->type == UNDIRECTED) {
    G->W[to->idx][from->idx] = DBL_MAX;
    from->inDegree--;
    to->outDegree--;
  }
}

//=================================================================
// Deletes an edge from the graph by label
void delEdgeL(graph *G, char *from, char *to) {
  if (! G || ! from || ! to) 
    return;
  
  size_t i, j;
  if (! sstMapHasKeyVal(G->indexMap, from, &i) || 
      ! sstMapHasKeyVal(G->indexMap, to, &j)) 
    return;

  delEdge(G, G->V[i], G->V[j]);
}

//=================================================================
// Gets the first neighbor of a vertex; the pointer to the
// to vertex is set to the neighbor; returns a pointer to the
// weight of the edge so that it can be modified
// Returns NULL if there are no neighbors
double *firstN (graph *G, vertex *from, vertex **to) {
  if (! G || ! from) 
    return NULL;
  for (size_t i = 0; i < G->nVertices; i++) {
    if (G->W[from->idx][i] != DBL_MAX) {
      *to = G->V[i];
      G->currentN = i;
      return &G->W[from->idx][i];
    }
  }
  G->currentN = 0;
  return NULL;
}

//=================================================================
// Gets the next neighbor of a vertex; the pointer to the
// to vertex is set to the neighbor; returns a pointer to the
// weight of the edge so that it can be modified
// Returns NULL if there are no more neighbors
double *nextN (graph *G, vertex *from, vertex **to) {
  if (! G || ! from) 
    return NULL;
  for (size_t i = G->currentN + 1; i < G->nVertices; i++) {
    if (G->W[from->idx][i] != DBL_MAX) {
      *to = G->V[i];
      G->currentN = i;
      return &G->W[from->idx][i];
    }
  }
  G->currentN = 0;
  return NULL;
}

//=================================================================
// Gets the first edge in the graph; returns a pointer to the
// weight of the edge so that it can be modified; from and to
// are set to the source and destination vertices
double *firstE(graph *G, vertex **from, vertex **to) {
  if (! G) 
    return NULL;
  for (size_t i = 0; i < G->nVertices; i++) {
    for (size_t j = 0; j < G->nVertices; j++) {
      if (G->W[i][j] != DBL_MAX) {
        *from = G->V[i];
        *to = G->V[j];
        G->currentFrom = i;
        G->currentTo = j;
        return &G->W[i][j];
      }
    }
  }
  G->currentFrom = 0;
  G->currentTo = 0;
  return NULL;
}

//=================================================================
// Gets the next edge in the graph; returns a pointer to the
// weight of the edge so that it can be modified; from and to are
// set to the source and destination vertices
// Returns NULL if there are no more edges
double *nextE(graph *G, vertex **from, vertex **to) {
  if (! G) 
    return NULL;

  for (size_t i = G->currentFrom; i < G->nVertices; i++) {
    for (size_t j = G->currentFrom == i ? G->currentTo + 1 : 0; 
          j < G->nVertices; j++) {
      if (G->W[i][j] && G->W[i][j] != DBL_MAX) {
        *from = G->V[i];
        *to = G->V[j];
        G->currentFrom = i;
        G->currentTo = j;
        return &G->W[i][j];
      }
    }
  }
  G->currentFrom = 0;
  G->currentTo = 0;
  return NULL;
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
  if (! G || ! from || ! to)
    return;
  vertex *src = addVertexR(G, from);
  vertex *dest = addVertexR(G, to);
  addEdgeW(G, src, dest, weight);
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
// Returns the first vertex in the graph
vertex *firstV(graph *G) {
  if (! G) 
    return NULL;
  G->currentV = 0;
  if (G->nVertices > 0)
    return G->V[G->currentV++];
  return NULL;
}

//=================================================================
// Returns the next vertex in the graph; returns NULL if there
// are no more vertices
vertex *nextV(graph *G) {
  if (! G) 
    return NULL;
  if (G->currentV < G->nVertices)
    return G->V[G->currentV++];
  return NULL;
}

//=================================================================
// Returns a copy of the graph
graph *copyGraph(graph *G) {
  if (! G) return NULL;
  graph *copy = newGraph(G->capacity, G->weight);
  copy->type = G->type;
  copy->label = G->label;
  vertex *from, *to;

  for (vertex *v = firstV(G); v; v = nextV(G))
    addVertex(copy, v->label);

  for (double *w = firstE(G, &from, &to); w; 
       w = nextE(G, &from, &to))  
    addEdgeWL(copy, from->label, to->label, *w);

  return copy;
}

//=================================================================
// Returns the transposed graph of G
// This is a copy of G but with the edges reversed
graph *transposeGraph(graph *G) {
  if (! G) return NULL;

  graph *transposed = newGraph(G->capacity, G->weight);
  transposed->type = G->type;
  transposed->label = "TRANSPOSE";
  vertex *from, *to;

  for (vertex *v = firstV(G); v; v = nextV(G))
    addVertex(transposed, v->label);

  for (double *w = firstE(G, &from, &to); w; 
       w = nextE(G, &from, &to))  
    addEdgeWL(transposed, to->label, from->label, *w);

  return transposed;
}

//=================================================================
#undef MAX_VERTEX_LABEL