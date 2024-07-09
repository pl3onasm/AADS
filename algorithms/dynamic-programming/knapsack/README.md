# ${\color{Cadetblue}\text{0-1 Knapsack}}$ ${\color{Cadetblue}\text{problem}}$

## ${\color{rosybrown}\text{Problem}}$

We are given a knapsack of a given capacity $W$ and a set of $n$ items. Each item has a certain weight and value. The aim is to fill the knapsack with items such that the total weight is less than or equal to the capacity of the knapsack and the total value is maximized. The problem is called 0-1 knapsack because for each item, we can either choose to include it entirely in the knapsack or exclude it. It is not allowed to include a fraction of an item or include an item more than once.

## ${\color{darkseagreen}\text{The key idea}}$

The key idea is to think of each subproblem in terms of a ${\color{peru}\text{prefix}}$ of the item list and a remaining knapsack capacity $j \leq W$. At each step, we can choose to include the last item of the prefix in the knapsack if it fits or exclude it. If inclusion is possible, we need to consider the maximum value between including and excluding the item. Of course, if the item does not fit, we can only exclude it. The value of the subproblem is the maximum value we can get from the prefix of the item list and the remaining knapsack capacity.  

Clearly, the subproblems are ${\color{peru}\text{overlapping}}$ because we can have the same prefix of the item list and the same remaining knapsack capacity in multiple subproblems. The subproblems also have the ${\color{peru}\text{optimal substructure}}$ property because the optimal solution to a subproblem can be obtained by combining the optimal solutions to its own subproblems.

A recursive solution to the problem can be found by defining a function $V(i,j)$ that returns the maximum value that can be obtained from the prefix with last item having index $i$ and a knapsack capacity of $j$. The function can be defined as follows:
<br />

$$
\color{darkslateblue}\huge\boxed{\color{rosybrown}\normalsize \space
\normalsize V(i,j) = \begin{cases} 0 & \scriptsize \text{if } i = \infty \\
\scriptsize & \scriptsize \space \space \lor \space j = 0\\
\normalsize \text{max}\lbrace V(i-1,j), & \scriptsize \text{if } j \leq w_i \\
\normalsize \quad \space v_i + V(i-1,j-w_i) \rbrace  \\
\normalsize V(i-1,j) &\scriptsize \text{otherwise}
\end{cases}\space}
$$

<br />

The value of the original problem is obtained by calling $V(n-1,W)$. The base case occurs when we have no items left ($i = \infty$) or the knapsack capacity is zero. In this case, the value of the subproblem is zero. If we choose to include the last item in the knapsack, , we update the optimal solution of the previous subproblem by subracting its weight $w_i$ from the remaining capacity $j$ and adding its value $v_i$ to the total value. Excluding the item means the total value and the remaining capacity found in the previous subproblem remain unchanged.

## ${\color{darkseagreen}\text{Brute force}}$

The brute force solution implements the recursive function $V(i,j)$ as described above without any optimization. It ignores the fact that the subproblems overlap and recomputes the value of the same subproblem multiple times. The time complexity of the brute force solution is therefore exponential.

Implementation: [Knapsack - Brute Force](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/knapsack/knapsack-1.c)

## ${\color{darkseagreen}\text{Top-down}}$

The top-down solution implements the same recursive function $V(i,j)$ but uses a ${\color{peru}\text{memoization}}$ ${\color{peru}\text{table}}$ to store the value of each subproblem once it is computed. After initializing this table with a special value that indicates the subproblem has not been computed yet (e.g. -1), the program is modified to check at each recursive step if the value of the subproblem has already been computed. If it has, the stored value is returned. Otherwise, the value is computed as before and stored in the table. As each suproblem is thus guaranteed to be computed only once, the time complexity of the top-down solution is reduced to $\mathcal{O}(nW)$.

Implementation: [Knapsack - Top-down DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/knapsack/knapsack-2.c)

## ${\color{darkseagreen}\text{Bottom-up}}$

The bottom-up solution is based on the same idea as the top-down solution but computes the values of the subproblems from the smallest to the largest, starting from the base case. The benefit of this approach is that all the partial solutions needed to compute the value of a subproblem are already computed and available when the subproblem is considered, so there is no need to check if the value of the subproblem has already been computed. It also avoids the overhead of recursive calls. The time complexity of the bottom-up approach, however, is the same as the top-down approach, $\mathcal{O}(nW)$.

${\color{peru}\text{Reconstruction}}$ of an optimal solution from the memoization table is also straightforward. We start from the last item and the last knapsack capacity and then keep moving to the previous item and the previous knapsack capacity. At each step, we check if the value of the subproblem is the same as the value of the subproblem without the last item. If it is, we know the last item was not included in the knapsack. Otherwise, we include the item and subtract its weight from the remaining capacity. The process is repeated until we reach the first item.

Implementation: [Knapsack - Bottom-up DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/knapsack/knapsack-3.c)
