/* 
  file: knapsack-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: 0-1 knapsack problem
    bottom-up DP approach
  time complexity: O(nW)
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
// Shows the items that are included in the knapsack along with
// the total value and weight
void showItems (Item *items, size_t len, size_t W, double **memo) {
  
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
         "Total weight: %zu kg\n", memo[len - 1][W], totalWeight);
}

//===================================================================
// Computes the maximum value that can be put in a knapsack of
// capacity W, given len items and their weights and values
void fillKnapsack (Item *items, size_t len, size_t W, 
                   double **memo) {

    // work bottom-up by gradually increasing the number of items,
    // i.e. the prefix length of the items array
  for (size_t i = 1; i <= len; i++) 
    for (size_t w = 1; w <= W; w++) 
      if (items[i - 1].weight <= w) 
        memo[i][w] = MAX(memo[i - 1][w], 
                         memo[i - 1][w - items[i - 1].weight] 
                         + items[i - 1].value);
      else 
        memo[i][w] = memo[i - 1][w];
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
  CREATE_MATRIX(double, memo, len + 1, W + 1, 0);

  fillKnapsack(items, len, W, memo);

  showItems(items, len + 1, W, memo);

  free(items);
  FREE_MATRIX(memo, len + 1);
  
  return 0;
}