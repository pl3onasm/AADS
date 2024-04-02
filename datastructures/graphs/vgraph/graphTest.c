#include "graph.h"

int main() {
  graph *G = newGraph(20, UNWEIGHTED);
  //setUndirected(G);   // uncomment to make the graph undirected
  setGLabel(G, "ANIMAL GRAPH");
  addVertex(G, "cat");
  addVertex(G, "dog");
  addVertex(G, "bird");
  addVertex(G, "fish");
  addVertex(G, "mouse");
  addVertex(G, "elephant");
  addVertex(G, "lion");
  addVertex(G, "tiger");
  addVertex(G, "bear");
  addVertex(G, "wolf");
  addVertex(G, "fox");
  addVertex(G, "rabbit");
  addVertex(G, "deer");
  addVertex(G, "horse");
  addVertex(G, "cow");
  addVertex(G, "pig");
  addVertex(G, "sheep");
  addVertex(G, "goat");
  addVertex(G, "chicken");
  addVertex(G, "duck");

  addEdgeL(G, "cat", "dog");
  addEdgeL(G, "cat", "bird");
  addEdgeL(G, "cat", "fish");
  addEdgeL(G, "dog", "mouse");
  addEdgeL(G, "dog", "elephant");
  addEdgeL(G, "dog", "lion");
  addEdgeL(G, "fox", "tiger");
  addEdgeL(G, "dog", "bear");
  addEdgeL(G, "goat", "wolf");
  addEdgeL(G, "pig", "cow");
  addEdgeL(G, "pig", "sheep");
  addEdgeL(G, "mouse", "goat");
  addEdgeL(G, "mouse", "chicken");
  addEdgeL(G, "mouse", "duck");
  addEdgeL(G, "duck", "rabbit");
  addEdgeL(G, "rabbit", "fox");
  addEdgeL(G, "fox", "wolf");
  addEdgeL(G, "wolf", "deer");
  addEdgeL(G, "deer", "horse");
  addEdgeL(G, "horse", "horse");
  addEdgeL(G, "horse", "elephant");
  addEdgeL(G, "elephant", "lion");
  addEdgeL(G, "lion", "tiger");
  addEdgeL(G, "tiger", "bear");
  addEdgeL(G, "bear", "elephant");
  addEdgeL(G, "elephant", "wolf");
  addEdgeL(G, "wolf", "lion");
  addEdgeL(G, "tiger", "bear");
  addEdgeL(G, "fish", "wolf");
  addEdgeL(G, "cat", "horse");

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

    // check if the vertices are in the graph
  printf("\nVertex %s is in the graph: %s\n", "fox", 
         inGraphL(G, "fox") ? "true" : "false");
  printf("Vertex %s is in the graph: %s\n", "bird",  
         inGraphL(G, "bird") ? "true" : "false");
  printf("Vertex %s is in the graph: %s\n\n", "mouse", 
         inGraphL(G, "mouse") ? "true" : "false");

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