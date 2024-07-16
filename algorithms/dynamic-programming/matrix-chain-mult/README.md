${\color{Cadetblue}\text{\huge Matrix Chain}}$ ${\color{Cadetblue}\text{\huge Multiplication}}$ ${\color{Cadetblue}\text{\huge (MCM)}}$

<br />

${\color{rosybrown}\text{\Large Problem}}$

Given a sequence (chain) of matrices, which are not necessarily square, find the most efficient way to multiply these matrices together. The aim is not to actually perform the matrix multiplications, but rather to determine in which order to perform them so as to minimize the total number of scalar multiplications and thus minimize the total cost of the operation.  

In other words, try to find the ${\color{peru}\text{optimal}}$ ${\color{peru}\text{parenthesization}}$ of the matrix product. A matrix product is associative, so the order in which the products are performed does not affect the final result. However, the chosen order may dramatically affect the total number of scalar multiplications required to obtain that result and thus affect the overall cost of the operation.  

As an example, consider the chain of matrices $A_1, A_2, A_3$ with dimensions $10 \times 100$, $100 \times 5$, and $5 \times 50$, respectively. The number of scalar multiplications needed to compute the product $(A_1 A_2) A_3$ is $10 \times 100 \times 5 + 10 \times 5 \times 50 = 7500$. However, if we parenthesize the product as $A_1 (A_2 A_3)$, the number of scalar multiplications needed is $10 \times 100 \times 50 + 100 \times 5 \times 50 = 75000$, which is ten times as many as in the first case. So, the goal we are trying to achieve in every given chain of matrices is to find the parenthesization that ${\color{peru}\text{minimizes}}$ ${\color{peru}\text{the cost}}$ of the operation, i.e. minimizes the total number of scalar multiplications required to compute the product of the entire chain.

<br />

${\color{darkseagreen}\text{\Large The key idea}}$

