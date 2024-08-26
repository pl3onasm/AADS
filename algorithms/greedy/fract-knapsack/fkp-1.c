/* 
  file: fkp-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: fractional knapsack problem (FKP)
  time complexity: O(nlogn), where n is the number of items
*/ 

#include "../../../lib/clib.h"
#include <time.h>

//===================================================================
// Definition of the item structure
typedef struct {
  int id;             // id based on order of input
  double weight;      // total weight of the item
  double value;       // total value of the item
  double unitValue;   // value per unit of weight 
} Item;

//===================================================================
// Shows the id, weight and unit value of an item
void showItem(Item item) {
  printf("  item %d: %.2lf kg, € %.2lf / kg\n", 
         item.id, item.weight, item.unitValue);
}

//===================================================================
// Compares two items based on their unit value
int cmpItems(void const *a, void const *b) {
  return ((Item *)b)->unitValue < ((Item *)a)->unitValue ? -1 : 1;
}

//===================================================================
// Fills the knapsack with the most valuable items that fit and 
// shows the selected items, the total weight and the total value
void fillKnapsack(Item *items, size_t len, double W) {
  double totalValue = 0, totalWeight = 0;
  size_t idx = 0;

  qsort(items, len, sizeof(Item), cmpItems);

  printf("Items selected for 100 %%:\n");
  while (totalWeight + items[idx].weight <= W) {
    totalWeight += items[idx].weight; 
    totalValue += items[idx].value;
    showItem(items[idx++]);
  }

  if (idx == 0) printf("  None\n");

  if (totalWeight < W) {
    double rem = W - totalWeight;
    printf("\nSelected for %.2lf %%: \n", 
           100 * rem / items[idx].weight);  
    showItem(items[idx]);
    totalValue += rem * items[idx].unitValue;
    totalWeight += rem;
  }
  
  printf("\nTotal weight: %.2lf kg\nTotal value: € %.2lf\n",
          totalWeight, totalValue);

    // show remaining items
  printf("\nRemaining items:\n");
  if (idx >= len - 1) printf("  None\n");
  for (size_t i = idx + 1; i < len; i++) 
    showItem(items[i]);
}

//===================================================================
// Reads the input and returns the total weight of the items
double readInput (Item **items, size_t *nItems) {

  double value, weight, totalWeight = 0;
  size_t i = 0, cap = 100;
  *items = safeCalloc(cap, sizeof(Item));
  while (scanf("%lf %lf", &weight, &value) == 2) {
    (*items)[i].id = i + 1;
    (*items)[i].value = value;
    (*items)[i].weight = weight;
    (*items)[i].unitValue = value / weight;
    totalWeight += weight;
    if (++i == cap){
      cap *= 2;
      *items = safeRealloc(*items, cap * sizeof(Item));
    }
  }
  *nItems = i;
  return totalWeight;
}

//===================================================================

int main () {
  srand(time(NULL)); 
  
  double W;
  assert(scanf("%lf", &W) == 1);
  
  size_t nItems; Item *items;
  double totalWeight = readInput(&items, &nItems);

  if (totalWeight <= W) {
    printf(nItems == 0 ? "No" : "All");
    printf(" items selected\n");
    double totalValue = 0, totalWeight = 0;
    for (size_t i = 0; i < nItems; i++) {
      totalValue += items[i].value;
      totalWeight += items[i].weight;
    }
    printf("Total weight: %.2lf kg\nTotal value: € %.2lf\n",
            totalWeight, totalValue);
  
  } else fillKnapsack(items, nItems, W);

  free(items);

  return 0;
}