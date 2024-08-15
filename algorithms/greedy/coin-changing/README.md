${\huge\color{Cadetblue}\text{Coin Changing}}$  
${\huge\color{Cadetblue}\text{Problem (CCP)}}$

<br />

${\Large\color{rosybrown}\text{Problem}}$

Given a set of coins of different denominations, find the minimum number of coins needed to make a change for a given amount, assuming there is an infinite supply of each coin.

<br/>

${\Large\color{darkseagreen}\text{Greedy approach}}$

The greedy strategy is to always choose the largest coin that is less than or equal to the remaining amount. However, this approach only works for certain sets of coins. For example, the greedy approach works for the American coin system, which consists of coins for 1 (penny), 5 (nickel), 10 (dime), and 25 (quarter) cents. However, it stops working if we remove the nickel from the coin system. For example, if we want to make a change for 30 cents, the greedy approach will then yield 6 coins (1 quarter and 5 pennies), while the optimal solution in that case is 3 coins (3 dimes).

The coin systems for which the greedy algorithm always yields the optimal solution are called ${\color{peru}\text{canonical coin systems}}$. The question as to which conditions must be satisfied for a set of coins to be a canonical coin system is not easy to answer, and has been the subject of research. Also some algorithms have been designed to verify whether a given set of coins is canonical or not [^1] [^2] [^3].

Implementing the greedy algorithm is straightforward. We just need to iterate through the coins in descending order, and for each coin determine if the remaining amount is divisible by the coin's value. If it is, we add the quotient to the total number of coins, and subtract the quotient times the coin's value from the remaining amount. If the remaining amount is not divisible by the coin's value, we simply move on to the next coin. The algorithm terminates when the remaining amount is 0.

Time complexity: $\mathcal{O}(n)$, where n is the number of coins in the coin system.

Implementation: [CCP - Greedy](https://github.com/pl3onasm/AADS/blob/main/algorithms/greedy/coin-changing/ccp-1.c)

[^1]: [Pearson, D. (2005). A polynomial-time algorithm for the change-making problem. Operations Research Letters, 33(3), 231-234.](https://graal.ens-lyon.fr/~abenoit/algo09/coins2.pdf)
[^2]: [Cai, X. (2009, August). Canonical coin systems for change-making problems. In 2009 Ninth International Conference on Hybrid Intelligent Systems (Vol. 1, pp. 499-504). IEEE.](https://arxiv.org/pdf/0809.0400.pdf)
[^3]: [Chan, T. M., & He, Q. (2020). On the change-making problem. In Symposium on Simplicity in Algorithms (pp. 38-42). Society for Industrial and Applied Mathematics.](https://tmc.web.engr.illinois.edu/coin_sosa.pdf)

<br/>

${\Large\color{darkseagreen}\text{DP approach}}$

Given the fact that the greedy approach doesn't yield the optimal solution for all coin systems, there's always the option to resort to dynamic programming. Of course, the downside of dynamic programming is that it requires more space and time than the greedy approach. So if the given coin system is canonical, the greedy approach is definitely preferred.

Let $D = \lbrace d_1, d_2, \ldots, d_n \rbrace$ be the set of coins in the coin system, and let $A$ be the amount for which we want to make a change. We define $C(A)$ to be a function that returns the minimum number of coins needed to make a change for $A$ using the coins in $D$, where $C(0) = 0$. The recurrence relation for $C(A)$ is then as follows:

$$\color{darkslateblue}\boxed{\color{rosybrown}\space
C(A) = \begin{cases}
0 & \scriptsize \text{if } A = 0 \\
1 + \min \lbrace C(A - d_i) \\
\qquad \qquad : 1 \leq i \leq n \rbrace & \scriptsize \text{if } A > 0
\end{cases}\space}
$$

<br/>

The recurrence is easy to implement in a bottom-up fashion. We simply go through the amounts ranging from 1 to $A$, and then for each of these amounts, we iterate through the coins in $D$ to find the minimum number of coins needed to make a change for that amount, based on the optimal solutions that were already computed for the smaller amounts. The algorithm terminates when the amount $A$ is reached.

Time complexity: $\mathcal{O}(nA)$, where $n$ is the number of coins in the coin system, and $A$ is the amount for which we want to make a change.

For the sake of illustration, and to show that the dynamic programming approach works for non-canonical coin systems as well, we simply add a coin of 20 cents to the previous example of the American coin system to make it non-canonical: $D = \lbrace 25, 20, 10, 5, 1 \rbrace$. To see that this system is non-canonical, observe that for the amount of 40 cents, the greedy approach yields 3 coins (25 + 10 + 5), while the optimal solution is 2 coins (20 + 20). The DP approach yields this optimal solution.

Implementation: [CCP - Bottom-up DP](https://github.com/pl3onasm/AADS/blob/main/algorithms/greedy/coin-changing/ccp-2.c)

<br/>

${\Large\color{darkseagreen}\text{Combining greedy and DP}}$

It is possible to combine the greedy and dynamic programming approaches and let the program decide which approach to use based on the coin system. For this, we use the fact that if a coin system is non-canonical, then there exists at least one counterexample, i.e. at least one amount for which the greedy approach does not yield the optimal solution, just like in the examples given above. Of course, it would be inefficient to keep checking for counterexamples until we find one. Luckily, we do have an upper bound (thanks to Dexter Kozen and Shmuel Zaks): the smallest counterexample is always less than the sum of the two largest coins in the coin system. So we can simply do a one-time check for the smallest counterexample, and if it is found, we use the dynamic programming approach, otherwise we use the greedy approach for the entire coin system.

Implementation: [CCP - Greedy/DP](https://github.com/pl3onasm/AADS/blob/main/algorithms/greedy/coin-changing/ccp-3.c)
