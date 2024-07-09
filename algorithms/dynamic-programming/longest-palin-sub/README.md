# ${\color{Cadetblue}\text{Longest palindromic}}$ ${\color{Cadetblue}\text{subsequence}}$ ${\color{Cadetblue}\text{(LPS)}}$

## ${\color{rosybrown}\text{Problem}}$

Given a sequence $X = \langle x_0, x_1, \ldots, x_{n-1} \rangle$, we want to find the longest palindromic subsequence of $X$. A ${\color{peru}\text{subsequence}}$ is derived from a given sequence by deleting some or no elements from it without changing the order of the remaining elements. Such a sequence is ${\color{peru}\text{palindromic}}$ if it reads the same forwards and backwards.

For example, the sequence $X = \langle C, A, D, B, D, C, B, E, A, B \rangle$ has a longest palindromic subsequence of length 5, which is $\langle A, B, C, B, A \rangle$. The subsequence itself does not have to be unique, however: the input sequence $X$, has also $\langle A, D, B, D, A \rangle$ and $\langle C, D, B, D, C \rangle$ as palindromic subsequences of length 5. The length of the LPS is of course unique.

## ${\color{darkseagreen}\text{The key idea}}$

The idea is to think of each subproblem in terms of a ${\color{peru}\text{substring}}$ with indices $i$ and $j$ of the sequence $X$. The subproblem is then to find the LPS of the substring $X[i \ldots j]$. We start with the substring $X[0 \ldots n-1]$, the input sequence itself, and work our way down to the base cases, which are substrings of length $1$ or $0$.

At each step, we can choose to include the first and last characters of the substring in the palindromic subsequence if they are identical. If they are, we add $2$ to the length of the LPS so far and consider the LPS of the substring $X[i+1 \ldots j-1]$, which is the substring without the first and last characters. If they are different, however, we need to consider the maximum of the LPSs we can get by excluding either the first or the last character of the current substring, that is, we need to consider the lenghts of the LPSs of the substrings $X[i+1 \ldots j]$ and $X[i \ldots j-1]$ and the length of the LPS of the original substring is then the maximum of these two lengths.

Clearly, the subproblems are ${\color{peru}\text{overlapping}}$ because we are very likely to reencounter the same substrings in multiple subproblems, especially when the first and last characters are different and the subproblems are getting smaller. The subproblems also have the ${\color{peru}\text{optimal substructure}}$ property because the optimal solution to a particular subproblem can be obtained by combining the optimal solutions to its own subproblems.

Thus, a recursive solution to the problem can be found by defining a function $L(i,j)$ that returns the length of the longest palindromic subsequence of the substring $X[i \ldots j]$ as follows:
<br />

$$
\color{darkslateblue}\huge\boxed{\color{rosybrown}\normalsize \space
\normalsize L(i,j) = \begin{cases} 1 & \scriptsize \text{if } i = j \\
\normalsize 0 & \scriptsize \text{if } i > j \\
\normalsize 2 + L(i+1,j-1) &\scriptsize \text{if } X[i] = X[j] \\
\normalsize \text{max}\lbrace L(i+1,j), & \scriptsize \text{otherwise} \\
\normalsize \qquad \space L(i,j-1) \rbrace  \\
\end{cases}\space}
$$

<br />

The solution to the original problem is then obtained by calling $L(0,n-1)$. The base cases occur when the substring has only one or no characters left: in the first case, the length of the LPS is $1$ (since a single character is a palindrome), and in the second case, its length is $0$. If, at a given step, the first and last characters of the substring are the same, we update the optimal solution of the previous subproblem by increasing the LPS length by 2. If the characters are different, we compute the maximum LPS length between excluding the first character and excluding the last character of the substring at the current step.

## ${\color{darkseagreen}\text{Brute force}}$

The brute force solution implements the recursive function $L(i,j)$ as described above without any optimization. It ignores the fact that the subproblems overlap and recomputes the length of the LPS of the same substrings multiple times. The time complexity of the brute force solution is therefore exponential.

Implementation: [LPS - brute force](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-palin-sub/lps-1.c)

## ${\color{darkseagreen}\text{Top-down}}$

The top-down DP solution implements the same recursive function $L(i,j)$ but uses a ${\color{peru}\text{memoization}}$ ${\color{peru}\text{table}}$ to store the length of the LPS of each substring once it is computed. After initializing this table with a special value that indicates the subproblem has not been computed yet (e.g. $0$), the program is modified to check at each recursive step if the length of the LPS of the substring has already been computed. If it has, the stored length is returned. Otherwise, the length is computed as before and stored in the table. As each subproblem is thus guaranteed to be computed only once, the time complexity of the top-down solution is reduced to $\mathcal{O}(n^2)$.

${\color{peru}\text{Reconstruction}}$ of an optimal solution from the memoization table $T$ is possible by starting from the cell $T[0][n-1]$ and moving to the left or down if the value of the current cell is equal to the value of the cell to the left or below. Otherwise, we add the first and last characters of the substring to the solution and move diagonally to the cell $T[i+1][j-1]$. This process is repeated until we reach the base case where $i \geq j$.

Implementation: [LPS - top-down DP](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-palin-sub/lps-2.c)

## ${\color{darkseagreen}\text{Bottom-up}}$

In the bottom-up DP approach, we start by setting the base cases of the memoization table $T$ and then fill the table in a bottom-up manner, starting from the smallest substrings and working our way up to the original problem by gradually increasing the length of the substrings we consider. The time complexity of the bottom-up solution is $\mathcal{O}(n^2)$, the same as the top-down solution. Reconstruction of an optimal solution is done in the same way as in the top-down solution.
