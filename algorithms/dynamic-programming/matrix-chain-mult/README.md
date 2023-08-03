# Matrix Chain Multiplication (MCM)

## Problem

Given a sequence (chain) of matrices, which are not necessarily square, find the most efficient way to multiply these matrices together. The problem is not to actually perform the matrix multiplications, but merely to determine in which order to perform them so as to minimize the total number of scalar multiplications.  
In other words, try to find the optimal parenthesization of the matrix product. A matrix product is associative, so the order in which the products are performed does not affect the result. However, the chosen order may dramatically affect the number of scalar multiplications needed to compute that result.

This is visible from the code snippet below, where A is a $a \times b$ matrix, B is a $b \times p$ matrix, and P is an $a \times p$ matrix:

```c
void matrixProduct(int A[], int B[], int P[], int a, int b, int p){
  for (i = 0; i < a; i++){
    for (j = 0; j < b; j++){
      P[i][j] = 0;
      for (k = 0; k < p; k++)
        P[i][j] = P[i][j] + A[i][k] * B[k][j];
    }
  }
}
```

The running time is dominated by the innermost loop, which is executed $p \cdot a \cdot b$ times. If we need to multiply $n$ matrices, then the total number becomes $p_1\cdot a_1 \cdot b_1 + p_2 \cdot a_2 \cdot b_2 + \dots + p_n \cdot a_n \cdot b_n$. How can we minimize this number?

The overall idea is similar to the one behind the rod cutting problem: while in rod cutting we tried to find the optimal cuts for a rod of length $n$ in order to maximize the total revenue, here we try to find the optimal points $k$ to split the chain of matrices into subchains so as to minimize the number of scalar multiplications needed to compute the product of the entire chain. The optimal solution for a chain $A_i \dots A_j$ of length $j - i + 1$ is then the optimal solution for a chain of length $k - i + 1$ followed by the optimal solution for a chain of length $j - k$, where $i\leq k < j$ is the optimal split point, and the cost is then the sum of the costs for each of the two subchains plus the cost of multiplying the two subchains together. To find the optimal split point, i.e. the one that minimizes the cost, we need to try all possible values of $k$ and find the one that minimizes the cost for a given chain. In contrast to the rod cutting problem, the subproblems may vary at both ends of the chain: both $i$ and $j$ need to vary in order to be able to optimally parenthesize the chain $A_i \dots A_j$. In the rod cutting problem only the length varied and the price was the same for all rods of the same length, whereas here the cost of multiplying two matrices depends on their dimensions.

Thus, given a chain of matrices $A_1 \dots A_n$, the cost of parenthesizing it optimally is given by the following recurrence:

$$
m[i,j] = \begin{cases}
0 & \text{if } i = j \\
\min \lbrace m[i,k] + m[k+1,j] + p_{i-1} \cdot p_k \cdot p_j : i \leq k < j\rbrace & \text{if } i < j
\end{cases}
$$

where $m[i,j]$ is the minimum number of scalar multiplications needed to compute the product of the matrices $A_i \dots A_j$, and $p_i$ is the dimension of the $i$-th matrix, i.e. $A_i$ is a $p_{i-1} \times p_i$ matrix.

## Brute Force Solution

The brute force solution tackles the problem by trying all possible parenthesizations, whilst ignoring the fact that the same subproblem may be encountered multiple times during its recursive calls. The number of possible parenthesizations is in $\Omega(2^n)$, where $n$ is the number of matrices. The running time of this approach is thus in $\Omega(2^n)$.

Implementation: [MCM - Brute Force](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/matrix-chain-mult/mcm-1.c)

## Top-Down Approach with Memoization

This approach seeks to preserve the top-down strategy of the brute force solution, while making use of memoization by maintaining a table with subproblem solutions in order to avoid recomputing the same subproblems over and over again. This strategy improves the running time to $O(n^3)$.  

Implementation: [MCM - Top-down DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/matrix-chain-mult/mcm-2.c)

## Bottom-up Approach

The bottom-up approach is similar, but it starts from the base cases and gradually builds up the solution to the original problem. Subproblems are thus solved in order of increasing chain length, so that at each step all subsolutions needed to solve the current subproblem are readily availabe. This is achieved by maintaining a table, where the solution to each subproblem—its minimal cost in terms of scalar multiplications—is stored in a bottom-up fashion. The solution to the original problem is then the solution to the last subproblem in the table.  
If we also want to return an optimal parenthesization, and not only the minimal cost, we need to keep track of the optimal split point for each subproblem. We do this by maintaining an extra table for each choice of an optimal split point k, which is then used to reconstruct the optimal parenthesization. The running time of this approach is $O(n^3)$.

Implementation: [MCM - Bottom-up DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/matrix-chain-mult/mcm-3.c)
