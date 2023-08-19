# Greedy Algorithms

| **CLRS³** | **CLRS⁴** | **Link** |
|:---:|:---:|:---|
| Chap 16.1 | Chap 15.1 | [Activity-selection Problem](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/activity-selection)
| Chap 16.2 | Chap 15.2 | [Fractional Knapsack](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/fract-knapsack)
| Chap 16.3 | Chap 15.3 | [Huffman Codes](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/huffman)
| Prob 16-1 | Prob 15-1 | [Coin Changing](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/coin-changing)
| Chap 23 | Chap 21 | [MST - Kruskal](https://github.com/pl3onasm/CLRS-in-C/tree/main/algorithms/graphs/MST-kruskal)
| Chap 23 | Chap 21 | [MST - Prim](https://github.com/pl3onasm/CLRS-in-C/tree/main/algorithms/graphs/MST-prim)
| Chap 24.3 | Chap 22.3 | [SSSP - Dijkstra](https://github.com/pl3onasm/CLRS-in-C/tree/main/algorithms/graphs/SSSP-dijkstra)

# Greedy Approach

Like [dynamic programming](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming), greedy algorithms are best suited for optimization problems. Unlike dynamic programming, which first works out the optimal solutions to subproblems and then uses these solutions to make choices that eventually lead to an optimal solution to the original problem, greedy algorithms make whichever choice seems best at the moment and then move on to solve the subproblems that arise after making that choice. The choice made by a greedy algorithm may depend on *choices* made so far, but not on *future* choices or on the *solutions* to subproblems. Greedy algorithms may also be used as an approximation algorithm for a problem.  

There are two key properties of a problem that suggest the use of a greedy algorithm:

1. **Greedy choice property**  
   The property holds if it is possible to reach a global optimum by making the greedy choice at each step. That is, at each step, we make the choice that looks best at that moment, without looking ahead to see whether it is the best choice or without depending on solutions to subproblems. This is the key difference with [dynamic programming](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming). In other words, the greedy choice is made locally, without considering its global effect, and the property holds if that choice is safe to make at each step.

2. **Optimal substructure**  
   A problem has optimal substructure if an optimal solution can be constructed from optimal solutions to its subproblems. This is a key ingredient for both dynamic programming and greedy algorithms. In the case of greedy algorithms, each greedy choice leads to a smaller subproblem, and we need to show that an optimal solution to the subproblem, combined with the greedy choice that led to the subproblem, yields an optimal solution to the original problem.  
