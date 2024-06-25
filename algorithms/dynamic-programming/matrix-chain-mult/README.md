# ${\color{Cadetblue}\text{Matrix Chain}}$ ${\color{Cadetblue}\text{Multiplication}}$ ${\color{Cadetblue}\text{(MCM)}}$

## ${\color{rosybrown}\text{Problem}}$

Given a sequence (chain) of matrices, which are not necessarily square, find the most efficient way to multiply these matrices together. The aim is not to actually perform the matrix multiplications, but merely to determine in which order to perform them so as to minimize the total number of scalar multiplications and minimize the total cost of the operation.
In other words, try to find the ${\color{peru}\text{optimal}}$ ${\color{peru}\text{parenthesization}}$ of the matrix product. A matrix product is associative, so the order in which the products are performed does not affect the result. However, the chosen order may dramatically affect the number of scalar multiplications required to obtain that result.  

As an example, consider the chain of matrices $A_1, A_2, A_3$ with dimensions $10 \times 100$, $100 \times 5$, and $5 \times 50$, respectively. The number of scalar multiplications needed to compute the product $(A_1A_2)A_3$ is $10 \times 100 \times 5 + 10 \times 5 \times 50 = 7500$. However, if we parenthesize the product as $A_1(A_2A_3)$, the number of scalar multiplications needed is $10 \times 100 \times 50 + 100 \times 5 \times 50 = 75000$, which is ten times more. The goal is to find the parenthesization that ${\color{peru}\text{minimizes}}$ ${\color{peru}\text{the cost}}$ of the operation, i.e. the total number of scalar multiplications required to compute the product of the entire chain.

## ${\color{darkseagreen}\text{The key idea}}$

The overall idea is very similar to the one behind the [rod cutting problem](https://github.com/pl3onasm/CLRS/tree/main/algorithms/dynamic-programming/rod-cutting): while in that problem we tried to find the optimal cuts for a rod of length $n$ in order to maximize the total revenue, here we try to find the optimal points $k$ to split the matrix chain $A_1 \dots A_n$ into subchains (marked by parentheses) so as to minimize the overall cost to compute the product of the entire chain.  

Given any such subchain of matrices $A_i \dots A_j$, the optimal solution is to split the chain into two subchains at some optimal point $k$ and then compute the optimal solutions for each of the two resulting subchains. In other words, the optimal solution for a chain $A_i \dots A_j$ of length $j - i + 1$ is the optimal solution for a chain of length $k - i + 1$ followed by the optimal solution for a chain of length $j - k$, where $i\leq k < j$ is the optimal split point, and the total cost to compute the product of the chain is then given by the sum of the costs for each of the two subchains plus the cost of multiplying the two subchains together.  

Thus, for a subchain of matrices $A_i \dots A_j$, the cost of parenthesizing it optimally is given by the following ${\color{peru}\text{recurrence}}$:

$$
m[i,j] = \begin{cases}
\text{if } i = j: 0  \\
\text{if } i < j :\min \lbrace m[i,k] + m[k+1,j] \\
  \qquad \quad +\space p_{i-1} \cdot p_k \cdot p_j : i \leq k < j\rbrace \\
\end{cases}
$$

where $m[i,j]$ is the minimum number of scalar multiplications needed to compute the product of the matrices $A_i \dots A_j$, and $p_i$ is the dimension of the $i$-th matrix, i.e. $A_i$ is a $p_{i-1} \times p_i$ matrix. The base case is when the chain consists of only one matrix, in which case the cost is zero. From the recurrence, it is also clear that in order to compute the optimal solution for a chain of matrices $A_i \dots A_j$, we need to compute the optimal solutions for all subchains $A_i \dots A_k$ and $A_{k+1} \dots A_j$ for all possible values of $k$ between $i$ and $j$. In the end, the solution to the original problem is the solution to the subproblem $m[1,n]$, which is the optimal cost to compute the product of the entire chain $A_1 \dots A_n$.  

In contrast to the rod cutting problem, subproblems may vary at both ends of the chain: both $i$ and $j$ need to vary in order to be able to find an optimal parenthesization for the chain $A_1 \dots A_n$. This is why the DP solution to the matrix chain multiplication problem is more complex than the one to the rod cutting problem, and why we need a two-dimensional table to store the solutions to the subproblems instead of a one-dimensional array as in the rod cutting problem.  

## ${\color{darkseagreen}\text{Brute force}}$

The brute force solution tackles the problem by trying all possible parenthesizations, whilst ignoring the fact that identical subproblems are encountered multiple times during the recursive calls. The total number of possible parenthesizations is in $\Omega(2^n)$, and so the running time of this approach is also in $\Omega(2^n)$.

Implementation: [MCM - Brute Force](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/matrix-chain-mult/mcm-1.c)

## ${\color{darkseagreen}\text{Top-down}}$

This approach seeks to preserve the top-down strategy of the ${\color{peru}\text{recursive}}$ brute force solution, but also to avoid recomputing the same subproblems over and over again. It does this by storing the solutions to the subproblems in a table as soon as they are computed, and checking this table to see if the solution is already available before computing anything. This strategy is called ${\color{peru}\text{memoization}}$: we keep a memo of each solution to a subproblem, and use this memo instead of recomputing the solution again when we encounter the same subproblem. Using this approach, the running time of the algorithm drops to $\mathcal{O}(n^3)$. Note the difference in the running time of the DP solution to the rod cutting problem, which is $\mathcal{O}(n^2)$: this is because the matrix chain multiplication problem has two varying indices, $i$ and $j$, which need to be considered in order to find an optimal solution.

Implementation: [MCM - Top-down DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/matrix-chain-mult/mcm-2.c)

## ${\color{darkseagreen}\text{Bottom-up}}$

The bottom-up approach is similar to the top-down approach, but does not use recursion, and works the other way around: it does not start with the original problem and recursively breaks it down into smaller subproblems until it reaches the base case, but it actually starts from the base cases and gradually builds up from there to the original problem. The subproblems are thus solved in order of ${\color{peru}\text{increasing}}$ ${\color{peru}\text{subchain length}}$, so that at each step all subsolutions needed to solve the current subproblem are readily availabe. This is achieved by maintaining a table, where the solution to each subproblem—its minimal cost in terms of scalar multiplications—is stored in a bottom-up fashion. The solution to the original problem is then the solution to the last subproblem in the table. The running time of this approach is also in $\mathcal{O}(n^3)$.

If we also want to return an optimal parenthesization, and not only the minimal cost, we need to keep track of the optimal split point for each subproblem. We can do this by maintaining an extra table for each choice of an optimal split point k, which is then used to reconstruct the optimal parenthesization after the minimal cost has been computed. Note that such an optimal parenthesization is not necessarily unique, but the total cost of the operation is.

Implementation: [MCM - Bottom-up DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/matrix-chain-mult/mcm-3.c)
