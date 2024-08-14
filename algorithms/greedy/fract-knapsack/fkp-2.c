/* 
  file: fpk-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: fractional knapsack problem (FKP) 
  time complexity: O(n), where n is the number of items
*/

#include "../../../lib/clib/clib.h"
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
void showItem(Item *item) {
  printf("  item %d: %.2lf kg, %.2lf €/kg\n", 
         item->id, item->weight, item->unitValue);
}

//===================================================================
// Partitions an array around a random pivot element and returns
// the sorted position of this pivot
size_t partition (Item *items, size_t left, size_t right, 
                  size_t *nEq, double *wGr, double *wEq) {
  
  size_t l = left;
    // put a random pivot at the end of the array
  SWAP(items[left + rand() % (right - left)], items[right - 1]);
  *wEq = items[right - 1].weight;

    // keep swapping elements with greater unit value than 
    // the pivot to the left of the array, and elements with 
    // equal unit value to the right of the pivot
  for (size_t j = left; j < right - 1 - *nEq; j++) {
    if (items[j].unitValue == items[right - 1].unitValue) {
      ++*nEq;
      *wEq += items[j].weight;
      SWAP(items[j], items[right - 1 - *nEq]);
    }
    if (items[j].unitValue > items[right - 1].unitValue) {
      *wGr += items[j].weight;
      SWAP(items[l++], items[j]);
    }
  }
    // put pivot and elements with equal unit value 
    // in their final positions
  for (size_t j = 0; j <= *nEq; j++) 
    SWAP(items[l + j], items[right - j - 1]);
  
  return l;
}

//===================================================================
// Selects the most valuable items that fit in the knapsack 
void selectItems(Item *items, size_t left, size_t right, double W) {

    // partition the items around a random pivot element, such that
    // the items with greater unit value are to the left of the pivot
  size_t nEq = 0; double wGr = 0, wEq = 0;
  size_t pvtIdx = partition(items, left, right, &nEq, &wGr, &wEq);

  if (wGr > W) 
      // too heavy, take no items and recurse on the left part
    selectItems(items, left, pvtIdx, W);
  else if (wGr + wEq < W) 
      // too light, take all items from the left part 
      // and recurse on the right part
    selectItems(items, pvtIdx + nEq + 1, right, W - wGr - wEq);
}

//===================================================================
// Fills the knapsack with the most valuable items that fit and 
// shows the selected items, the total weight and the total value
void fillKnapsack(Item *items, size_t len, double W) {
  double totalValue = 0, totalWeight = 0;
  size_t idx = 0;

  selectItems(items, 0, len, W);

  printf("Items selected for 100 %%:\n");
  while (totalWeight + items[idx].weight <= W) {
    totalWeight += items[idx].weight; 
    totalValue += items[idx].value;
    showItem(&items[idx++]);
  }

  if (idx == 0) printf("  None\n");

  if (totalWeight < W) {
    double rem = W - totalWeight;
    printf("\nSelected for %.2lf %%: \n", 
           100 * rem / items[idx].weight);  
    showItem(&items[idx]);
    totalValue += rem * items[idx].unitValue;
    totalWeight += rem;
  }
  
  printf("\nTotal weight: %.2lf\nTotal value: %.2lf\n", 
          totalWeight, totalValue);

    // show remaining items
  printf("\nRemaining items:\n");
  if (idx >= len - 1) printf("  None\n");
  for (size_t i = idx + 1; i < len; i++) 
    showItem(&items[i]);
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
    printf("Total weight: %.2lf\nTotal value: %.2lf\n", 
            totalWeight, totalValue);
  
  } else fillKnapsack(items, nItems, W);

  free(items);

  return 0;
}