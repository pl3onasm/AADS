/* 
  file: med-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: minimum edit distance
    using top-down DP with memoization and path reconstruction
*/

#include "../../../lib/clib/clib.h"
#include <stdint.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b))
typedef enum { COPY, INSERT, DELETE, REPLACE, SWAP, KILL } op;

//===================================================================
// Updates the minimum value and operation if a new minimum is found
void updateMin(size_t **dp, op **ops, size_t i, size_t j, 
               size_t oldCost, op newOp, size_t *costs) {

  if (dp[i][j] > oldCost + costs[newOp]) {
    dp[i][j] = oldCost + costs[newOp];
    ops[i][j] = newOp;
  }
}

//===================================================================
// Top-down DP approach to compute the minimum edit distance
// between source and target strings using memoization
size_t computeMED(string *src, string *tgt, size_t i, size_t j, 
    size_t *costs, size_t **dp, op **ops, size_t *kill) {

    // return the value if it has already been computed
  if (dp[i][j] != SIZE_MAX) return dp[i][j];

  size_t med;
  if (i && i == strLen(src) && j == strLen(tgt)) {
    for (size_t k = 0; k < strLen(src); ++k) {
      med = computeMED(src, tgt, k, j, costs, dp, ops, kill);
      updateMin(dp, ops, i, j, med, KILL, costs);
      if (dp[i][j] == dp[k][j] + costs[KILL]) *kill = k;
    }
  }

    // base case: source is empty
  if (i == 0) {
    ops[i][j] = INSERT;
    return dp[i][j] = j * costs[INSERT];
  }

    // base case: target is empty
  if (j == 0) {
    ops[i][j] = DELETE;
    return dp[i][j] = i * costs[DELETE];
  }

  if (charAt(src, i - 1) == charAt(tgt, j - 1)){
    med = computeMED(src, tgt, i - 1, j - 1, costs, dp, ops, kill);
    updateMin(dp, ops, i, j, med, COPY, costs);
  }

  if (i > 1 && j > 1 && charAt(src, i - 1) == charAt(tgt, j - 2) 
      && charAt(src, i - 2) == charAt(tgt, j - 1)) {
    med = computeMED(src, tgt, i - 2, j - 2, costs, dp, ops, kill);
    updateMin(dp, ops, i, j, med, SWAP, costs);
  }

  med = computeMED(src, tgt, i, j - 1, costs, dp, ops, kill);
  updateMin(dp, ops, i, j, med, INSERT, costs);

  med = computeMED(src, tgt, i - 1, j, costs, dp, ops, kill);
  updateMin(dp, ops, i, j, med, DELETE, costs);

  if (charAt(src, i - 1) != charAt(tgt, j - 1)) {
    med = computeMED(src, tgt, i - 1, j - 1, costs, dp, ops, kill);
    updateMin(dp, ops, i, j, med, REPLACE, costs);
  }

  return dp[i][j];
}

//===================================================================
// Shows the current state of the string being edited
void showEdit(string *str, string *src, unsigned char ch,
              size_t pos) {
  
  if (ch) appendChar(str, ch);
  if (strLen(str)) showSubstring(str, 0, strLen(str) - 1, '\0');
  printf(" | ");
  if (strLen(src)) showSubstring(src, pos, strLen(src) - 1, '\0');
  printf("\n");
}

//===================================================================
// Reconstructs the optimal operation sequence to transform source 
// into target string while showing the intermediate steps
void reconstructPath(string *src, string *tgt, size_t i, size_t j, 
                     op **ops, size_t kill, string *str) {

  if (i == 0 && j == 0) {
    printf("Source:\n  ");
    showString(src);
    return;
  }

  switch (ops[i][j]) {
    case COPY:
      reconstructPath(src, tgt, i - 1, j - 1, ops, kill, str);
      printf("Copy %c:\n  ", charAt(src, i - 1)), 
      showEdit(str, src, charAt(src, i - 1), i);
      break;
    case INSERT:
      reconstructPath(src, tgt, i, j - 1, ops, kill, str);
      printf("Insert %c:\n  ", charAt(tgt, j - 1));
      showEdit(str, src, charAt(tgt, j - 1), i);
      break;
    case DELETE:
      reconstructPath(src, tgt, i - 1, j, ops, kill, str);
      printf("Delete %c:\n  ", charAt(src, i - 1));
      showEdit(str, src, '\0', i);
      break;
    case REPLACE:
      reconstructPath(src, tgt, i - 1, j - 1, ops, kill, str);
      printf("Replace %c with %c:\n  ", charAt(src, i - 1), 
             charAt(tgt, j - 1));
      showEdit(str, src, charAt(tgt, j - 1), i);
      break;
    case SWAP:
      reconstructPath(src, tgt, i - 2, j - 2, ops, kill, str);
      printf("Swap %c%c with %c%c:\n  ", charAt(src, i - 2), 
        charAt(src, i - 1), charAt(tgt, j - 2), charAt(tgt, j - 1));
      appendChar(str, charAt(tgt, j - 2));
      showEdit(str, src, charAt(tgt, j - 1), i);
      break;
    case KILL:
      reconstructPath(src, tgt, kill, j, ops, kill, str);
      printf("Kill %zu remaining chars:\n  ", strLen(src) - kill);
      showEdit(str, src, '\0', strLen(src));
      break;
  }

  if (i == strLen(src) && j == strLen(tgt)) {
    printf("Target:\n  ");
    showString(tgt);
  }
}

//===================================================================

int main () {
  
    // read source and target strings
  READ_STRING(src, '\n');
  READ_STRING(tgt, '\n');

    // read operation costs 
  size_t costs[6];
  READ_ARRAY(costs, "%zu", 6);

    // create memoization table
  CREATE_MATRIX(size_t, dp, strLen(src) + 1, 
                strLen(tgt) + 1, SIZE_MAX);

    // create operations table
  CREATE_MATRIX(op, ops, strLen(src) + 1, 
                strLen(tgt) + 1, INSERT);

    // populate the memoization table and operations table
  size_t kill = strLen(src);
  size_t med = computeMED(src, tgt, strLen(src), 
                          strLen(tgt), costs, dp, ops, &kill);

    // show edit distance and an optimal operation sequence
  printf("Minimum edit distance: %zu\n\n"
         "Optimal operation sequence:\n\n", med);
  if (med) {
    string *str = newString(strLen(src) + strLen(tgt) + 1);
    reconstructPath(src, tgt, strLen(src), strLen(tgt), 
                    ops, kill, str);
    freeString(str);
  } else printf("None\n");

    // deallocate memory
  FREE_MATRIX(dp, strLen(src) + 1);
  FREE_MATRIX(ops, strLen(src) + 1);
  freeString(src);
  freeString(tgt);
  
  return 0;
}