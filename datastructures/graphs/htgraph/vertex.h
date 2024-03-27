/*
  Author: David De Potter
  License: MIT, see LICENSE file in repository root folder
  Description:
    Defines different flavors of a vertex for a graph.
    To use an augmented vertex structure, compile the program 
    using a compiler flag, like this:

    gcc -O2 -D VERTEX_TYPE1 ...
    
    If no flag is set, the default vertex structure is used,
    which only contains a label.
*/


#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>


//===================================================================
// VERTEX TYPE 1  (application example: DFS)
#if defined (VERTEX_TYPE1)      // usage example: DFS

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
    size_t dist;              // distance from the source
  } vertex;

//===================================================================
// VERTEX TYPE 2  (application example: BFS)
#elif defined (VERTEX_TYPE2)

  typedef struct vertex {
    struct vertex *parent;    // pointer to the parent vertex
    int dist;                 // distance from the source vertex
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