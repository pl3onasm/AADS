$\huge{\color{Cadetblue}\text{Rod cutting}}$

<br/>

$\Large{\color{rosybrown}\text{Problem}}$

Given a rod of length $n$ meters and a table $P$ of prices for rod lengths from $0$ to $m$ meters, where $m \geq n$, determine the maximum revenue obtainable by cutting up the rod and selling the pieces. Note that if the price $P[n]$ for a rod of length $n$ is large enough, an optimal solution may require no cutting at all.

<br/>

$\Large{\color{darkseagreen}\text{The key idea}}$

The idea is to cut the rod into two pieces, and then solve the problem for each of the two pieces. The optimal solution for the original rod is then the sum of the optimal solutions for the two pieces. In other words, the optimal solution for a rod of length $n$ is the maximum of the following two values:

1. The price $P[n]$ for a rod of length $n$  
2. The sum of the optimal solutions (maximum revenues) for the two pieces of lengths $i$ and $n - i$ obtained by cutting the rod into two pieces of lengths $i$ and $n - i$, respectively, where $1 \leq i \leq n$

A slightly different and easier way of approaching the problem is to think of each subproblem as the combination of an uncut piece of length $i$ and the optimal solution for the remaining piece of length $n-i$, a ${\color{peru}\text{suffix}}$ of the original rod, which becomes our new subproblem, the new rod to be cut. The optimal solution (maximum revenue) for the original rod is then the maximum sum of all optimal solutions for the subproblems of lengths $1, 2, ..., n-1$.

Clearly, the subproblems are ${\color{peru}\text{overlapping}}$, since the same subproblem of finding the optimal cuts for a particular rod length is sure to be encountered multiple times during the recursive calls. They also show an ${\color{peru}\text{optimal substructure}}$, since the optimal solution for the original rod can be constructed from the optimal solutions for the subproblems: the optimal solution for a rod of length $n$ is the maximum sum of the optimal solutions for the subproblems of lengths $1, 2, ..., n-1$.

Thus, we can define a function $r(n)$ that yields the maximum revenue obtainable from a rod of length $n$ as follows:

$$
\color{darkslateblue}\boxed{\color{rosybrown}\space
r(n) = \begin{cases}
0 & \scriptsize \text{if } n = 0 \\
\text{max} \lbrace P[i] + r(n-i)  & \scriptsize \text{otherwise}\\
\qquad \space : 1 \leq i \leq n \rbrace
\end{cases}\space}
$$

<br/>

where $P[i]$ is the price for an uncut piece of length $i$, readily available from the table of prices, and $r(n-i)$ represents the recursive call to the function, which yields the maximum revenue obtainable from a rod of length $n-i$. The base case occurs when the rod length is $0$, in which case the revenue is $0$, since the maximum revenue obtainable from a rod of length $0$ is $0$.

<br/>

$\Large{\color{darkseagreen}\text{Brute force}}$

The brute force approach simply implements the recursive formula above as is, without any optimization. Of course, this approach is very inefficient, since it completely ignores the overlapping nature of the subproblems, meaning that the same subproblem of finding the optimal cuts for a particular rod length recurs multiple times during the recursive calls.

The running time of this approach is in $\mathcal{O}(2^n)$, and it is easy to see why: each possible solution is a permutation of cut decisions, and there are $2^{n-1}$ such permutations, all of which need to be checked.  

Implementation: [Naive recursive solution](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod1.c)

<br/>

$\Large{\color{darkseagreen}\text{Top-down}}$

As said, the brute force approach ignores the fact that to compute the optimal solution for rods of different lengths, we repeatedly need the optimal solutions for the same smaller rod lengths, and so it ends up recomputing the same subproblems multiple times. This flaw can be repaired, however, by storing the optimal solution for each rod length in a table as soon as it is computed, and checking this table to see if the solution is already available before computing anything. This strategy is called ${\color{peru}\text{memoization}}$: we literally keep a memo of each solution to a subproblem, and use this memo instead of recomputing the solution again when we need it. Obviously, this saves a lot of time, since we only compute each subproblem once, resulting in a dramatic drop to an overall running time of $\mathcal{O}(n^2)$. The approach is called ${\color{peru}\text{top-down}}$ because we start with the problem we want to solve and recursively break it down into smaller subproblems until we reach the base case, which is the smallest subproblem that can be solved directly.

Implementation: [Top-down DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod2.c)

<br/>

$\Large{\color{darkseagreen}\text{Bottom-up}}$

The bottom-up DP approach also uses a table to store the optimal solutions for subproblems, but it does not use recursion. Instead, subproblems are solved iteratively, starting with the smallest subproblems and working up to the original problem, while storing the solution to each subproblem when it is first solved. Thus, the maximum revenues for subproblems of size $1, 2, ..., n$ are computed in order of increasing piece size, in a ${\color{peru}\text{bottom-up}}$ fashion, ensuring that all required partial solutions are readily available to solve the next subproblem in line. The running time of this approach is also in $\mathcal{O}(n^2)$.  

Implementation: [Bottom-up DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod3.c)

<br/>

$\Large{\color{darkseagreen}\text{Extended bottom-up}}$

The extended bottom-up approach not only computes the maximum revenue, but also an optimal solution: a list of piece sizes that together yield the maximum revenue. This is done by keeping track of the ${\color{peru}\text{optimal first cut}}$ for each rod length, which is the length of the first piece to be cut off in an optimal solution. These optimal first cuts are stored in an extra array, which is used to reconstruct the optimal solution after the maximum revenue has been computed. The running time of this approach is also in $\mathcal{O}(n^2)$, since the extra array that stores the optimal first cuts is updated in the same way as the revenue table. An optimal solution is reconstructed in $\mathcal{O}(n)$ time after the total revenue has been computed.

Implementation: [Extended bottom-up DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/rod-cutting/cut-rod4.c)
