/* 
  Generic graph implementation
  Some tests with a fictional animal graph
  Author: David De Potter
*/

#include "graph.h"

int main() {
  graph *G = newGraph(20, UNWEIGHTED);
  //setUndirected(G);   // uncomment to make the graph undirected
  setGLabel(G, "ANIMAL GRAPH");

  addVandE(G, "cat", "dog");
  addVandE(G, "cat", "bird");
  addVandE(G, "cat", "fish");
  addVandE(G, "dog", "mouse");
  addVandE(G, "dog", "elephant");
  addVandE(G, "dog", "lion");
  addVandE(G, "fox", "tiger");
  addVandE(G, "dog", "bear");
  addVandE(G, "goat", "wolf");
  addVandE(G, "pig", "cow");
  addVandE(G, "pig", "sheep");
  addVandE(G, "mouse", "goat");
  addVandE(G, "mouse", "chicken");
  addVandE(G, "mouse", "duck");
  addVandE(G, "duck", "rabbit");
  addVandE(G, "rabbit", "fox");
  addVandE(G, "fox", "wolf");
  addVandE(G, "wolf", "deer");
  addVandE(G, "deer", "horse");
  addVandE(G, "horse", "horse");
  addVandE(G, "horse", "elephant");
  addVandE(G, "elephant", "lion");
  addVandE(G, "lion", "tiger");
  addVandE(G, "tiger", "bear");
  addVandE(G, "bear", "elephant");
  addVandE(G, "elephant", "wolf");
  addVandE(G, "wolf", "lion");
  addVandE(G, "tiger", "bear");
  addVandE(G, "fish", "wolf");
  addVandE(G, "cat", "horse");

  showGraph(G);

    // tries to add an existing edge
  addEdgeL(G, "fish", "wolf");

    // tries to add an edge with a non-existing vertex
  addEdgeL(G, "fish", "shark");

    // tries to add an existing vertex
  addVertex(G, "fish");

    // tries to add an existing vertex with a different case
    // the graph is case sensitive, so a new vertex is added
  addVertex(G, "Fish");

    // remove some edges
  delEdgeL(G, "tiger", "bear");
  delEdgeL(G, "fish", "wolf");
  delEdgeL(G, "cat", "bird");

    // removes a non-existing edge
  delEdgeL(G, "rabbit", "bear");
  delEdgeL(G, "cat", "cat");

  printf("\nAfter adding and removing some edges and vertices:\n\n");

    // shows the resulting graph
  showGraph(G);

    // checks if the edges are in the graph
  printf("\nChecking if some edges are in the graph:\n");
  printf("\n\nEdge %s-%s is in the graph: %s\n", "cat", "bird", 
         hasEdgeL(G, "cat", "bird") ? "true" : "false");
  printf("Edge %s-%s is in the graph: %s\n", "mouse", "chicken",
          hasEdgeL(G, "mouse", "chicken") ? "true" : "false");
  printf("Edge %s-%s is in the graph: %s\n", "elephant", "horse",
          hasEdgeL(G, "elephant", "horse") ? "true" : "false");

    // check some properties of vertices
  printf("\nVertex %s has outdegree: %zu\n", "cat", outDegreeL(G, "cat"));
  printf("Vertex %s has indegree: %zu\n", "elephant", inDegreeL(G, "elephant"));
  printf("Vertex %s has degree: %zu\n", "mouse", degreeL(G, "mouse"));
  printf("Vertex %s is isolated: %s\n", "fish", 
         isIsolatedL(G, "fish") ? "true" : "false");
  printf("Vertex %s is isolated: %s\n", "bird", 
         isIsolatedL(G, "bird") ? "true" : "false");

    // tests the edge iterator over the graph
  printf("\nAll edges in the graph:\n");
  size_t n = 0;
  vertex *src;
  for (edge *e = firstE(G, &src); e; e = nextE(G, &src))
    printf("%zu: Edge %s-%s\n", ++n, src->label, e->to->label);

    // tests the transpose of the graph
  graph *GT = transposeGraph(G);
  setGLabel(GT, "TRANSPOSE OF\n ANIMAL GRAPH");

  showGraph(GT);

  freeGraph(G);
  freeGraph(GT);
  return 0;
} 