The overall idea is very similar to the one behind the [rod cutting problem](https://github.com/pl3onasm/CLRS/tree/main/algorithms/dynamic-programming/rod-cutting): while in that problem we tried to find the optimal cuts for a rod of length $n$ in order to maximize the total revenue, here we try to find the optimal points $k$ to split the matrix chain $A_1 \dots A_n$ into subchains (marked by parentheses) so as to minimize the overall cost to compute the product of the entire chain.  

Given any such subchain of matrices $A_i \dots A_j$, we can find its optimal parenthesization by considering all possible split points $k$ between $i$ and $j$ and then choosing the one that minimizes the total cost. We then recursively apply this strategy to each of the two resulting subchains, until we reach the base case of a chain consisting of only one matrix. This way of working is possible because the problem exhibits the property of ${\color{peru}\text{optimal substructure}}$: the optimal solution to the original problem can be found by combining the optimal solutions to the subproblems.

In other words, the optimal solution for a chain $A_i \dots A_j$ of length $j - i + 1$ is the optimal solution for a chain of length $k - i + 1$ followed by the optimal solution for a chain of length $j - k$, where $i\leq k < j$ is the optimal split point. The total cost to compute the product of the chain is then given by the sum of the costs for each of the two subchains plus the cost of multiplying the two subchains together.  

Thus, for a subchain of matrices $A_i \dots A_j$, the cost of parenthesizing it optimally is given by the following ${\color{peru}\text{recurrence}}$:
<br />

$$
\color{darkslateblue}\huge\boxed{\color{rosybrown}\small \space
m[i,j] =
\begin{cases}
0 & \scriptsize \text{if } i = j  \\
\min \lbrace m[i,k] + m[k+1,j] \\
  \space \space  +\space p_{i-1} \cdot p_k \cdot p_j : i \leq k < j\rbrace& \scriptsize \text{if } i < j  \\
\end{cases}\space}
$$

<br />

where $m[i,j]$ is the minimum number of scalar multiplications needed to compute the product of the matrices $A_i \dots A_j$, and $p_i$ is the column dimension of the $i$-th matrix in the chain, i.e. $A_i$ is a $p_{i-1} \times p_i$ matrix, with $p_0$ being the row dimension of the first matrix, and $p_n$ being the column dimension of the last matrix.  

The base case occurs when the chain consists of only one matrix, in which case the cost is zero. From the recurrence, it is also clear that in order to compute the optimal solution for a chain of matrices $A_i \dots A_j$, we need to compute the optimal solutions for all subchains $A_i \dots A_k$ and $A_{k+1} \dots A_j$ for all possible values of $k$ between $i$ and $j$. In the end, the solution to the original problem is then the solution to the subproblem $m[1,n]$, which is the optimal cost to compute the product of the entire chain $A_1 \dots A_n$.  

In contrast to the rod cutting problem, subproblems may vary at both ends of the chain: both $i$ and $j$ need to vary in order to be able to find an optimal parenthesization for the chain $A_1 \dots A_n$. This is why the DP solution to the matrix chain multiplication problem is more complex than the one to the rod cutting problem, and why we need a two-dimensional table to store the solutions to the subproblems instead of a one-dimensional array as in the rod cutting problem.  

<br />

${\color{darkseagreen}\text{\Large Brute force}}$

The brute force solution tackles the problem by implementing the above recurrence directly without any optimization. It just tries all possible split points at each step, and recursively applies the same strategy to each of the two resulting subchains, whilst ignoring the fact that identical subproblems are encountered multiple times during the recursive calls. This leads to an exponential running time, as the same subproblems are recomputed over and over again. The brute force solution is therefore not practical for large instances of the problem, as it is extremely slow.

Implementation: [MCM - Brute Force](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/matrix-chain-mult/mcm-1.c)

<br />

${\color{darkseagreen}\text{\Large Top-down}}$

This approach seeks to preserve the top-down strategy of the ${\color{peru}\text{recursive}}$ brute force solution, but also to avoid recomputing the same subproblems over and over again. It does this by storing the solutions to the subproblems in a table as soon as they are computed, and checking this table to see if the solution is already available before computing anything. This strategy is called ${\color{peru}\text{memoization}}$: we keep a memo of each solution to a subproblem, and use this memo instead of recomputing the solution again when we encounter the same subproblem. Using this approach, the running time of the algorithm drops to $\mathcal{O}(n^3)$. Note the difference in the running time of the DP solution to the rod cutting problem, which is $\mathcal{O}(n^2)$: this is because the matrix chain multiplication problem has two varying indices, $i$ and $j$, which need to be considered in order to find an optimal solution.

Implementation: [MCM - Top-down DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/matrix-chain-mult/mcm-2.c)

<br />

${\color{darkseagreen}\text{\Large Bottom-up}}$

The bottom-up approach is similar to the top-down approach, but does not use recursion, and works the other way around: it does not start with the original problem and recursively breaks it down into smaller subproblems until it reaches the base case, but it actually starts from the base cases and gradually builds up from there to the original problem. The subproblems are thus solved in order of ${\color{peru}\text{increasing}}$ ${\color{peru}\text{subchain length}}$, so that at each step all subsolutions needed to solve the current subproblem are readily availabe. This is achieved by maintaining a table, where the solution to each subproblem—its minimal cost in terms of scalar multiplications—is stored in a bottom-up fashion. The solution to the original problem is then the solution to the last subproblem in the table. The running time of this approach is also in $\mathcal{O}(n^3)$.

If we also want to return an optimal parenthesization, and not only the minimal cost, we need to keep track of the optimal split point for each subproblem. We can do this by maintaining an extra table for each choice of an optimal split point $k$, which is then used to reconstruct the optimal parenthesization after the minimal cost has been computed. Note that such an optimal parenthesization is not necessarily unique, but the total cost of the chain multiplication will be the same for all optimal parenthesizations.

Implementation: [MCM - Bottom-up DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/matrix-chain-mult/mcm-3.c)
