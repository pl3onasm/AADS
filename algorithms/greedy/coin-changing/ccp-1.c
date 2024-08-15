/* 
  file: ccp-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: coin changing problem (CCP)
              using a greedy approach 
  time complexity: O(n), where n is the number of coins in the coin system
*/ 

#include "../../../lib/clib/clib.h"

//===================================================================
// Makes change for a given amount using a greedy approach
void makeChange(size_t *coins, size_t nCoins, size_t amount) {
  printf("%zu = ", amount);
  size_t j = 0, coin = coins[j];
  size_t remAmount = amount;

  while (remAmount > 0 && j < nCoins) {
      // if amount is smaller than current coin, take next coin
    if (remAmount < coin) coin = coins[++j]; 
    else {
        // compute number of coins of current value 
        // and update remaining amount
      size_t q = remAmount / coin;
      remAmount %= coin; 
      printf(q > 1 ? "%zu x %zu" : "%zu", coin, q);
      printf(remAmount > 0 ? " + " : "\n");
    }
  }
}

//===================================================================

int main() {

    // read coin system
  READ_UNTIL(size_t, coins, "%zu", '.', nCoins);

    // read amounts to be changed
  READ_UNTIL(size_t, amounts, "%zu", '.', nAmounts);
  
    // make change for each amount
  for (size_t i = 0; i < nAmounts; i++) 
    makeChange(coins, nCoins, amounts[i]);

  free(coins);
  free(amounts);
  
  return 0;
}