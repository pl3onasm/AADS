${\color{Cadetblue}\text{\huge Longest Increasing}}$  
${\color{Cadetblue}\text{\huge Subsequence (LIS)}}$

<br />

${\color{rosybrown}\text{\Large Problem}}$

Given an array of integers, find the length of the longest ${\color{peru}\text{monotonically}}$ ${\color{peru}\text{increasing}}$ (so not necessarily strictly increasing) subsequence. A ${\color{peru}\text{subsequence}}$ is a sequence that can be derived from the original array by deleting some elements (possibly none) from it without changing the order of the remaining elements.  

In other words, given an array $X$ of length $n$, find the maximum length $m$ of a sequence $X'$ where $X'[i] = X[j]$ for all $0 \leq i < m$ and some $0 \leq j < n$, such that $X'[i] \leq X'[i+1]$ for all $0 \leq i < m-1$. Note that the elements of the subsequence can but do not have to be adjacent in the original array.  

For example, given the array $X = [12, 8, 1, 4, 2, 9, 10, 18, 15, 7, 20]$, the length of the LIS is $6$. A possible LIS is for example $X' = [1, 2, 9, 10, 15, 20]$. This LIS is not unique, however. For example, $X'' = [1, 4, 9, 10, 18, 20]$ is also a valid LIS. Note that if the array $X$ is sorted in ascending order, then we have $m = n$ and $X' = X$.  

<br />

${\color{darkseagreen}\text{\Large The key idea}}$

