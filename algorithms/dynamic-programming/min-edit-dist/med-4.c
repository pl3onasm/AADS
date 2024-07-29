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
void checkMin(size_t **dp, op **ops, size_t i, size_t j, 
              size_t oldCost, op newOp, size_t *costs, size_t k) {

  if (dp[i][j] > oldCost + costs[newOp]) {
    dp[i][j] = oldCost + costs[newOp];
    ops[i][j] = newOp;
    if (newOp == KILL) costs[6] = i - k;
  }
}

//===================================================================
// Bottom-up approach to compute the minimum edit distance
size_t computeMED(string *src, string *tgt, size_t *costs, 
                  size_t **dp, op **ops) {

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
        checkMin(dp, ops, i, j, dp[i - 1][j - 1], COPY, costs, 0);
      
      if (i > 1 && j > 1 && charAt(src, i - 1) == charAt(tgt, j - 2) 
          && charAt(src, i - 2) == charAt(tgt, j - 1)) {
        checkMin(dp, ops, i, j, dp[i - 2][j - 2], SWAP, costs, 0);
      }

      checkMin(dp, ops, i, j, dp[i][j - 1], INSERT, costs, 0);
      
      checkMin(dp, ops, i, j, dp[i - 1][j], DELETE, costs, 0);

      if (charAt(src, i - 1) != charAt(tgt, j - 1))
        checkMin(dp, ops, i, j, dp[i - 1][j - 1], REPLACE, costs, 0);
    }
  }
  
    // check if killing last remaining chars is cheaper
  for (size_t k = 0; k < strLen(src); ++k) 
    checkMin(dp, ops, strLen(src), strLen(tgt), 
             dp[k][strLen(tgt)], KILL, costs, k);

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
// into target string while showing all intermediate edit steps
void recPath(string *src, string *tgt, size_t i, size_t j, 
             op **ops, string *str, size_t *costs) {

  if (i == 0 && j == 0) {
    printf("Source:\n  ");
    showString(src);
    return;
  }

  switch (ops[i][j]) {
    case COPY:
      recPath(src, tgt, i - 1, j - 1, ops, str, costs);
      printf("Copy %c:\n  ", charAt(src, i - 1)), 
      showEdit(str, src, charAt(src, i - 1), i);
      break;
    case INSERT:
      recPath(src, tgt, i, j - 1, ops, str, costs);
      printf("Insert %c:\n  ", charAt(tgt, j - 1));
      showEdit(str, src, charAt(tgt, j - 1), i);
      break;
    case DELETE:
      recPath(src, tgt, i - 1, j, ops, str, costs);
      printf("Delete %c:\n  ", charAt(src, i - 1));
      showEdit(str, src, '\0', i);
      break;
    case REPLACE:
      recPath(src, tgt, i - 1, j - 1, ops, str, costs);
      printf("Replace %c with %c:\n  ", charAt(src, i - 1), 
             charAt(tgt, j - 1));
      showEdit(str, src, charAt(tgt, j - 1), i);
      break;
    case SWAP:
      recPath(src, tgt, i - 2, j - 2, ops, str, costs);
      printf("Swap %c%c with %c%c:\n  ", charAt(src, i - 2), 
        charAt(src, i - 1), charAt(tgt, j - 2), charAt(tgt, j - 1));
      appendChar(str, charAt(tgt, j - 2));
      showEdit(str, src, charAt(tgt, j - 1), i);
      break;
    case KILL:
      recPath(src, tgt, strLen(src) - costs[6], j, ops, str, costs);
      printf("Kill %zu remaining chars:\n  ", costs[6]);
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

    // read operation costs (copy, insert, delete, replace, swap,
    // kill); the 7th cell is for storing the number of chars killed
  size_t costs[7]; 
  READ_ARRAY(costs, "%zu", 6);

    // create memoization table
  CREATE_MATRIX(size_t, dp, strLen(src) + 1, 
                strLen(tgt) + 1, SIZE_MAX);

    // create operations table
  CREATE_MATRIX(op, ops, strLen(src) + 1, strLen(tgt) + 1, INSERT);

    // compute minimum edit distance
  size_t med = computeMED(src, tgt, costs, dp, ops);

    // show edit distance and an optimal operation sequence
  printf("Minimum edit distance: %zu\n\n"
         "Optimal operation sequence:\n\n", med);

  if (med) {
    string *str = newString(strLen(src) + strLen(tgt) + 1);
    recPath(src, tgt, strLen(src), strLen(tgt), ops, str, costs);
    freeString(str);
  } else printf("None\n");

    // deallocate memory
  FREE_MATRIX(dp, strLen(src) + 1);
  FREE_MATRIX(ops, strLen(src) + 1);
  freeString(src);
  freeString(tgt);

  return 0;
}