# ${\color{Cadetblue}\text{Rod cutting}}$

## ${\color{rosybrown}\text{Problem}}$

Given a rod of length $n$ inches and a table of prices $p_i$ for rod lengths $i = 1, 2, ..., n$, determine the maximum revenue $r_n$ obtainable by cutting up the rod and selling the pieces. Note that if the price $p_n$ for a rod of length $n$ is large enough, an optimal solution may require no cutting at all.

## ${\color{darkseagreen}\text{The key idea}}$

The idea is to cut the rod into two pieces, and then solve the problem for each of the two pieces. The optimal solution for the original rod is then the sum of the optimal solutions for the two pieces.
More concretely, the optimal solution for a rod of length $n$ is the maximum of the following two values:

1. The price $p_n$ for a rod of length $n$.  
2. The sum of the optimal solutions for the two pieces of lengths $i$ and $n - i$ obtained by cutting the rod into two pieces at each possible position $1 \leq i \leq n-1$.  

A slightly different way of formulating the same idea is to define the revenue $r_n$ as the maximum revenue obtainable from a rod of length $n$, and to express the relationship between $r_n$ and $p_n$ as follows:

$$
{\color{darkviolet}\boxed{\color{rosybrown}\space r_n = \max \lbrace p_i + r_{n-i} : 1 \leq i \leq n \rbrace \space}}
$$

We simply try all possible combinations of an uncut piece of length $i$ and the recursive solution to the problem for the remaining piece of length $n-i$. The maximum revenue is then the maximum of all these combinations.

## ${\color{darkseagreen}\text{Brute force}}$

The brute force way of working out the above idea is to use a recursive approach that finds the maximum revenue by just trying all possible values for $i$ and keeping track of the maximum revenue found. Of course, this approach is very inefficient, since it completely ignores the fact that the same subproblem of finding the optimal cuts for a particular rod length is sure to be encountered multiple times during the recursive calls and is thus recomputed over and over again. 

The running time of this approach is in $\mathcal{O}(2^n)$, and it is easy to see why: each possible solution is a permutation of cut decisions, and there are $2^{n-1}$ such permutations, all of which need to be checked.

Implementation: [Naive recursive solution](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod1.c)

## ${\color{darkseagreen}\text{Top-down}}$

As mentioned before, the previous approach ignores the fact that the problem contains overlapping subproblems: to compute the optimal solution for rods of different lengths, we repeatedly need the optimal solutions for the same smaller rod lengths. This flaw can be repaired, however, by storing the optimal solution for each rod length in a table as soon as it is computed, and checking this table to see if the solution is already available before computing anything. This strategy is called ${\color{peru}\text{memoization}}$: we literally keep a memo of each solution to a subproblem, and use this memo instead of recomputing the solution again when we need it. Obviously, this saves a lot of time, since we only compute each subproblem once, resulting in a dramatic drop to an overall running time of $\mathcal{O}(n^2)$. The approach is called ${\color{peru}\text{top-down}}$ because we start with the problem we want to solve and recursively break it down into smaller subproblems until we reach the base case, which is the smallest subproblem that can be solved directly.

Implementation: [Top-down DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod2.c)

## ${\color{darkseagreen}\text{Bottom-up}}$

The bottom-up DP approach also uses a table to store the optimal solutions for subproblems, but it does not use recursion. Instead, subproblems are solved iteratively, starting with the smallest subproblems and working up to the original problem, while storing the solution to each subproblem when it is first solved. Thus, the maximum revenues for subproblems of size $1, 2, ..., n$ are computed in order of increasing piece size, in a ${\color{peru}\text{bottom-up}}$ fashion, ensuring that all required partial solutions are readily available to solve the next subproblem in line. The solution to the original problem is then the solution to the subproblem of size $n$. The running time of this approach is also in $\mathcal{O}(n^2)$.  

Implementation: [Bottom-up DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod3.c)

## ${\color{darkseagreen}\text{Extended bottom-up}}$

The extended bottom-up approach not only computes the maximum revenue, but also an optimal solution: a list of piece sizes that together yield the maximum revenue. This is done by keeping track of the ${\color{peru}\text{optimal first cut}}$ for each rod length, which is the length of the first piece to be cut off in an optimal solution. These optimal first cuts are stored in an extra array, which is used to reconstruct the optimal solution after the maximum revenue has been computed. The running time of this approach is also in $\mathcal{O}(n^2)$, since the extra array is updated in the same way as the revenue table, and the optimal solution is reconstructed in $\mathcal{O}(n)$ time after the total revenue has been computed.

Implementation: [Extended bottom-up DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod4.c)
