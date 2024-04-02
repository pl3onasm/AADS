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

typedef enum { WEIGHTED, UNWEIGHTED } weightType; 

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
// DEFAULT EDGE TYPE
#else                          

  typedef struct edge {         
    vertex *to;                 // pointer to the destination vertex
    weightType wType;           // weighted or unweighted 
    double weight;              // weight of the edge
  } edge;


#endif    // EDGE TYPES

#endif    // EDGE_H_INCLUDED