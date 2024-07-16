/* 
  file: knapsack-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: binary knapsack problem
    top-down dp approach with memoization
  time complexity: O(nW)
*/ 
 
#include "../../../lib/clib/clib.h"
#include <stdint.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))

//===================================================================
// Definition of item structure
typedef struct {
  size_t weight;
  double value;
} Item;

//===================================================================
// Reads items from stdin
Item *readItems(size_t *len) {
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
// Shows the items that are included in the knapsack along with
// the total value and weight
void showItems(Item *items, size_t len, size_t W, double **memo, 
               double maxVal) {
  
  printf("Items included:\n");
  size_t w = W, totalWeight = 0;

  for (size_t i = len; --i; ) {
    if (memo[i][w] != memo[i - 1][w] ) {
      printf("  (%zu,%.2lf)\n", items[i - 1].weight, 
                                items[i - 1].value);
      w -= items[i - 1].weight;
      totalWeight += items[i - 1].weight;
    }
  }

  printf("\nTotal value: € %.2lf\n"
         "Total weight: %zu kg\n", maxVal, totalWeight);
}

//===================================================================
// Computes the maximum value that can be put in a knapsack of
// capacity W, given len items with weights and values
// using top-down dynamic programming with memoization
double fillKnapsack(Item *items, size_t n, size_t W, double **memo) {
  
    // if the value is already memoized, return it
  if (memo[n][W] != SIZE_MAX) return memo[n][W];

    // base case: no more items or no more capacity
  if (n == 0 || W == 0) return 0;
  
    // if the current item still fits in the knapsack, compute the
    // maximum value obtained by either including it or excluding it
  if (items[n - 1].weight <= W) 
    memo[n][W] = 
      MAX(items[n - 1].value + 
          fillKnapsack(items, n - 1, W - items[n - 1].weight, memo),
          fillKnapsack(items, n - 1, W, memo));
  else
      // skip the current item if it doesn't fit
    memo[n][W] = fillKnapsack(items, n - 1, W, memo);
  
  return memo[n][W];
}

//===================================================================

int main () {

    // read knapsack capacity
  size_t W;
  assert(scanf("%zu ", &W) == 1);

    // read items
  size_t len = 0;
  Item *items = readItems(&len);

    // create memoization table
  CREATE_MATRIX(double, memo, len + 1, W + 1, SIZE_MAX);
  
  double maxVal = fillKnapsack(items, len, W, memo);

  showItems(items, len + 1, W, memo, maxVal);

  free(items);
  FREE_MATRIX(memo, len + 1);
  
  return 0;
}



