/*
  Author: David De Potter
  License: MIT, see LICENSE file in repository root folder
  Description:
    Defines different flavors of edges for a graph.
    To use an augmented edge structure, define the desired
    EDGE_TYPE from the command line using a flag, e.g.:

    $ gcc -O2 -D EDGE_TYPE1 ...
    
    If not defined, the default edge structure is used, which
    only contains a pointer to the destination vertex and the
    weight of the edge.
*/


#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include "vertex.h"

//===================================================================
// EDGE TYPE 1  (application example: DFS)
#if defined (EDGE_TYPE1)        

  typedef enum {
    TREE =    'T',
    BACK =    'B',    
    FORWARD = 'F',
    CROSS =   'C'
  } eType;

  typedef struct edge {         
    vertex *to;                 // pointer to the destination vertex
    double weight;              // weight of the edge
    eType type;                 // type of the edge
  } edge;

//===================================================================
// EDGE TYPE 2  (application example: Kruskal)
#elif defined (EDGE_TYPE2)      

  typedef struct edge {        
    vertex *from;               // pointer to the source vertex 
    vertex *to;                 // pointer to the destination vertex
    double weight;              // weight of the edge
    bool reversed;              // if true: edge was added in reverse
                                // for undirected graphs
  } edge;

//===================================================================
// EDGE TYPE 3  (application example: Hierholzer, Fleury)
#elif defined (EDGE_TYPE3)      

  typedef struct edge {         
    vertex *from;               // pointer to the source vertex
    vertex *to;                 // pointer to the destination vertex
    struct edge *rev;           // pointer to the reverse edge
    double weight;              // weight of the edge
    bool taken;                 // if true: edge is part of the tour
  } edge;

//===================================================================
// DEFAULT EDGE TYPE
#else                          

  typedef struct edge {         
    vertex *to;                 // pointer to the destination vertex
    double weight;              // weight of the edge
  } edge;


#endif    // EDGE TYPES

#endif    // EDGE_H_INCLUDED