/*
  Author: David De Potter
  License: MIT, see LICENSE file in repository root folder
  Description:
    Defines different flavors of edges for a flow network.
    To use an augmented edge structure, define the desired
    EDGE_TYPE from the command line using a flag, e.g.:

      $ gcc -O2 -D EDGE_TYPE1 ...
    
    If not defined, the default edge structure is used.
*/


#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include "vertex.h"

struct edge {                 // default
  vertex *from;               // pointer to the source vertex
  vertex *to;                 // pointer to the destination vertex
  size_t cap;                 // capacity of the edge
  size_t flow;                // flow through the edge
  bool residual;              // true if the edge is a residual edge
  struct edge *rev;           // pointer to the reverse edge
};


#endif    // EDGE_H_INCLUDED