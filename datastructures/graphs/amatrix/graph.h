/* 
  Generic graph implementation using an adjacency matrix
  Description: 
    Some algorithms require a graph to be represented as
    an adjacency matrix. This implementation provides
    a generic graph data structure that does just that.
    It is only recommended for small graphs or (nearly)
    complete graphs, as the adjacency matrix has a space
    complexity of O(|V|²).

    Labels are used to identify the vertices and 
    are stored in a hash map for fast lookup.
    The graph can be directed or undirected, and the
    edges can be weighted or unweighted.

    Vertices can have different flavors and are defined
    in vertex.h. You can add your own vertex types by 
    modifying this header. You could, for example, add 
    a vertex with a void pointer to satellite data.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "../../htables/single-value/sstmap.h"
#include "vertex.h"

  // graph types
typedef enum { DIRECTED, UNDIRECTED } graphType;
typedef enum { WEIGHTED, UNWEIGHTED } weightType; 

  // graph data structure
typedef struct {
  vertex **V;         // array of vertices
  double **W;         // adjacency matrix of weights
  size_t nEdges;      // number of edges in the graph
  size_t capacity;    // maximum number of vertices
  size_t nVertices;   // number of vertices in the graph
  sstMap *indexMap;   // hash map: label to index
  graphType type;     // directed or undirected, 
                      // set to directed by default
  weightType weight;  // weighted or unweighted
  char *label;        // label of the graph
  size_t currentV;    // current vertex index for iteration
  size_t currentFrom; // current source vertex index
  size_t currentTo;   // current destination vertex index
} graph;


  // Creates a new graph; requires the initial capacity
  // and type of the edges (weighted/unweighted)
  // By default, the graph is directed 
graph *newGraph (size_t capacity, weightType weight);

  // Sets the graph type to undirected
  // Should be called before adding any edges
void setUndirected(graph *G);

  // Deallocates the graph
void freeGraph(graph *G);

  // Gets the vertex given its label
vertex *getVertex(graph *G, char *label);

  // Adds a vertex with given label to the graph; 
  // the vertex is not added if it already exists
void addVertex(graph *G, char *label);

  // Same as addVertex, but returns a 
  // pointer to the vertex
vertex *addVertexR(graph *G, char *label);

  // Returns true if the vertex exists in the graph
bool hasVertex(graph *G, char *label);

  // Adds an unweighted edge to the destination 
  // vertex's adjList
  // In an undirected graph, the edge is also 
  // added to the destination vertex's adjList
void addEdge(graph *G, vertex *from, vertex *to);

  // Same as addEdge, but for a weighted edge
void addEdgeW(graph *G, vertex *from, vertex *to, 
              double weight);

  // Adds an unweighted edge given the labels of 
  // the source and destination vertices
  // In an undirected graph, the edge is also
  // added to the destination vertex's adjList
void addEdgeL(graph *G, char *from, char *to);

  // Same as addEdgeL, but for a weighted edge
void addEdgeWL(graph *G, char *from, char *to, 
               double weight);

  // Adds the vertices if they do not exist, and
  // adds the edge between them with the given weight
  // In an undirected graph, the edge is also added
  // to the destination vertex's adjList
void addVandE(graph *G, char *from, char *to);

  // Same as addVandE, but for a weighted edge
void addVandEW(graph *G, char *from, char *to, 
               double weight);

  // Returns true if the edge exists in the graph
bool hasEdge(graph *G, vertex *from, vertex *to);

  // Returns true if the edge exists in the graph
  // given the labels of the source and destination 
bool hasEdgeL(graph *G, char *from, char *to);

  // Removes an edge from source vertex's adjList
  // If the graph is undirected, it is also removed 
  // from the destination vertex's adjList
void delEdge(graph *G, vertex *from, vertex *to);

  // Removes an edge from the graph given the 
  // labels of the source and destination 
void delEdgeL(graph *G, char *from, char *to);

  // Shows the graph
void showGraph(graph *G);

  // Shows a vertex and its adjacency list
void showVertex(graph *G, vertex *v);

  // Shows a vertex and its adjList given the label
void showVertexL(graph *G, char *label);

  // Reads a graph from standard input
void readGraph(graph *G);

  // Returns a copy of the graph
graph *copyGraph(graph *G);

  // Returns the transposed graph of G
  // This is a copy of G with all edges reversed
graph *transposeGraph(graph *G);

  // Returns a sorted array of the vertices
  // Sorted by label
vertex **sortVertices(graph *G);

  // Returns the number of vertices in the graph
static inline size_t nVertices(graph *G) {
  return G->nVertices;
}

  // Returns the number of edges in the graph
static inline size_t nEdges(graph *G) {
  return G->nEdges;
}

  // Returns the out-degree of a vertex
static inline size_t outDegree(vertex *v) {
  return v ? v->outDegree : 0;
}

  // Same as above, but by label
static inline size_t outDegreeL(graph *G, char *label) {
  vertex *v = getVertex(G, label);
  return v ? v->outDegree : 0;
}

  // Returns the in-degree of a vertex
static inline size_t inDegree(vertex *v) {
  return v ? v->inDegree : 0;
}

  // Same as above, but by label
static inline size_t inDegreeL(graph *G, char *label) {
  vertex *v = getVertex(G, label);
  return v ? v->inDegree : 0;
}

  // Returns the degree of a vertex
static inline size_t degree(graph *G, vertex *v) {
  if (G->type == UNDIRECTED)
    return outDegree(v);
  return outDegree(v) + inDegree(v);
}

  // Returns the degree of a vertex given its label
static inline size_t degreeL(graph *G, char *label) {
  vertex *v = getVertex(G, label);
  return v ? degree(G, v) : 0;
}

  // Determines if the vertex is isolated
static inline bool isIsolated(graph *G, vertex *v) {
  return degree(G, v) == 0;
}

  // Same as above, but by label
static inline bool isIsolatedL(graph *G, char *label) {
  return degreeL(G, label) == 0;
}

  // Sets the label of the graph
static inline void setGLabel(graph *G, char *label) {
  G->label = label;
}

  // Returns true if the graph is empty
static inline bool graphIsEmpty(graph *G) {
  return nVertices(G) == 0;
}

  // Gets the first neighbor of a vertex; the pointer to
  // to-vertex is set to the first neighbor; returns a
  // pointer to the weight of the edge so that it can be
  // modified; returns NULL if there are no neighbors
double *firstN(graph *G, vertex *from, vertex **to);

  // Gets the next neighbor of a vertex; the pointer to 
  // to-vertex is set to the next neighbor; returns a
  // pointer to the weight of the edge so that it can be
  // modified; returns NULL if there are no more neighbors
double *nextN(graph *G, vertex *from, vertex **to);

  // Returns the first edge of the iteration; the pointers
  // from and to are set to the source and destination 
  // vertices; returns a pointer to the weight of the edge
  // so that it can be modified; returns NULL if there are
  // no edges
double *firstE(graph *G, vertex **from, vertex **to);

  // Returns the next edge of the iteration; the pointers
  // from and to are set to the source and destination
  // vertices; returns a pointer to the weight of the edge
  // so that it can be modified; returns NULL if there are
  // no more edges
double *nextE(graph *G, vertex **from, vertex **to);

  // Returns the first vertex of the iteration 
  // NULL if there are no vertices
vertex *firstV(graph *G);

  // Returns the current vertex of the iteration;
  // NULL if there are no more vertices
vertex *nextV(graph *G);
  
#endif // GRAPH_H_INCLUDED