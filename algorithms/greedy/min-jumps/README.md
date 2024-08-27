$\huge{\color{Cadetblue}\text{Minimum number}}$  
$\huge{\color{Cadetblue}\text{of jumps}}$

<br />

${\Large\color{rosybrown}\text{Problem}}$

Given is an array of non-negative integers where each element represents the maximum number of steps that can be made forward from that position. For example, an element of value 3 at index 2 means that we can jump to any of the indices 3, 4, or 5, starting from index 2. An element of value 0, on the other hand, means that we cannot move any further from that position.

Starting from the first position in the array, our task is to reach the last position by taking a minimum number of jumps. Of course, it may very well be the case that we cannot reach the last position at all, in which case we return $\infty$.

<br />

${\Large\color{darkseagreen}\text{Brute Force}}$

The brute force approach is to consider all possible jumps from each position and recursively find the minimum number of jumps required to reach the last position. In other words, starting from the first position, we just keep taking all possible jumps for each new position until we reach our destination. In the end, we take the minimum of all the possible paths that took us to the last position.

We can express this approach in the form of a recursive function $M(i)$, which returns the minimum number of jumps required to reach the last position, starting from position $i$ in the array $A$ of length $n$. This function can be defined as follows:

$$\color{darkslateblue}\boxed{\color{rosybrown}\space
\text{M}(i) = \begin{cases}
\infty & \scriptsize \text{if } A[i] = 0 \\
 & \scriptsize \space \land \space i < n - 1 \\
0 & \scriptsize \text{if } i \geq n - 1 \\
1 + \text{min} \lbrace \text{M}(i + j) & \scriptsize  \text{otherwise}\\
\qquad \qquad  : 1 \leq j \leq A[i] \rbrace
\end{cases}\space}
$$

<br/>

The base cases occur when we are already at the last position or when we cannot move any further from the current position. In the latter case, we return $\infty$ as it is not possible to reach the last position from the current position. Otherwise, we take the minimum of all the possible jumps from the current position and add 1 to it to account for the current jump. The minimum number of jumps required to reach the last position from the first position is then given by $\text{M}(0)$.

Implementing this recursive function as is, however, will result in an exponential time complexity of $O(2^n)$, where $n$ is the length of the array. This is because a lot of subproblems overlap, and we end up solving the same subproblems multiple times.

Implementation: [Min Jumps - Brute Force](https://github.com/pl3onasm/CLRS/tree/main/algorithms/greedy/min-jumps/mjp-1.c)

<br />

${\Large\color{darkseagreen}\text{DP approach}}$

We can improve the time complexity of the above approach by using dynamic programming. Opting for a bottom-up approach, we start from the base cases by initializing a memoization table of size $n$ with its first element set to 0 and the rest of the elements set to $\infty$. We then iterate over the array from left to right, updating the cells as we go along using the recursive formula defined above. The final answer is then given by the last element of the memoization table.

Running time complexity of this approach is $O(n^2)$, where $n$ is the length of the array. If we also want to reconstruct the path, we can keep track of the indices that led to the minimum number of jumps at each position by storing those indices in a separate array and then reconstruct the path by backtracking from the last position to the first position.

Implementation: [Min Jumps - DP](https://github.com/pl3onasm/CLRS/tree/main/algorithms/greedy/min-jumps/mjp-2.c)

<br />

${\Large\color{darkseagreen}\text{Greedy approach}}$

We can further improve the time complexity of the above approach by using a greedy strategy. The idea is to keep taking the jump that lands us on the position that can take us the farthest ahead, thereby minimizing the number of jumps required to reach the last position. This means that for each position we are at, we always take the jump that leads us to the cell with the maximum value that we can reach from that position.

So, to make this more concrete, we start at the first position in the input array and first check to see which cell from the ones that we can reach from this position has the maximum value. After finding this cell, we update our current position to this cell and repeat the process for the new position we are at by first finding the cell with the maximum value that we can reach from our new position and then taking the jump to that cell. We keep doing this until we reach the last position, and if at any point we reach a position where we cannot move any further, we return $\infty$ as the last position cannot be reached.

The time complexity of this approach is $O(n)$, where $n$ is the length of the array. This is because we can suffice with a single pass through the array to find the minimum number of jumps required to reach the last position.

Implementation: [Min Jumps - Greedy](https://github.com/pl3onasm/CLRS/tree/main/algorithms/greedy/min-jumps/mjp-3.c)