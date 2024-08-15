/* 
  file: ccp-3.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: coin changing problem (CCP)
               using a combined greedy and DP approach
  time complexity: O(n), where n is the number of coins in the coin system
*/ 

#include "../../../lib/clib/clib.h"

//===================================================================
// Returns the maximum element in an array
size_t maxArray (size_t *arr, size_t len) {
  size_t max = arr[0];
  for (size_t i = 1; i < len; ++i) 
    if (arr[i] > max) max = arr[i];
  return max;
}

//===================================================================
// Computes the optimal change for all amounts up to given amount
// using a greedy approach
void grdMakeChange(size_t *coins, size_t nCoins, size_t amount, 
                   size_t *minCoins) {

  size_t idx = 0;
  while (amount) 
    minCoins[amount--] = amount >= coins[idx] ? 
                         coins[idx] : (idx < nCoins - 1 ?
                         coins[++idx] : 0); 
}

//===================================================================
// Computes the optimal change for all amounts up to given amount
// using a DP approach
void dpMakeChange(size_t *coins, size_t nCoins, size_t amount, 
                     size_t *minCoins) {

  CREATE_ARRAY(size_t, dp, amount + 1, 0);

    // compute optimal solutions by increasing amount
  for (size_t i = 1; i <= amount; ++i) {     
    dp[i] = amount;
      // try all coins for the current amount and update dp
      // if a better solution is found
    for (size_t j = 0; j < nCoins; ++j) {        
      if (i >= coins[j] && dp[i - coins[j]] + 1 < dp[i]) {
        dp[i] = dp[i - coins[j]] + 1;  
        minCoins[i] = coins[j];                      
      }
    }
  }

  free(dp);
}

//===================================================================
// Checks whether a coin system is canonical or not
bool checkCanonical(size_t *coins, size_t nCoins) {

  size_t n = coins[0] + coins[1];
  CREATE_ARRAY(size_t, grdChanges, n + 1, 0);
  CREATE_ARRAY(size_t, dpChanges, n + 1, 0);

  grdMakeChange(coins, nCoins, n, grdChanges);
  dpMakeChange(coins, nCoins, n, dpChanges);

    // see if there is a counterexample to the canonicality
  for (size_t i = 0; i <= n; i++) 
    if (grdChanges[i] != dpChanges[i]) {
      free(grdChanges);
      free(dpChanges);
      return false;
    }

  free(grdChanges);
  free(dpChanges);
  return true;
}

//===================================================================
// Prints the optimal change for a given amount
void showChange(size_t *minChanges, size_t amount) {

  printf("%zu = ", amount);

  if (amount == 0) {
    printf("0\n");
    return;
  }

  size_t coin = minChanges[amount], nCoins = 0;
  while (amount && coin) {
    while (minChanges[amount] == coin) {
      amount -= coin;
      nCoins++;
    }
    printf(nCoins > 1 ? "%zu x %zu" : "%zu", coin, nCoins);
    coin = minChanges[amount];
    nCoins = 0;
    printf(amount && coin ? " + " : "\n");
  }
}

//===================================================================

int main() {

    // read coin system
  READ_UNTIL(size_t, coins, "%zu", '.', nCoins);

    // check whether the coin system is canonical or not
  bool canonical = checkCanonical(coins, nCoins);

    // read amounts to be changed
  READ_UNTIL(size_t, amounts, "%zu", '.', nAmounts);

    // get maximum amount to be changed
  size_t max = maxArray(amounts, nAmounts); 

    // compute optimal solutions for all amounts up to max
  CREATE_ARRAY(size_t, minChanges, max + 1, 0);

  if (canonical) {
    printf("Canonical coin system\n... using greedy approach\n\n");
    grdMakeChange(coins, nCoins, max, minChanges);
  } else { 
    printf("Non-canonical coin system\n... using DP approach\n\n");
    dpMakeChange(coins, nCoins, max, minChanges);
  }

    // show optimal solutions for all requested amounts
  for (int i = 0; i < nAmounts; i++)
    showChange(minChanges, amounts[i]);
    
  free(minChanges);
  free(coins);
  free(amounts);

  return 0;
}