# Coin Changing Problem (CCP)

## Problem

Given a set of coins of different denominations, find the minimum number of coins needed to make a change for a given amount, assuming there is an infinite supply of each coin.

## Greedy Algorithm

The greedy algorithm is to always choose the largest coin that is less than or equal to the remaining amount. However, this approach only works for certain sets of coins. For example, the greedy approach works for the American coin system, which consists of coins for 1 (penny), 5 (nickel), 10 (dime), and 25 (quarter) cents. However, it stops working if we remove the nickel from the coin system. For example, if we want to make a change for 30 cents, the greedy approach will then yield 6 coins (1 quarter and 5 pennies), while the optimal solution in that case is 3 coins (3 dimes).

The coin systems for which the greedy algorithm always yields the optimal solution are called **canonical coin systems**. The question as to which conditions must be satisfied for a set of coins to be a canonical coin system is not easy to answer, and has been the subject of research. Also some algorithms have been designed to verify whether a given set of coins is canonical or not [^1] [^2] [^3].

Implementing the greedy algorithm is straightforward. We just need to iterate through the coins in descending order, and for each coin determine if the remaining amount is divisible by the coin's value. If it is, we add the quotient to the total number of coins, and subtract the quotient times the coin's value from the remaining amount. If the remaining amount is not divisible by the coin's value, we simply move on to the next coin. The algorithm terminates when the remaining amount is 0.

Time complexity: O(n), where n is the number of coins in the coin system.

Implementation: [CCP - Greedy](https://github.com/pl3onasm/AADS/blob/main/algorithms/greedy/coin-changing/ccp-1.c)

[^1]: [Pearson, D. (2005). A polynomial-time algorithm for the change-making problem. Operations Research Letters, 33(3), 231-234.](https://graal.ens-lyon.fr/~abenoit/algo09/coins2.pdf)
[^2]: [Cai, X. (2009, August). Canonical coin systems for change-making problems. In 2009 Ninth International Conference on Hybrid Intelligent Systems (Vol. 1, pp. 499-504). IEEE.](https://arxiv.org/pdf/0809.0400.pdf)
[^3]: [Chan, T. M., & He, Q. (2020). On the change-making problem. In Symposium on Simplicity in Algorithms (pp. 38-42). Society for Industrial and Applied Mathematics.](https://tmc.web.engr.illinois.edu/coin_sosa.pdf)

## Dynamic Programming

Given the fact that the greedy approach doesn't yield the optimal solution for all coin systems, there's always the option to fall back on dynamic programming. Of course, the downside of dynamic programming is that it requires more space and time than the greedy approach. So if the given coin system is canonical, the greedy approach is preferred.

It should be an easy exercise to implement a top-down approach with memoization. We'll limit ourselves to the bottom-up approach. Let $D = \lbrace d_1, d_2, \ldots, d_n \rbrace$ be the set of coins in the coin system, and let $A$ be the amount for which we want to make a change. We define $C[A]$ as the minimum number of coins needed to make a change for $A$. We can then define the recurrence relation as follows:

$$
C[A] = \begin{cases}
0 & \text{if } A = 0 \\
1 + \text{min}_{i=1}^n \lbrace C[A - d_i] \rbrace & \text{if } A > 0
\end{cases}
$$

The recurrence relation is easy to implement in a bottom-up fashion. We just need to iterate through the amounts ranging from 1 to $A$, and for each amount we iterate through the coins in the coin system, and check if the remaining amount is greater than or equal to the coin's value. If it is, we update the minimum number of coins needed to make a change for the current amount. The algorithm terminates when the minimum number of coins needed to make a change for $A$ is found.

Time complexity: O(nA), where n is the number of coins in the coin system, and A is the amount for which we want to make a change.

For the implementation, we simply add a coin of 20 cents to the previous example of the American coin system: $D = \lbrace 25, 20, 10, 5, 2, 1 \rbrace$ to make it non-canonical. To see that this system is not canonical, observe that for the amount of 40 cents, the greedy approach yields 3 coins (25 + 10 + 5), while the optimal solution is 2 coins (20 + 20). The bottom-up approach yields the optimal solution.

Implementation: [CCP - Bottom-up DP](https://github.com/pl3onasm/AADS/blob/main/algorithms/greedy/coin-changing/ccp-2.c)
