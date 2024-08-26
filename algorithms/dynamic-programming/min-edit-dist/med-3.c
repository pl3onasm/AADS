/* 
  file: med-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: minimum edit distance
    using top-down DP with memoization and path reconstruction
  time complexity: O(n*m)
*/

#include "../../../lib/clib.h"
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
// Top-down DP approach to compute the minimum edit distance
// between source and target strings using memoization
size_t computeMED(string *src, string *tgt, size_t i, size_t j, 
                  size_t *costs, size_t **dp, op **ops) {

    // return the value if it has already been computed
  if (dp[i][j] != SIZE_MAX) return dp[i][j];

    // source is empty: insert remaining target chars
  if (i == 0) {
    ops[i][j] = INSERT;
    return dp[i][j] = j * costs[INSERT];
  }

    // check if killing last remaining chars is cheaper
  size_t med;
  if (i == strLen(src) && j == strLen(tgt)) 
    for (size_t k = 0; k < strLen(src); ++k) {
      med = computeMED(src, tgt, k, j, costs, dp, ops);
      checkMin(dp, ops, i, j, med, KILL, costs, k);
    }

    // target is empty: delete all remaining source chars 
    // or kill if it's the only and cheapest operation
  if (j == 0) {
    if (ops[i][j] == KILL && i * costs[DELETE] > costs[KILL])
      return dp[i][j];
    ops[i][j] = DELETE;
    return dp[i][j] = i * costs[DELETE];
  }

  if (charAt(src, i - 1) == charAt(tgt, j - 1)){
    med = computeMED(src, tgt, i - 1, j - 1, costs, dp, ops);
    checkMin(dp, ops, i, j, med, COPY, costs, 0);
  }

  if (i > 1 && j > 1 && charAt(src, i - 1) == charAt(tgt, j - 2) 
      && charAt(src, i - 2) == charAt(tgt, j - 1)) {
    med = computeMED(src, tgt, i - 2, j - 2, costs, dp, ops);
    checkMin(dp, ops, i, j, med, SWAP, costs, 0);
  }

  med = computeMED(src, tgt, i, j - 1, costs, dp, ops);
  checkMin(dp, ops, i, j, med, INSERT, costs, 0);

  med = computeMED(src, tgt, i - 1, j, costs, dp, ops);
  checkMin(dp, ops, i, j, med, DELETE, costs, 0);

  if (charAt(src, i - 1) != charAt(tgt, j - 1)) {
    med = computeMED(src, tgt, i - 1, j - 1, costs, dp, ops);
    checkMin(dp, ops, i, j, med, REPLACE, costs, 0);
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

    // populate the memoization table and operations table
  size_t med = computeMED(src, tgt, strLen(src), 
                          strLen(tgt), costs, dp, ops);

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