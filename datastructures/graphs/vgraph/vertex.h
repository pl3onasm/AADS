/*
  Author: David De Potter
  License: MIT, see LICENSE file in repository root folder
  Description:
    Defines different flavors of a vertex for a graph.
    To use an augmented vertex structure, define the desired
    VERTEX_TYPE from the command line using a flag, e.g.:

    $ gcc -O2 -D VERTEX_TYPE1 ...
    
    If no flag is set, the default vertex structure is used,
    which only contains a label.
*/


#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>


//===================================================================
// VERTEX TYPE 1  (application example: DFS)
#if defined (VERTEX_TYPE1)      

  typedef enum {
    WHITE,
    GRAY,
    BLACK
  } clr;

  typedef struct vertex {
    struct vertex *parent;    // ptr to the parent vertex
    char *label;              // the label of the vertex
    clr color;                // color of the vertex
    size_t dTime;             // discovery time
    size_t fTime;             // finish time
  } vertex;

//===================================================================
// VERTEX TYPE 2  (application example: BFS, Dijkstra, Prim)
#elif defined (VERTEX_TYPE2)

  typedef struct vertex {
    struct vertex *parent;    // pointer to the parent vertex
    double dist;              // distance from the source vertex
    char *label;              // the label of the vertex
  } vertex;

//===================================================================
// VERTEX TYPE 3  (application example: topological sort, SCC)
#elif defined (VERTEX_TYPE3)

  typedef struct vertex {
    char *label;              // the label of the vertex
    bool visited;             // marks the vertex as visited
  } vertex;

//===================================================================
// DEFAULT VERTEX TYPE
#else     

  typedef struct vertex {   
    char *label;              // the label of the vertex
  } vertex;


#endif    // VERTEX TYPES

#endif    // VERTEX_H_INCLUDED