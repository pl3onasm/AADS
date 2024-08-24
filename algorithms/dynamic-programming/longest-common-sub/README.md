${\color{Cadetblue}\text{\huge Longest Common}}$  
${\color{Cadetblue}\text{\huge Subsequence (LCS)}}$

<br />

$\Large{\color{rosybrown}\text{Problem}}$

Given two sequences $X_m = \langle x_0, x_1, ..., x_{m-1} \rangle$ and $Y_n = \langle y_0, y_1, ..., y_{n-1} \rangle$, find the length of their longest common subsequence. A ${\color{peru}\text{subsequence}}$ is derived from a given sequence by deleting some elements from it (possibly none) without changing the order of the remaining elements. For example, the sequence $\langle A, B, D \rangle$ is a subsequence of $\langle A, B, C, D, B, A \rangle$, but $\langle B, D, A, B \rangle$ is not, since the order of the elements is not preserved.  

However, we do not care about just any subsequence, but about the longest one that is present in both sequences $X$ and $Y$. In other words, the problem is to find the maximum length of a subsequence $Z_k = \langle z_0, z_1, ..., z_{k-1} \rangle$ such that $z_i = x_j$ and $z_i = y_h$ for each $0 \leq i < k$ and for some $0 \leq j < m$ and $0 \leq h < n$.  

For example, the longest common subsequence of the sequences $X = \langle A, B, C, B, D, A, B \rangle$ and $Y = \langle B, D, C, A, B, A \rangle$ is $Z = \langle B, C, A, B \rangle$, which has length $4$. Mind that the longest common subsequence is not necessarily unique: $Z = \langle B, D, A, B \rangle$ and $Z = \langle B, C, B, A \rangle$ are also valid solutions. Its length of course is unique, and in this case it is $4$.

<br />

$\Large{\color{darkseagreen}\text{The key idea}}$

The main idea is to think of the subproblems in terms of ${\color{peru}\text{prefixes}}$ of the input sequences $X$ and $Y$: the length of the LCS of $X$ and $Y$ can be found by considering the LCS of the prefixes $X_{m-1}$ and $Y_{n-1}$, and then adding $1$ to the LCS length if $X[m-1] = Y[n-1]$. If they are not equal, we need to consider two subproblems: finding the LCS of $X_{m-1}$ and $Y$, and finding the LCS of $X$ and $Y_{n-1}$, so that we can take the maximum of the two solutions. Clearly, these subproblems overlap and also exhibit the property of ${\color{peru}\text{optimal substructure}}$: the optimal solution to any of them can be found by combining the optimal solutions to their subproblems.  

As we keep repeating the process for the remaining elements of $X$ and $Y$, we continually reduce the size of the problem to smaller subproblems, i.e. to smaller prefixes of $X$ and $Y$. Eventually, we will reach the base case of an empty sequence, when we reach the beginning of one of the original sequences, at which point the maximum length of the LCS is fully computed.  

This process can be formulated as a recurrence, which defines the length of the LCS in terms of the *lengths* $m$ and $n$ of the prefixes of $X$ and $Y$ respectively:
<br />

$$
\color{darkslateblue}\boxed{\color{rosybrown}\space
L(m,n) = \begin{cases}
0 & \scriptsize \text{if } m = 0 \\
& \scriptsize \space \space \lor \space n = 0 \\
L(m-1, n-1) + 1 & \scriptsize \text{if } X[m-1] \\
& \scriptsize \space \space = Y[n-1] \\
\text{max}\lbrace L(m-1, n), & \scriptsize \text{otherwise } \\
\qquad \space  L(m, n-1) \rbrace
\end{cases}\space}
$$

<br />

$\Large{\color{darkseagreen}\text{Brute force}}$

The brute force solution simply implements the recurrence without any optimization, and ignores the fact that identical subproblems (finding longest common subsequences for the same prefixes of $X$ and $Y$) are encountered multiple times during the recursive calls. This leads to an exponential running time, as the same subproblems are recomputed over and over again. The running time of this approach is in $\mathcal{O}(2^{m+n})$.

Implementation: [LCS - Brute Force](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-common-sub/lcs-1.c)

<br />

$\Large{\color{darkseagreen}\text{Top-down}}$

The top-down DP approach fixes the flaw of the brute force solution by using a ${\color{peru}\text{memoization table}}$ to avoid recomputing the LCS for the same prefixes of $X$ and $Y$ multiple times. This memoization table stores the length of the LCS for each pair of prefixes of $X$ and $Y$ as soon as it is computed, so that it can be used again when encountering the same subproblem. The space complexity of the table is in $\mathcal{O}(mn)$, since it has $m \cdot n$ entries: one for each possible pair of prefixes of $X$ and $Y$, plus two additional rows and columns filled with zeros for the base cases of empty sequences. The final solution is then the value stored in the memoization table for the pair $X$ and $Y$ itself which is the entry found at the last row and last column of the table. As the memoization table ensures that each subproblem is computed only once, the running time of the algorithm is able to drop from exponential to $\mathcal{O}(mn)$.  

Implementation: [LCS - Top-Down DP](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-common-sub/lcs-3.c)

<br />

$\Large{\color{darkseagreen}\text{Bottom-up}}$

The bottom-up DP approach also uses a memoization table, but does not use recursion. Instead, subproblems are solved iteratively, starting with the base case of a pair of empty sequences, and computing the LCS for all possible pairs of prefixes of $X$ and $Y$ in a bottom-up fashion, until we reach the original problem of finding the LCS for the pair $X$ and $Y$ itself. The running time of this approach is also in $\mathcal{O}(mn)$.

${\color{peru}\text{Reconstruction}}$ of an actual LCS from the memoization table is also possible. We can do this by starting at the last row and last column of the table, and moving backwards through the table, literally tracing the steps of the recurrence that was used to fill the table in the first place. The idea is as follows: if the value in the current cell is equal to the value in the cell above or the cell to the left, we move to that cell, otherwise we add the corresponding element from $X$ or $Y$ to the LCS and move diagonally up and to the left. We continue this process until we reach the first row or the first column of the table, at which point an actual LCS has been reconstructed in reverse order and in $\mathcal{O}(m+n)$ time.  

As multiple LCSs are possible, this process does not necessarily yield a unique solution. If we want to return all possible LCSs, we can use a recursive backtracking algorithm to explore all possible paths through the table.

Implementation: [LCS - Bottom-up DP](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-common-sub/lcs-4.c)