Just as in the case of the [longest common subsequence problem](https://github.com/pl3onasm/CLRS/tree/main/algorithms/dynamic-programming/longest-common-sub), the key idea is to think of the subproblems in terms of ${\color{peru}\text{prefixes}}$ of the input array $X$. In other words, we want to think of each subproblem as finding the longest increasing subsequence of the prefix $X_i$ of length $i$ for some $0 < i \leq n$. We start with the subproblem for the prefix of length $n$, which is the original problem, and work our way down to the subproblem for the prefix of length $0$, which is the base case.

The subproblem for a prefix $X_i$ can be solved by considering the possible choices for its last element $X[i - 1]$: we can either choose to include this element in the subsequence or not. The choice to exclude the element $X[i - 1]$ is always valid. Including the element, on the other hand, can be done in two ways: either we prepend $X[i - 1]$ to an empty subsequence to start a new increasing subsequence or we prepend $X[i - 1]$ to an existing increasing subsequence that is starting with an element $X[j]$ such that $X[j] \geq X[i - 1]$.  

Clearly, the subproblems ${\color{peru}\text{overlap}}$, as the same subproblem is encountered multiple times during the computation of the optimal solution to the original problem. They also exhibit ${\color{peru}\text{optimal}}$ ${\color{peru}\text{substructure}}$: the optimal solution to a subproblem can be constructed from the optimal solutions to its subproblems.

As a result, we can define the following recurrence to compute the length $L(i,j)$ of the longest increasing subsequence of the prefix $X_i$, and where $j$ is the index of the previous element that was included in the subsequence (or $\infty$ if no element was included yet and we start from an empty subsequence):

$$
\color{darkslateblue}\huge\boxed{\color{rosybrown}\normalsize \space
\normalsize L(i,j) = \begin{cases} \small 0 & \scriptsize \text{if } i = 0 \\
\small \text{max}\lbrace L(i-1,j), & \scriptsize \text{if } j = \infty \\
\small \quad \space 1 + L(i-1,i-1) \rbrace &\scriptsize \space  \lor \space X[i - 1] \leq X[j] \\
\small L(i-1,j) &\scriptsize \text{otherwise}
\end{cases}\space}
$$

<br />

To find the length of the LIS of the entire input array $X$, we need to compute $L(n,\infty)$. The base case occurs when the prefix length is $0$, in which case the length of the LIS is $0$. In the recursive case, we first need to check if including the current element in the subsequence is a valid choice or not. If it is not, then the length of the LIS remains the same and we move on to the next smaller prefix. If it is valid, then we need to take the maximum of the two possible choices of excluding or including the current element. In the latter case, the length of the LIS is incremented by $1$ and the index of the last element that was included in the subsequence is updated to the current index.

<br />

${\color{darkseagreen}\text{\Large Brute force}}$

The brute force solution simply implements the recurrence without any optimization, and ignores the fact that identical subproblems (finding longest increasing subsequences for the same prefixes of $X$) are encountered multiple times during the recursive calls. This leads to an exponential running time, as the same subproblems are recomputed over and over again. The running time of this approach is in $\mathcal{O}(2^n)$.

Implementation: [LIS - Brute Force](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-increasing-sub/lis-1.c)

<br />

${\color{darkseagreen}\text{\Large Top-down}}$

The top-down approach uses ${\color{peru}\text{memoization}}$ to store the results of the subproblems in a table $T$ so that they need not be recomputed. At the beginning, this table is initialized with $\infty$ values to indicate that the subproblems have not been solved yet. At each recursive call, the table $T$ is consulted to see if the result of the subproblem has already been computed. If it has, then the result is simply returned. If it has not, then the subproblem is solved recursively and the result is stored in the table $T$ for future reference. The final result is then found in the cell $T[n][n]$, which contains the length of the longest increasing subsequence of the entire input array $X$. The total running time of this approach is in $\mathcal{O}(n^2)$.

${\color{peru}\text{Reconstruction}}$ of an actual LIS can be done from the table $T$: starting from the cell $T[n][n]$, we can backtrack to the cell $T[0][0]$ by following the choices that were made during the computation of the optimal solution. If the value in the cell $T[i][j]$ is equal to the value in the cell $T[i-1][j]$, then the element $X[i-1]$ was not included in the subsequence. If, however, the value in the cell $T[i][j]$ is equal to $1 + T[i-1][i-1]$, then the element $X[i-1]$ was included in the subsequence. The running time of this reconstruction is in $\mathcal{O}(n)$.

Implementation: [LIS - top-down DP](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-increasing-sub/lis-2.c)

<br />

${\color{darkseagreen}\text{\Large Bottom-up}}$

Instead of solving the subproblems recursively, the bottom-up approach solves them iteratively, starting from the subproblem for the prefix $X[0]$ of length $1$ and working all the way up to the subproblem for the prefix $X[0..n-1]$ of length $n$. As the subproblems are solved in order of increasing prefix length, all the subproblems needed to solve the next subproblem are already available in the table $T$. This means that we do not need to store the choices that were made for the index $j$ of the last element that was included in the subsequence, thus reducing the space complexity of the table $T$ from $n \times n$ to $n$. Although this approach is thus more efficient in terms of space and overhead, the total running time is still in $\mathcal{O}(n^2)$.

${\color{peru}\text{Reconstructing}}$ an actual LIS can be done by following the path of an optimal solution in the table $T$: starting from the cell $T[n-1]$, we can backtrack to the cell $T[0]$ by following the choices that were made during the computation. If the value in the cell $T[i]$ is equal to the value in the cell $T[i-1]$, then the element $X[i]$ was not included in the subsequence. If, however, the value in the cell $T[i]$ is equal to $1 + T[i-1]$, then the element $X[i]$ was included in the subsequence. The running time of this reconstruction is in $\mathcal{O}(n)$.

Implementation: [LIS - bottom-up DP](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-increasing-sub/lis-3.c)

<br />

${\color{darkseagreen}\text{\Large Linearithmic (Ex 14.4-6)}}$

A linearithmic solution to the LIS problem is achieved by maintaining a one-dimensional table $T$ of length $n$ where $T[i]$ represents a candidate for the last element of the LIS of length $i+1$. At the beginning, the table $T$ is initialized with the first element of the input array $X$ and the length of the LIS is set to $1$. The remaining elements of the input array $X$ are then processed one by one in a greedy manner. For each element $X[i]$, the table $T$ is searched, using binary search, for the leftmost element $T[j]$ that is greater than or equal to $X[i]$. If such an element is found, then $T[j]$ is replaced by $X[i]$. If no such element is found, then $X[i]$ is appended to the table $T$ and the length of the LIS is incremented by $1$. The final length of the LIS is then equal to the length of the table $T$.

This whole process can be thought of as ${\color{peru}\text{patience sorting}}$: the table $T$ represents the piles of cards that are formed when playing the patience card game, and the aim at each step is to find the leftmost pile where the current card can be placed. If no such pile is found, then a new pile is created to the right of the last pile and the card is placed there, while the total number of piles is kept to a minimum at all times. It is actually thanks to a theorem by Hammersley (1972) that we can be sure that the length of the LIS is equal to the number of piles at the end of the game. The total running time of this approach is in $\mathcal{O}(n \log n)$, as the binary search operation takes $\mathcal{O}(\log n)$ time and is performed $n$ times.

${\color{peru}\text{Reconstruction}}$ of an actual LIS can be done if we also maintain an auxiliary table $P$ of length $n$ where $P[j]$ stores the index of the element on top of the pile $T[j-1]$ at the time when the element $X[i]$ was added to the pile $T[j]$. This way, we can reconstruct the indices of the elements that form an actual LIS. Starting from the last element of the table $T$, we can then backtrack to the first element of the LIS by following the indices stored in the table $P$. The running time of this reconstruction is in $\mathcal{O}(n)$.

Implementation: [LIS - linearithmic](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-increasing-sub/lis-4.c)
