/* 
  file: hungarian.c  
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: weighted MCBM using the Hungarian algorithm
  time complexity: O(V³), using the optimization as described
    in problem 25-2 of CLRS4 (implemented as slack attribute), 
    which reduces the time complexity from O(V⁴) to O(V³)
  note: make sure to use VERTEX_TYPE8 in the vertex.h file  
    by defining it from the command line using
      $ gcc -D VERTEX_TYPE8 ...
*/

#define VERTEX_TYPE8
#include "../../../datastructures/graphs/graph/graph.h" 
#include "../../../datastructures/queues/queue.h"
#include <assert.h>
#include <float.h>
#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

//===================================================================
// Show function for the queue
void showMember(void *item) {
  vertex *v = (vertex *) item;
  printf("%s", v->label);
}


//===================================================================
// Returns true if the graph is bipartite (two-colorable), i.e. if it 
// is possible to partition the vertices into two sets such that no 
// two adjacent vertices belong to the same set. This is done using a 
// BFS traversal and assigning a type to each vertex (LEFT or RIGHT)
bool isBipartite(graph *G) {
  vertex *v = firstV(G);
  v->type = LEFT;
  queue *q = newQueue(nVertices(G)); 
  enqueue(q, v); 

  while (!isEmptyQueue(q)) {
    vertex *u = dequeue(q);
    dll *edges = getNeighbors(G, u); 
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
      if (e->to->type == NIL) {
        e->to->type = u->type == LEFT ? RIGHT : LEFT;
        enqueue(q, e->to);            
      } else if (e->to->type == u->type) {
        freeQueue(q);
        return false;                
      }
    }
  }
  freeQueue(q);
  return true; 
}

//===================================================================
// Creates and initializes a queue with all unmatched vertices 
// in the left set 
queue *initBFS(graph *G) {
  queue *Q = newQueue(nVertices(G));
  
  for (vertex *u = firstV(G); u; u = nextV(G)) {
    if (u->type == RIGHT) {
      dll *edges = getNeighbors(G, u);
      // determine the minimum slack of the edges incident to u
      // and set the minParent to the vertex that minimizes the slack
      u->slack = DBL_MAX;
      for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
        if (e->to->match == NULL) {
          double slack = u->height + e->to->height - e->weight;
          if (slack < u->slack) {
            u->slack = slack;
            u->minParent = e->to;
            printf("u: %s, minParent: %s, slack: %f\n", u->label, u->minParent->label, u->slack);
          }
        }
      }
      u->parent = NULL;
    } else {
      if (u->match == NULL) {
        u->parent = u;    
        enqueue(Q, u);
      } else {
        u->parent = NULL;
      }
    }
  }
  return Q;
}

//===================================================================
// Prints the matching
void printMatching(graph *G) {
  double cost = 0;
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    cost += v->height;
    if (v->type == LEFT) {
      printf("%s -> %s\n", v->label, v->match->label);
    }
  }
  printf("Total cost: %.2lf\n", cost);
}

//===================================================================
// Initializes the heights of the vertices: a vertex in the left set 
// has height equaling the maximum weight of the edges incident to 
// it; a vertex in the right set has height 0 and sigma set to INF 
void initHeights(graph *G) {
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    if (v->type == LEFT) {
      dll *edges = getNeighbors(G, v);
      v->height = -DBL_MAX;
      for (edge *e = dllFirst(edges); e; e = dllNext(edges)) 
        v->height = MAX(v->height, e->weight);
    } else {
      v->height = 0;
      v->slack = DBL_MAX;
    }
  }
  printf("Initial Heights: ");
  for (vertex *v = firstV(G); v; v = nextV(G)) {
    printf("%s: %.2lf ", v->label, v->height);
  }
  printf("\n");
}

//===================================================================
// Updates the matching
void updateMatching(vertex *u, vertex *v) {
  while (true) {
    u->match = v;
    v->match = u;
    printf("Updating Matching: %s -> %s\n", u->label, v->label);
    if (u->parent == u) break;
    v = u->parent;
    u = v->parent;
  } 
}

