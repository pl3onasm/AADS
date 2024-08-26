/* 
  file: knapsack-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: binary knapsack problem
    naive recursive implementation
  time complexity: O(2^n)
*/ 
 
#include "../../../lib/clib.h"

//===================================================================
// Definition of item structure
typedef struct {
  size_t weight;
  double value;
} Item;

//===================================================================
// Reads items from stdin
Item *readItems (size_t *len) {
  size_t cap = 100;
  Item *items = safeCalloc(cap, sizeof(Item));
  while (scanf(" ( %zu , %lf ) , ", &items[*len].weight, 
                                    &items[*len].value) == 2) {
    if (++*len == cap) {
      cap *= 2;
      items = safeRealloc(items, cap * sizeof(Item));
    }
  }

  items = safeRealloc(items, *len * sizeof(Item));
  return items;
}

//===================================================================
// Naive recursive implementation of the binary knapsack problem;
// computes the maximum value that can be put in a knapsack of
// capacity W, given len items and their weights and values
double fillKnapsack (Item *items, size_t len, size_t W) {

    // base case: no more items or no more capacity
  if (len == 0 || W == 0) return 0;
  
    // if the current item fits in the knapsack, we compute the
    // maximum value obtained by either including it or excluding it
  if (items[len - 1].weight <= W) 
    return 
      MAX(items[len - 1].value + 
          fillKnapsack(items, len - 1, W - items[len - 1].weight),
          fillKnapsack(items, len - 1, W));
  
    // skip the current item if it doesn't fit
  return fillKnapsack(items, len - 1, W);
}

//===================================================================

int main () {

    // read knapsack capacity
  size_t W;
  assert(scanf("%zu ", &W) == 1);

    // read items
  size_t len = 0;
  Item *items = readItems(&len);
  
  printf("Max value: %.2lf\n", fillKnapsack(items, len, W));

  free(items);
  
  return 0;
}
