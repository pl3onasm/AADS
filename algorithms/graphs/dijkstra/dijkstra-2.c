/* file: dijkstra-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Dijkstra's shortest paths algorithm
     using a fibonacci heap
   assumption: nodes are numbered 0..n-1
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct list list;    // forward declaration

typedef struct node {
  // graph fields
  int id, parentG;           // node id and parent id
  double dist;               // distance from source
  list *adj;                 // adjacency list
  
  // fibonacci heap fields
  struct node *parentH;      // parent in the heap
  struct node *child;        // first child in the heap
  struct node *next, *prev;  // next and previous node in the heap
  int degree;                // number of children
  int mark;                  // whether the node has lost a child
  double key;                // key value used to sort the heap
  short inHeap;              // 1 if node is in the heap, 0 otherwise
} node;

struct list {                
  node *n;                   // pointer to a node in the graph
  list *next;                // pointer to next node in the list
  double w;                  // weight of incoming edge at node n
};                           

typedef struct heap {
  int nNodes;                // number of elements in the heap
  node *min;                 // pointer to the minimum node
  node **nodes;              // array of pointers to nodes
} heap;

typedef struct graph {
  int nNodes, nEdges;        // number of nodes and edges 
  node **nodes;              // array of pointers to nodes
} graph;

//::::::::::::::::::::::: memory management :::::::::::::::::::::::://

void *safeCalloc (int n, int size) {
    /* allocates n elements of size size, initializing them to 0, and
       checks whether the allocation was successful */
    void *ptr = calloc(n, size);
    if (ptr == NULL) {
        printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void *safeRealloc (void *ptr, int newSize) {
    /* reallocates memory and checks whether the allocation was successful */
    ptr = realloc(ptr, newSize);
    if (ptr == NULL) {
        printf("Error: realloc(%d) failed. Out of memory?\n", newSize);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

//::::::::::::::::::::::::: list functions ::::::::::::::::::::::::://

list *newList() {
  /* creates an empty list */
  return NULL;
}

void freeList(list *L) {
  /* frees all memory allocated for the list */
  if (L == NULL) return;
  freeList(L->next);
  free(L);
}

list *listInsert (list *L, node *n, double w) {
  /* inserts the node n at the beginning of the list L */
  list *new = safeCalloc(1, sizeof(list));
  new->n = n;
  new->next = L;
  new->w = w;
  return new;
}

//::::::::::::::::::::::::: graph functions :::::::::::::::::::::::://

node *newNode(int id) {
  /* creates a new node with id id */
  node *n = safeCalloc(1, sizeof(node));
  n->id = id;
  n->parentG = -1;
  n->dist = DBL_MAX;
  n->adj = newList();
  return n;
}

graph *newGraph(int nNodes) {
  /* creates a new graph with nNodes nodes */
  graph *G = safeCalloc(1, sizeof(graph));
  G->nNodes = nNodes;
  G->nodes = safeCalloc(nNodes, sizeof(node*));
  for (int i = 0; i < nNodes; i++) 
    G->nodes[i] = newNode(i);
  return G;
}

void buildGraph (graph *G) {
  /* builds the graph G by reading weighted edges from stdin */
  int u, v; double w;
  while (scanf("%d %d %lf", &u, &v, &w) == 3) {
    node *n = G->nodes[u];
    n->adj = listInsert(n->adj, G->nodes[v], w);
    G->nEdges++;
  }
}

void freeGraph(graph *G) {
  /* frees the memory allocated to the graph G */
  for (int i = 0; i < G->nNodes; i++) {
    freeList(G->nodes[i]->adj);
    free(G->nodes[i]);
  }
  free(G->nodes);
  free(G);
}

//:::::::::::::::::::: fibonacci heap functions :::::::::::::::::::://

void cListInsert (node *u, node *v) {
  /* inserts node u before node v in a circular doubly linked list */
  u->next = v;
  u->prev = v->prev;
  v->prev->next = u;
  v->prev = u;
}

void cListRemove (node *u) {
  /* removes node u from a circular doubly linked list */
  u->prev->next = u->next;
  u->next->prev = u->prev;
  u->next = u->prev = u;        // turn u into a circular list
  u->parentH = NULL;            // u becomes an orphan
}

void makeCircularRoot (heap *H, node *u) {
  /* makes u the only node in the root list */
  u->next = u->prev = u;
  H->min = u;
}

void link(heap *H, node *u, node *v) {
  /* removes u from the root list and makes it a child of v */
  cListRemove(u);               // remove u from the root list
  if (v->child == NULL)         // if v's child list is empty
    v->child = u;               // make u the only child of v        
  else 
    cListInsert(u, v->child);   // insert u into the child list of v
  u->parentH = v;               // set u's parent to v
  v->degree++;                  // v has one more child
  u->mark = 0;                  // u is no longer marked
}

void insertNode(heap *H, node *u) {
  /* inserts a node into the heap */
  u->degree = 0;                
  u->mark = 0;
  u->child = NULL;              
  u->parentH = NULL;
  u->key = u->dist;
  if (H->min == NULL)           // if heap is empty
    makeCircularRoot(H, u);     // turn u into a circular root list
  else {
    cListInsert(u, H->min);     // insert u into the root list
    if (u->key < H->min->key)   // update min pointer if necessary
      H->min = u;
  }
  H->nNodes++;                  // H now has one more node
}

heap *newHeap(graph *G) {
  /* creates a heap with n nodes */
  heap *H = safeCalloc(1, sizeof(heap));
  H->nNodes = 0;
  H->min = NULL;
  // insert all graph nodes into the heap
  for (int i = 0; i < G->nNodes; i++)
    insertNode(H, G->nodes[i]);
  return H;
}

void consolidate(heap *H) {
  /* consolidates the heap by linking nodes of equal degree */

  // maxDegree is the upper bound on the degree of any node in H
  int maxDegree = 1 + floor(log(H->nNodes)/log(2));
  
  // A is an auxiliary array of pointers to nodes
  node **A = safeCalloc(maxDegree, sizeof(node*));  
  node *u = H->min, *end = H->min->prev;
  short stop = 0;

  while (!stop && u != u->next) {
    if (u == end) stop = 1;     // only one node left for processing
    node *next = u->next;       // save the next node in the root list
    int d = u->degree;          // d is the number of children of u
    while (A[d] != NULL) {      
      node *v = A[d];           // another node of the same degree as u
      if (u->key > v->key) {    // make sure u is the smaller node
        node *z = u;
        u = v;
        v = z;
      }
      link(H, v, u);            // link v to u
      A[d++] = NULL;            // remove v from the array
    }
    A[d] = u;                   // u is the only node of degree d
    u = next;                   // move to the next node in the root list
  }            
  
  // rebuild the root list from the array A
  H->min = NULL;                // clear the root list
  for (int i = 0; i < maxDegree; i++) {
    if (A[i] != NULL) {         
      node *w = A[i];
      if (H->min == NULL)       // if the root list is empty
        makeCircularRoot(H, w); // turn w into a circular root list
      else {
        cListInsert(w, H->min); // insert w into the root list
        if (w->key < H->min->key) 
          H->min = w;           // update min pointer if necessary
      }
    }
  }
  free(A); 
}

node *extractMin(heap *H) {
  /* removes the node with the minimum key */
  node *z = H->min;
  if (z != NULL) {
    node *u = z->child;
    for (int i = 0; i < z->degree; i++) {
      // add each child of z to the root list
      node *w = u->next;        
      cListRemove(u);           // remove u from the child list
      cListInsert(u, H->min);   // insert u into the root list
      u = w;
    }
    if (z == z->next)           // if z is the only node in the heap
      H->min = NULL;   
    else {
      H->min = z->next;         // otherwise, set H.min to z's sibling
      cListRemove(z);           // remove z from the root list
      if (H->min != H->min->next) 
        consolidate(H);           
    }
    H->nNodes--;                // update the number of nodes 
    z->inHeap = 0;              // z is no longer in the heap
  }
  return z;
}

void cut(heap *H, node *u, node *v){
  /* removes u from the child list of v, 
     and makes it a root node */
  v->degree--;                  // v has one less child
  if (v->child == u)            // update v's child pointer if necessary
    v->child = u->next;         
  if (v->degree == 0)           // if v has no children
    v->child = NULL;            
  cListRemove(u);               // remove u from the child list of v
  cListInsert(u, H->min);       // add u to the root list of H
  u->mark = 0;                  // u is no longer marked
}

void cascadingCut(heap *H, node *u) {
  /* keeps cutting u's parent until u is a root or u is unmarked */
  node *z = u->parentH;
  if (z != NULL) {
    if (!u->mark) u->mark = 1;
    else {
      cut(H, u, z);
      cascadingCut(H, z);
    }
  }
}

void decreaseKey(heap *H, node *u, double newKey) {
  /* decreases the key of the node to newKey */
  if (newKey > u->key) {
    printf("Error: new key is greater than current key\n");
    exit(EXIT_FAILURE);
  }
  u->key = newKey;              
  node *v = u->parentH;
  if (v != NULL && u->key < v->key) { 
    // if u is not a root and its key is less than 
    // its parent's key, cut u from its parent  
    cut(H, u, v);
    cascadingCut(H, v);
  }
  if (u->key < H->min->key) H->min = u;
}

void freeHeap(heap *H) {
  /* frees the memory allocated to the heap */
  free(H);
}

//::::::::::::::::::::::: dijkstra functions ::::::::::::::::::::::://

short relax(node *u, node *v, double w) {
  /* relaxes the edge (u,v) */
  if (v->dist > u->dist + w) {
    v->dist = u->dist + w;
    v->parentG = u->id;
    return 1;
  }
  return 0;
}

void dijkstra(graph *G, int s) {
  /* computes the shortest paths from node s to all other nodes */
  G->nodes[s]->dist = 0;   // set the distance of the source to 0
  heap *H = newHeap(G);

  while (H->nNodes > 0) {
    node *u = extractMin(H);

    for (list *a = u->adj; a; a = a->next){
      node *v = a->n;
      if (relax(u, v, a->w) && v->inHeap)
        decreaseKey(H, v, v->dist);
    }
  }
  freeHeap(H);
}

void print(graph *G, int s) {
  /* prints the results of the shortest paths computation */
  printf("Node   Distance   Parent\n"); 
  for (int i = 0; i < G->nNodes; i++) {
    node *n = G->nodes[i];
    if (n->id == s) printf("%4s", "src");
    else printf("%4d", n->id);
    if (n->dist == DBL_MAX)
      printf("%11s", "inf");
    else printf("%11.2lf", n->dist);
    if (n->parentG == -1)
      printf("%9c\n", '-');
    else if (n->parentG == s)
      printf("%9s\n", "src");
    else printf("%9d\n", n->parentG);
  }
}

//::::::::::::::::::::::::::::: main ::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  int n, s;                    // n = number of nodes
  scanf("%d %d", &n, &s);      // s = source node 

  graph *G = newGraph(n); 
  buildGraph(G);               // read edges from stdin

  dijkstra(G, s);              // compute shortest paths from node s
  print(G, s);                 // print results

  freeGraph(G);
  return 0;
}
