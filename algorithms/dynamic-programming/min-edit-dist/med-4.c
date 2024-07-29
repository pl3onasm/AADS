/* 
  file: med-4.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: minimum edit distance
    using bottom-up DP with path reconstruction
  time complexity: O(n*m)
*/

#include "../../../lib/clib/clib.h"
#include <stdint.h>
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
// Bottom-up approach to compute the minimum edit distance
size_t computeMED(string *src, string *tgt, size_t *costs, 
                  size_t **dp, op **ops, size_t *kill) {

    // target is empty: delete all source chars
  for (size_t i = 0; i <= strLen(src); ++i) {
    dp[i][0] = i * costs[DELETE];
    ops[i][0] = DELETE;
  }

    // source is empty: insert all target chars
  for (size_t j = 0; j <= strLen(tgt); ++j) {
    dp[0][j] = j * costs[INSERT];
    ops[0][j] = INSERT;
  }

  for (size_t i = 1; i <= strLen(src); ++i) {
    for (size_t j = 1; j <= strLen(tgt); ++j) {
      
      if (charAt(src, i - 1) == charAt(tgt, j - 1))
        updateMin(dp, ops, i, j, dp[i - 1][j - 1], COPY, costs);
      
      if (i > 1 && j > 1 && charAt(src, i - 1) == charAt(tgt, j - 2) 
          && charAt(src, i - 2) == charAt(tgt, j - 1)) {
        updateMin(dp, ops, i, j, dp[i - 2][j - 2], SWAP, costs);
      }

      updateMin(dp, ops, i, j, dp[i][j - 1], INSERT, costs);
      
      updateMin(dp, ops, i, j, dp[i - 1][j], DELETE, costs);

      if (charAt(src, i - 1) != charAt(tgt, j - 1))
        updateMin(dp, ops, i, j, dp[i - 1][j - 1], REPLACE, costs);
    }
  }
  
    // check if killing last remaining chars is cheaper
  for (size_t k = 0; k < strLen(src); ++k) {
    size_t oldCost = dp[k][strLen(tgt)];
    updateMin(dp, ops, strLen(src), strLen(tgt), 
              oldCost, KILL, costs);
    if (dp[strLen(src)][strLen(tgt)] == oldCost + costs[KILL])
      *kill = k;
  }

  return dp[strLen(src)][strLen(tgt)];
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
                strLen(tgt) + 1, COPY);

    // compute minimum edit distance
  size_t kill = 0;
  size_t med = computeMED(src, tgt, costs, dp, ops, &kill);

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