//===================================================================
// Runs BFS to find an augmenting path from the left set to the right
// set; relabels if no path is found
void bfs(graph *G) {
  printf("\n\nRunning BFS\n");
  queue *Q = initBFS(G);
  setQueueShow(Q, showMember);
  printf("New Queue: ");
  showQueue(Q);

  while (true) {
    if (isEmptyQueue(Q)) {
      // relabel 
      printf("Relabeling\n");
      double delta = DBL_MAX;
      for (vertex *u = firstV(G); u; u = nextV(G)) 
        if (u->type == RIGHT && u->parent == NULL && u->minParent->parent != NULL)
          delta = MIN(delta, u->slack);   // minParent should be in the forest and
                                          // u should not be in the forest / undiscovered
      printf("Delta: %f\n", delta);
      for (vertex *u = firstV(G); u; u = nextV(G)) {
        if (u->type == LEFT && u->parent)
          u->height -= delta;
        if (u->type == RIGHT) {
          if (u->parent) 
            u->height += delta;
          else 
            u->slack -= delta;
        }
      } 
      printf("Updated Heights: ");
      for (vertex *u = firstV(G); u; u = nextV(G)) {
        printf("%s: %.2lf ", u->label, u->height);
      }
      printf("\n");

      for (vertex *v = firstV(G); v; v = nextV(G)) {
        if (v->type == RIGHT && v->slack == 0 ) {
          //v->slack = DBL_MAX; // reset slack
          vertex *u = v->minParent;
          if (! v->match) {
            printf("Augmenting path found after relabeling\n");
            updateMatching(u, v);
            printf("Updated Matching: \n");
            printMatching(G);
            freeQueue(Q);
            return;
          }
          enqueue(Q, u);
        }
      }
    }

    vertex *u = dequeue(Q);
    printf("Dequeued: %s\n", u->label);

    dll *edges = getNeighbors(G, u);
    
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
      vertex *v = e->to;
      printf ("Neighbor v: %s, type: %d, slack: %f\n", v->label, v->type, v->slack);
      printf ("e->weight: %f, u->height: %f, v->height: %f, equality: %f\n", e->weight, u->height, v->height, u->height + v->height);
      printf (e->weight == u->height + v->height ? "In equality subgraph\n" : "Not in equality subgraph\n");
        // if the edge belongs to the equality subgraph and the
        // vertex is unmatched, we have found an M-augmenting path
      if (v->parent == NULL && ((v->type == LEFT && u->match == v) ||
          (v->type == RIGHT && u->height + v->height - e->weight < 1e-6))) {
        // < 1e-6 is used instead of == 0 to avoid floating point errors
        
        // if v-type == LEFT, then we need to update the slack of the
        // vertices in the right set since v is added to the forest
        if (v->type == LEFT) {
          dll *es = getNeighbors(G, v);
          for (edge *w = dllFirst(es); w; w = dllNext(es)) {
            if ( w->to->parent == NULL) {
              printf("Initial slack: %f\n", w->to->slack);  
              double slack = w->to->height + v->height - w->weight;
              if (slack < w->to->slack) {
                w->to->slack = slack;
                w->to->minParent = v;
                printf("Update w: %s, minParent: %s, slack: %f\n", w->to->label, w->to->minParent->label, w->to->slack);
              }
            }
          }
        }
        printf("u: %s, v: %s\n", u->label, v->label);
        v->parent = u;
        if (v->type == RIGHT && v->match == NULL) {	
          printf("Augmenting path found\n");
          updateMatching(u, v);
          printf("Updated Matching:\n ");
          printMatching(G);
          freeQueue(Q);
          return;
        }
        enqueue(Q, v);
        printf("Updated Queue: ");
        showQueue(Q);
      }
    }
  }
  freeQueue(Q);
  return;
}

//===================================================================
// Initializes the matching with a greedy algorithm: for each
// unmatched vertex in the left set, it finds the first adjacent
// unmatched vertex in the right set and matches them
size_t initMatching(graph *G) {
  size_t n = 0;
  for (vertex *u = firstV(G); u; u = nextV(G)) {
    if (u->type == RIGHT) continue;
    dll *edges = getNeighbors(G, u);
    for (edge *e = dllFirst(edges); e; e = dllNext(edges)) {
      if (e->weight == u->height + e->to->height 
          && e->to->match == NULL) {
        n++;
        u->match = e->to;
        e->to->match = u;
        printf("Init Matched: %s -> %s\n", u->label, e->to->label);
        break;
      }
    }
  }
  return n;
}

//===================================================================
// Runs the Hungarian algorithm to find the maximum cardinality
// bipartite matching in a weighted graph
void hungarian(graph *G) {

  initHeights(G);
  size_t n = initMatching(G);
  printf("Initial matching: %lu\n", n);

  // while the matching is not perfect
  while (n < nVertices(G) / 2) {
    bfs(G);
    n++;
  }
}



//===================================================================
// Reads the cost type from stdin: either MIN or MAX
int readCostType() {
  char type[20];
  assert(scanf("%s ", type) == 1);
  if (strcmp(type, "MIN") == 0) return 1;
  else if (strcmp(type, "MAX") == 0) return -1;
  else {
    fprintf(stderr, "Invalid cost type: %s\n"
                    "Valid types are: MIN, MAX\n", type);
    exit(EXIT_FAILURE);
  }
}

//===================================================================
// Main function

int main() {
  int cType = readCostType(); 

  graph *G = newGraph(50, WEIGHTED);
  setUndirected(G);

  readGraph(G);
  showGraph(G);

  if (!isBipartite(G)) {
    printf("Graph is not bipartite\n");
    freeGraph(G);
    return 0;
  }

  if (nEdges(G) != nVertices(G) * nVertices(G) / 4) {
    printf("The graph is not complete\n");
    freeGraph(G);
    return 0;
  }

  hungarian(G);

  printf("\nFinal matching:\n");
  printMatching(G);

  freeGraph(G);
  return 0;
}