/* 
  file: ccp-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: coin changing problem (CCP)
    using a bottom-up dynamic programming approach
  time complexity: O(nk), where n is the number of coins 
  in the coin system and k is the amount to be changed
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
// Returns an array where the i-th element contains the denomination
// of a coin used as a last step in an optimal solution to the
// (sub)problem of making change for amount i
size_t *dpMakeChange(size_t *coins, size_t nCoins, size_t amount) {

  CREATE_ARRAY(size_t, dp, amount + 1, 0);
  CREATE_ARRAY(size_t, minCoins, amount + 1, 0);

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
  return minCoins; 
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

    // read amounts to be changed
  READ_UNTIL(size_t, amounts, "%zu", '.', nAmounts);

    // get maximum amount to be changed
  size_t max = maxArray(amounts, nAmounts); 

    // compute optimal solutions for all amounts up to max
  size_t *minChanges = dpMakeChange(coins, nCoins, max);

    // print optimal solutions for all amounts
  for (int i = 0; i < nAmounts; i++)
    showChange(minChanges, amounts[i]);     
    
  free(minChanges);
  free(coins);
  free(amounts);

  return 0;
}