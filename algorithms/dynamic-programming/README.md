# ${\color{Cadetblue}\text{Dynamic programming}}$

## ${\color{Rosybrown}\text{Overview}}$

| ${\color{peru}\text{CLRS (3/4)}}$ | ${\color{peru}\text{Link}}$ |
|:---|:---|
| 15/14.1 | [Rod Cutting](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting) |
| 15/14.2 | [Matrix Chain Multiplication](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/matrix-chain-mult) |
| 15/14.4 | [Longest Common Subsequence](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-common-sub) |
| Prob 15/14-2 | [Longest Palindromic Subsequence](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-palin-sub) |
| Ex 15/14.4-4,5 |[Longest Increasing Subsequence](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-increasing-sub) |
| Ex 16/15.2-2 | [0-1 Knapsack Problem](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/knapsack) |
| – | [Minimal Palindromic Partitioning](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/min-pal-part) |
| Prob 15/14-5 | [Minimum Edit Distance](https://github.com/pl3onasm/AADS/tree/main/algorithms/dynamic-programming/min-edit-dist) |
| 25/23.2 | [Floyd-Warshall](https://github.com/pl3onasm/CLRS-in-C/tree/main/algorithms/graphs/APSP-floyd) |

&nbsp;

## ${\color{Rosybrown}\text{DP approach}}$

Dynamic programming is most suited for optimization problems. Such problems should have two main properties in order to apply dynamic programming:

1. ${\color{Mediumorchid}\text{Optimal substructure}}$  
   The optimal solution to a problem consists of optimal solutions to its subproblems. Mind that the subproblems of the *same* problem should be ${\color{peru}\text{\it independent}}$ of each other, i.e. the optimal solution to one of its subproblems should not depend on the optimal solution to another of its subproblems.
2. ${\color{Mediumorchid}\text{Overlapping subproblems}}$  
   Subproblems share subsubproblems, and the same subsubproblems are encountered multiple times when solving different subproblems. Dynamic programming takes advantage of this property by solving each subsubproblem only once and then storing the solution in a table where it can be looked up when needed.  
   A good test for this property is to see if the recursive solution has repeated calls for the same inputs. This is different from the type of subproblems that [divide-and-conquer](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/divide-and-conquer) algorithms solve, where recursive calls typically generate new subproblems each time. A divide-and-conquer approach for a problem whose subproblems overlap would be inefficient, since it would solve the same subproblem each time it reappears in the recursion tree.

