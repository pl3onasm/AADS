/* 
  Generic flow network implementation
  Description: 
    A network N = (V, E) is implemented as a hash table, 
    where the keys are the vertices and the values are
    the adjacency lists. The adjacency lists are doubly
    linked lists of edges.
    Vertices and edges can have different flavors and are
    defined in vertex.h and edge.h. You can add your own
    vertex and edge types by modifying these headers. 
    You could, for example, add a vertex with a void 
    pointer to satellite data.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include "../../htables/multi-value/htable.h"
#include "edge.h"
#include "vertex.h"

typedef enum { WEIGHTED, UNWEIGHTED } weightType;

  // network data structure
typedef struct {
  htable *V;          // hash table of vertices
  weightType weight;  // unweighted: edges have no cost
  size_t nEdges;      // number of edges in the network
  edge *e;            // dummy edge for lookup
  dll *adjList;       // dummy adjacency list
  vertex *u;          // dummy vertex for lookup
  vertex *v;          // dummy vertex for lookup  
  vertex *z;          // dummy vertex for iteration
  size_t maxCap;      // maximum edge capacity
  size_t maxFlow;     // maximum flow in the network
} network;


  // Creates a new flow network with given capacity
network *newNetwork (size_t capacity, weightType weight);

  // Deallocates the network
void freeNetwork (network *N);

  // Sets the pointer to the adjList of a vertex
  // If the vertex has no adj list or is not in the network,
  // the pointer is set to NULL
  // Returns true if the source vertex is in the network
bool hasAdjList(network *N, vertex *v, dll **adjList);

  // Gets the adjacency list of a vertex
dll *getNeighbors(network *N, vertex *v);

  // Gets the adjacency list of a vertex by label
dll *getNeighborsL(network *N, char *label);

  // Gets the vertex given its label
vertex *getVertex(network *N, char *label);

  // Adds a vertex with given label to the network; 
  // the vertex is not added if it already exists
void addVertex(network *N, char *label);

  // Same as addVertex, but returns a 
  // pointer to the new vertex
vertex *addVertexR(network *N, char *label);

  // Returns true if the vertex exists in the network
bool hasVertex(network *N, char *label);

  // Gets the edge between two vertices
edge *getEdge(network *N, vertex *from, vertex *to);

  // Gets the edge between two vertices given their labels
edge *getEdgeL(network *N, char *from, char *to);

  // Adds an edge with given capacity between 
  // two vertices
void addEdge(network *N, vertex *from, vertex *to, 
             size_t capacity);

  // Adds an edge with given capacity between 
  // the source and destination given their labels
void addEdgeL(network *N, char *from, char *to, 
              size_t capacity);

  // Adds an edge with given capacity and weight
  // between two vertices
void addEdgeW(network *N, vertex *from, vertex *to, 
              size_t capacity, double weight);

  // Adds an edge with given capacity and weight
  // between the source and dest given their labels
void addEdgeWL(network *N, char *from, char *to, 
               size_t capacity, double weight);

  // Adds the vertices if they do not exist, and
  // adds an edge between them with given capacity
  // and weight (cost of the edge)
void addVandEW(network *N, char *from, char *to, 
               size_t capacity, double weight);

  // Adds the vertices if they do not exist, and
  // adds an edge between them with given capacity
void addVandE(network *N, char *from, char *to, 
              size_t capacity);

  // Returns true if the edge exists in the network
bool hasEdge(network *N, vertex *from, vertex *to);

  // Returns true if the edge exists in the network
  // given the labels of the source and destination 
bool hasEdgeL(network *N, char *from, char *to);

  // Removes an edge from source vertex's adjList
void delEdge(network *N, vertex *from, vertex *to);

  // Removes an edge from the network given the 
  // labels of the source and destination 
void delEdgeL(network *N, char *from, char *to);

  // Shows the network
void showNetwork(network *N);

  // Shows the flow in the network
void showFlow(network *N, vertex *src, vertex *sink);

  // Shows a vertex and its adjacency list
void showVertex(network *N, vertex *v);

  // Shows a vertex and its adjList given the label
void showVertexL(network *N, char *label);

  // Reads a network from standard input
void readNetwork(network *N);

  // Returns a copy of the network
network *copynetwork(network *N);

  // Returns an array of the vertices in the network
  // The array is sorted by the vertex labels
vertex **sortVertices(network *N);

  // Returns the number of vertices in the network
static inline size_t nVertices(network *N) {
  return htSize(N->V);
}

  // Returns the number of edges in the network
static inline size_t nEdges(network *N) {
  return N->nEdges;
}

  // Returns the out-degree of a vertex
static inline size_t outDegree(network *N, vertex *v) {
  return dllSize(getNeighbors(N, v));
}

  // Same as above, but by label
static inline size_t outDegreeL(network *N, char *label) {
  return dllSize(getNeighborsL(N, label));
}

  // Returns the in-degree of a vertex
static inline size_t inDegree(network *N, vertex *v) {
  return v ? v->inDegree : 0;
}

  // Same as above, but by label
static inline size_t inDegreeL(network *N, char *label) {
  vertex *v = getVertex(N, label);
  return v ? v->inDegree : 0;
}

  // Returns the degree of a vertex
static inline size_t degree(network *N, vertex *v) {
  return inDegree(N, v) + outDegree(N, v);
}

  // Returns the degree of a vertex given its label
static inline size_t degreeL(network *N, char *label) {
  return inDegreeL(N, label) + outDegreeL(N, label);
}

  // Determines if the vertex is isolated
static inline bool isIsolated(network *N, vertex *v) {
  return degree(N, v) == 0;
}

  // Determines if the vertex is isolated given its label
static inline bool isIsolatedL(network *N, char *label) {
  return degreeL(N, label) == 0;
}

  // Sets the label of the network
static inline void setNLabel(network *N, char *label) {
  htSetLabel(N->V, label);
}

  // Sets the string delimiter for showing 
  // the adjacency lists (default is ", ")
static inline void setAdjDelim(network *N, char *adjDelim) {
  htSetValDelim(N->V, adjDelim);
}

  // Returns true if the network is empty
static inline bool networkIsEmpty(network *N) {
  return htIsEmpty(N->V);
}

  // Returns the first edge in the network and
  // sets the edge iterator to the next edge
  // Returns NULL if there are no edges
edge *firstE(network *N);

  // Returns the current edge of the iteration
  // and sets the edge iterator to the next edge;
  // Returns NULL if there are no more edges
edge *nextE(network *N);

  // Returns the first vertex in the network
  // and sets the iterator to the next vertex
vertex *firstV(network *N);

  // Returns the current vertex in the network
  // and updates the iterator to the next vertex;
  // returns NULL if there are no more vertices
vertex *nextV(network *N);
  
#endif // NETWORK_H_INCLUDED