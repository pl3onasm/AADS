# Rod Cutting

## Problem

Given a rod of length $n$ inches and a table of prices $p_i$ for rod lengths $i = 1, 2, ..., n$, determine the maximum revenue $r_n$ obtainable by cutting up the rod and selling the pieces. Note that if the price $p_n$ for a rod of length $n$ is large enough, an optimal solution may require no cutting at all.

The idea is to cut the rod into two pieces, and then solve the problem for each of the two pieces. The optimal solution for the original rod is then the sum of the optimal solutions for the two pieces.
More concretely, the optimal solution for a rod of length $n$ is the maximum of the following two values:

1. The price $p_n$ for a rod of length $n$.  
2. The sum of the optimal solutions for the two pieces of lengths $i$ and $n - i$ obtained by cutting the rod into two pieces at each possible position $1 \leq i \leq n-1$.  

A slightly different way of stating the same idea is to define the revenue $r_n$ as the maximum revenue obtainable from a rod of length $n$, and to write the relationship between $r_n$ and $p_n$ as follows:

$$
r_n = \max \lbrace p_i + r_{n-i} : 1 \leq i \leq n \rbrace
$$

We simply try all possible combinations of an uncut piece of length $i$ and the recursive solution to the problem for the remaining piece of length $n-i$. The maximum revenue is then the maximum of all these combinations.

## Brute Force Solution

This approach finds the maximum revenue by trying all possible values for $i$ and keeping track of the maximum revenue, whilst ignoring the fact that the same subproblem (finding optimal cuts for a smaller rod length) may be encountered multiple times during the recursive calls and is thus recomputed over and over again. The running time of this approach is in $O(2^n)$. It is easy to see why: each possible solution is a permutation of cut decisions, and there are $2^{n-1}$ such permutations, all of which need to be checked.

Implementation: [Naive recursive solution](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod1.c)

## Top-down Approach with Memoization

The problem clearly contains overlapping subproblems: to compute the optimal solution for rods of different lengths, we repeatedly need the optimal solutions for smaller rod lengths. The previous approach can be repaired by storing the optimal solution for each rod length in a table and using this table to avoid recomputing the same subresult over and over again. This approach is called *memoization*: we literally keep a memo of each subproblem solution and use this memo instead of recomputing the solution when needed. The running time of this approach is in $O(n^2)$.

Implementation: [Top-down DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod2.c)

## Bottom-up Approach

The bottom-up approach solves the subproblems in order of increasing piece size, while storing the solution to each subproblem when it is first solved. Thus, the maximum revenues for subproblems of size $1, 2, ..., n$ are computed in ascending order, and all required partial solutions are readily available to solve the next subproblem in line. The solution to the original problem is then the solution to the subproblem of size $n$. The running time of this approach is also in $O(n^2)$. Note that the bottom-up approach requires a double loop, whereas the top-down approach only needs a single loop, since it uses recursion instead.

Implementation: [Bottom-up DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod3.c)

## Extended Bottom-up Approach

The extended bottom-up approach not only computes the maximum revenue, but also an optimal solution: a list of piece sizes. For this purpose, an array is used to store the optimal first cut for each rod length.

Implementation: [Extended bottom-up DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod4.c)
