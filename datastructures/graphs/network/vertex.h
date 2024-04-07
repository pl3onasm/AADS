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
#include "edge.h"

typedef struct edge edge;

//===================================================================
// VERTEX TYPE 1  (application example: Ford-Fulkerson, Edmonds-Karp)
#if defined (VERTEX_TYPE1)

  typedef struct vertex {
    char *label;              // the label of the vertex
    edge *pathEdge;           // edge leading to the vertex
    size_t inDegree;          // in-degree of the vertex
  } vertex;

//===================================================================
// VERTEX TYPE 2  (application example: Ford-Fulkerson, DFS)

#elif defined (VERTEX_TYPE2)

  typedef struct vertex {
    char *label;              // the label of the vertex
    bool visited;             // true if the vertex has been visited
    size_t inDegree;          // in-degree of the vertex
  } vertex;

//===================================================================
// DEFAULT VERTEX TYPE
#else     

  typedef struct vertex {   
    char *label;              // the label of the vertex
    size_t inDegree;          // in-degree of the vertex
  } vertex;


#endif    // VERTEX TYPES

#endif    // VERTEX_H_INCLUDED