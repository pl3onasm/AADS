/*
  Author: David De Potter
  License: MIT, see LICENSE file in repository root folder
  Description:
    Defines different flavors of vertex structures for a graph.
    To use an augmented vertex structure, compile the program 
    using a compiler flag, like this:

    gcc -O2 -D VERTEX_TYPE1 -o a.out graph.c <other files> -lm
    
    If no flag is set, the default vertex structure is used,
    which only contains a label.
*/


#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

#if defined (VERTEX_TYPE1)

  typedef enum {
    WHITE,
    GRAY,
    BLACK
  } color;

  typedef struct vertex {
    struct vertex *parent;      // the source vertex of an edge
    char *label;                // the label of the vertex
    color clr;                  // color of the vertex
    size_t dTime;               // discovery time
    size_t fTime;               // finish time
    size_t dist;                // distance from the source vertex
  } vertex;

#elif defined (VERTEX_TYPE2)

  typedef struct vertex {
    struct vertex *parent;
    int dist;                   // distance from the source vertex
    char *label;                // the label of the vertex
    bool visited;               // flag to mark the vertex as visited
  } vertex;

#else     // default

  typedef struct vertex {   
    char *label;                // the label of the vertex
  } vertex;

#endif  

#endif    // VERTEX_H_INCLUDED