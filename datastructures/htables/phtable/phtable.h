/* 
  Generic perfect hash table implementation
  Description: this is a generic perfect hash table implementation. 
    Perfect hash tables are ideal for situations where the key set 
    is known in advance and is static. It is handy for situations
    in which you want to avoid the overhead of a hash table with
    chaining or open addressing, and just want to have a simple
    lookup table, where each lookup is guaranteed to be O(1).
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef _PHTABLE_H_INCLUDED_
#define _PHTABLE_H_INCLUDED_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// The perfect hash table structure
typedef struct phtable_t {
  size_t size;
  size_t nKeys;
  size_t (*hash1)(void*);
  size_t (*hash2)(void*);
  void** keys;
  void** values;
} phtable_t;






#endif // _PHTABLE_H_INCLUDED_