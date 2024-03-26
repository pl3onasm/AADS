/* 
  Generic graph implementation
  Description: 
    A graph G = (V, E) is represented as a hash table, 
    where the keys are the vertices and the values are
    the adjacency lists. The adjacency lists are doubly
    linked lists of edges, which contain a pointer to the
    destination vertex, and the weight of the edge.
    The graph can be directed or undirected, and the
    edges can be weighted or unweighted.
    Nodes are represented as vertex structs, which are 
    defined in vertex.h. They can have different definitions
    depending on the application or algorithm. You can even
    make one with a void pointer to satelite data.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "../../htables/htable/htable.h"
#include "vertex.h"

  // graph types
typedef enum { WEIGHTED, UNWEIGHTED } edgeType;
typedef enum { DIRECTED, UNDIRECTED } graphType;

  // graph data structure
typedef struct {      
  vertex *to;         // pointer to the destination vertex
  double weight;      // weight of the edge
} edge;

typedef struct {
  ht *V;              // hash table of vertices
  size_t nEdges;      // number of edges in the graph
  graphType type;     // directed or undirected
  edgeType weight;    // weighted or unweighted
  edge *e;            // dummy edge 
  vertex *u;          // dummy vertex
  vertex *v;          // dummy vertex
} graph;


  // Creates a new graph; requires the initial capacity,
  // the type of the graph (directed/undirected) and the
  // type of the edges (weighted/unweighted)
graph *newGraph (size_t capacity, graphType type, 
                 edgeType weight);

  // Deallocates the graph
void freeGraph(graph *G);

  // Sets the pointer to the adjacency list of a vertex
  // If the vertex has no adj list or is not in the graph,
  // the pointer is set to NULL
  // Returns true if the source vertex is in the graph
bool hasAdjList(graph *G, vertex *v, dll **adjList);

  // Gets the adjacency list of a vertex
dll *getNeighbors(graph *G, vertex *v);

  // Gets the adjacency list of a vertex by label
dll *getNeighborsL(graph *G, char *label);

  // Gets the vertex given its label
vertex *getVertex(graph *G, char *label);

  // Adds a node with the given label to the graph; 
  // the data of the node is set to NULL
void addVertex(graph *G, char *label);

  // Same as addVertex, but returns a 
  // pointer to the vertex
vertex *addVertexR(graph *G, char *label);

  // Returns true if the vertex is in the graph
bool hasVertex(graph *G, char *label);

  // Gets the edge between two vertices
edge *getEdge(graph *G, vertex *from, vertex *to);

  // Adds an unweighted edge to the destination 
  // vertex's adjList
  // In an undirected graph, the edge is also 
  // added to the source vertex's adjList
void addEdge(graph *G, vertex *from, vertex *to);

  // Same as addEdge, but for a weighted edge
void addEdgeW(graph *G, vertex *from, vertex *to, 
              double weight);

  // Adds an unweighted edge given the labels of 
  // the source and destination vertices
void addEdgeL(graph *G, char *from, char *to);

  // Same as addEdgeL, but for a weighted edge
void addEdgeWL(graph *G, char *from, char *to, 
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

  // Checks if a vertex is in the graph
  // A vertex is in the graph if it has at least
  // one edge in its adjacency list
inline bool inGraph(graph *G, vertex *v) {
  return dllSize(getNeighbors(G, v)) > 0;
}

  // Same as above, but by label
bool inGraphL(graph *G, char *label);

inline size_t nVertices(graph *G) {
  return htSize(G->V);
}

inline size_t nEdges(graph *G) {
  return G->nEdges;
}

  // Returns the out-degree of a vertex
inline size_t outDegree(graph *G, vertex *v) {
  return dllSize(getNeighbors(G, v));
}

  // Same as above, but by label
inline size_t outDegreeL(graph *G, char *label) {
  return dllSize(getNeighborsL(G, label));
}

  // Sets the label of the graph
inline void setGLabel(graph *G, char *label) {
  htSetLabel(G->V, label);
}

  // Sets the string delimiter for showing 
  // the adjacency lists (default is ", ")
inline void setAdjDelim(graph *G, char *adjDelim) {
  htSetValDelim(G->V, adjDelim);
}

  // Returns the weight of an edge
inline double getWeight(edge *e) {
  return e->weight;
}

  // Returns true if the graph is empty
inline bool graphIsEmpty(graph *G) {
  return htIsEmpty(G->V);
}

  // Returns the first edge in the adjacency list
  // and sets the iterator to the next edge;
  // returns NULL if the list is empty
edge *firstE(dll *adjList);

  // Returns the current edge in the adjacency list
  // and updates the iterator to the next edge;
  // NULL if the end of the list is reached
edge *nextE(dll *adjList);

  // Returns the first vertex in the graph
  // and sets the iterator to the next vertex
vertex *firstV(graph *G);

  // Returns the current vertex in the graph
  // and updates the iterator to the next vertex
vertex *nextV(graph *G);
  
#endif // GRAPH_H_INCLUDED_