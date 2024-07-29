/* 
  file: med-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: minimum edit distance
    using a naive recursive approach
  time complexity: exponential
  caution: may explode for large strings n > 20
*/

#include "../../../lib/clib/clib.h"
#include <stdint.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b))
typedef enum { COPY, INSERT, DELETE, REPLACE, SWAP, KILL } op;

//===================================================================
// Naive recursive approach to compute the minimum edit distance
size_t computeMED(string *src, string *tgt, size_t i, size_t j, 
                  size_t *costs) { 
  
    // source is empty: insert remaining target chars
  if (i == 0) return j * costs[INSERT];
 
  size_t med, minCost = SIZE_MAX;

    // check if killing last remaining chars is cheaper
  if (i == strLen(src) && j == strLen(tgt))
    for (size_t k = 0; k < strLen(src); ++k) {
      med = computeMED(src, tgt, k, j, costs);
      minCost = MIN(minCost, med + costs[KILL]);
    }

    // target is empty: choose between deleting or killing
    // all remaining source chars
  if (j == 0) {
    if (i == strLen(src) && i * costs[DELETE] > costs[KILL])
      return costs[KILL];
    return i * costs[DELETE];
  }

  if (charAt(src, i - 1) == charAt(tgt, j - 1)) {
    med = computeMED(src, tgt, i - 1, j - 1, costs);
    minCost = MIN(minCost, med + costs[COPY]);
  }

  if (i > 1 && j > 1 && charAt(src, i - 1) == charAt(tgt, j - 2) 
      && charAt(src, i - 2) == charAt(tgt, j - 1)) {
    med = computeMED(src, tgt, i - 2, j - 2, costs);
    minCost = MIN(minCost, med + costs[SWAP]);
  }

  med = computeMED(src, tgt, i, j - 1, costs);
  minCost = MIN(minCost, med + costs[INSERT]);

  med = computeMED(src, tgt, i - 1, j, costs);
  minCost = MIN(minCost, med + costs[DELETE]);

  if (charAt(src, i - 1) != charAt(tgt, j - 1)) {
    med = computeMED(src, tgt, i - 1, j - 1, costs);
    minCost = MIN(minCost, med + costs[REPLACE]);
  }

  return minCost;
}

//===================================================================

int main () {
  
    // read source and target strings
  READ_STRING(src, '\n');
  READ_STRING(tgt, '\n');

    // read operation costs 
  size_t costs[6];
  READ_ARRAY(costs, "%zu", 6);

  printf("Minimum edit distance: %zu\n", 
    computeMED(src, tgt, strLen(src), strLen(tgt), costs));
  
  freeString(src);
  freeString(tgt);
  return 0;
}