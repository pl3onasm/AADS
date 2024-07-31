${\color{Cadetblue}\text{\huge Minimum Edit}}$  
${\color{Cadetblue}\text{\huge Distance (MED)}}$

<br />

${\color{rosybrown}\text{\Large Problem}}$

Given are two strings $S$ and $T$ of lengths $m$ and $n$ respectively, and a set of operations each with an associated cost. Our task is to transform the source string $S$ to the target string $T$, by applying a sequence of operations on $S$, such that the total cost of the transformation is minimized.

At each step of the transformation process, we can choose from the following operations:

- ${\color{cornflowerblue}\text{Copy}}$ the current character from $S$, if it matches the current character from $T$, with cost ${\color{orchid}c_c}$  
- ${\color{cornflowerblue}\text{Insert}}$ a character at the current position in $S$ with cost ${\color{orchid}c_i}$. This operation is always possible.
- ${\color{cornflowerblue}\text{Delete}}$ the current character from $S$ with cost ${\color{orchid}c_d}$. This operation is always possible.
- ${\color{cornflowerblue}\text{Replace}}$ the current non-matching character from $S$ with the current character from $T$ with cost ${\color{orchid}c_r}$
- ${\color{cornflowerblue}\text{Swap}}$ the current and next character in $S$ and copy them with cost ${\color{orchid}c_s}$ in the same step in order to match the next two characters in $T$
- ${\color{cornflowerblue}\text{Kill}}$ the remaining characters in $S$ with cost ${\color{orchid}c_k}$ in order to reach the end of $T$. This operation is necessarily the last operation in the sequence.

For example, given $S =$ `five` and $T =$ `six`, with costs $c_c = 1$, $c_i = 2$, $c_d = 3$, $c_r = 4$, $c_s = 5$, and $c_k = 6$, an optimal operation sequence is:

- `|five` &emsp; Replace `f` by `s`
- `s|ive` &emsp; Copy `i`
- `si|ve` &emsp; Replace `v` by `x`
- `six|e` &emsp; Delete `e`
- `six|`

The total cost of this transformation is the total sum of the costs of the operations, which is $4 + 1 +$ $4 + 3$ $= 12$. This is the minimum edit distance between the two strings $S$ and $T$.

Note that the optimal sequence of operations is ${\color{peru}\text{not unique}}$, as there may be multiple ways to transform the source string to the target string with the same minimum cost. For the example above, another optimal operation sequence is:

- `|five` &emsp; &emsp;Insert `s`
- `s|five` &emsp; &ensp;Insert `i`
- `si|five` &emsp; Insert `x`
- `six|five` &ensp; Kill 4 chars
- `six|`

The total cost of this transformation is the same as before: $2 + 2 +$ $2 + 6$ $= 12$.

<br />

${\color{darkseagreen}\text{\Large The key idea}}$

The idea is to think of the subproblems in terms of ${\color{peru}\text{prefixes}}$ of the two given strings. Let $S_i$ and $T_j$ be the prefixes of lengths $i$ and $j$ respectively. By solving the subproblems for smaller and smaller prefixes, we can build up the solution for the entire input strings, as the minimum edit distance can be expressed in terms of the minimum edit distances of the prefixes by considering at each step all the possible options for the last operation and eventually choosing the one that minimizes the total cost.

For example, if the last operation is a ${\color{cornflowerblue}\text{copy}}$, then the minimum edit distance of the prefixes $S_i$ and $T_j$ is the minimum edit distance of their prefixes $S_{i-1}$ and $T_{j-1}$, plus the cost of the copy operation ${\color{orchid}c_c}$. If, on the other hand, the last operation is a ${\color{cornflowerblue}\text{delete}}$, then the minimum edit distance of the prefixes $S_i$ and $T_j$ is the minimum edit distance of their prefixes $S_{i-1}$ and $T_j$, plus the cost of the delete operation ${\color{orchid}c_d}$, and so on.

Clearly, the subproblems are ${\color{peru}\text{overlapping}}$, and also exhibit ${\color{peru}\text{optimal substructure}}$, as the minimum edit distance of the prefixes can be expressed in terms of the minimum edit distances of their prefixes. This makes the problem an ideal candidate for dynamic programming.  

As a consequence, we can define a recursive function $D(i, j)$ that returns the minimum edit distance of the prefixes $S_i$ and $T_j$ as follows:

$$
\color{darkslateblue}\huge\boxed{\color{rosybrown}\normalsize \space
D(i,j) = \min \begin{cases}
\small D(i-1, j-1) + {\color{orchid}c_r} & \scriptsize \text{if } S[i-1] \neq T[j-1] \\
\small D(i-1, j-1) + {\color{orchid}c_c} & \scriptsize \text{if } S[i-1] = T[j-1] \\
\small D(i-1, j) + {\color{orchid}c_d} \\
\small D(i, j-1) + {\color{orchid}c_i} \\
\small D(i-2, j-2) + {\color{orchid}c_s} & \scriptsize \text{if } S[i-1] = T[j-2] \\
& \space \scriptsize \land \space S[i-2] = T[j-1] \\
\small \min \lbrace D(k, n) + {\color{orchid}c_k}   \\
\qquad \small : 0 \leq k < m \rbrace  & \scriptsize \text{if } i = m \land j = n
\end{cases}\space}
$$

<br />

The base cases are $D(0, j) = j \cdot c_i$ and $D(i, 0) = i \cdot c_d$, since reaching an empty source string $S$ requires inserting $j$ characters, the ones that are still left in $T$, at a total cost of $j \cdot c_i$, and similarly, reaching an empty target string $T$ requires deleting $i$ characters, the ones that are still left in $S$, at a total cost of $i \cdot c_d$. The recursive case is when the last operation is a copy, delete, insert, replace, or swap. The kill operation, on the other hand, is necessarily the last operation in the sequence, so it is only considered when the end of both strings is reached. The final answer then is obtained by computing $D(m, n)$ for the entire input strings $S$ and $T$.  

<br />

${\color{darkseagreen}\text{\Large Brute force}}$

Solving the minimum edit distance problem by brute force is straightforward: we implement the recurrence relation above without any optimization. The time complexity of this approach is $\mathcal{O}(6^{m+n})$, since there are $6$ possible operations at each step, and the total number of steps is $m + n$. No need to say that this approach is impractical for large strings. 

Implementation: [MED - Brute Force](https://github.com/pl3onasm/CLRS/blob/main/algorithms/dynamic-programming/min-edit-dist/med-1.c)

<br />

${\color{darkseagreen}\text{\Large Top-down}}$

The top-down approach is essentially a quick fix to the brute force solution, where we cache the results of the subproblems in a table to avoid redundant computations, ensuring that each subproblem is solved only once. The time complexity of this approach is $\mathcal{O}(m \cdot n)$, since there are $m \cdot n$ unique subproblems that need to be solved.  

Implementation: [MED - Top-down](https://github.com/pl3onasm/CLRS/blob/main/algorithms/dynamic-programming/min-edit-dist/med-2.c)

<br />

A ${\color{peru}\text{reconstruction}}$ of an optimal sequence of operations can be achieved by storing the previous operation that led to the minimum edit distance of the prefixes $S_i$ and $T_j$ in a separate table. This way, we can trace back the sequence of operations that were applied to reach the minimum edit distance of the entire input strings $S$ and $T$.

Implementation: [MED - Top-down with Reconstruction](https://github.com/pl3onasm/CLRS/blob/main/algorithms/dynamic-programming/min-edit-dist/med-3.c)

<br />

${\color{darkseagreen}\text{\Large Bottom-up}}$

The bottom-up approach is a more efficient way to solve the MED problem, as it avoids the overhead of recursive calls. The idea here is to build up the final solution by solving the subproblems in a ${\color{peru}\text{topological order}}$ determined by the lengths of the prefixes, starting with the smallest prefixes and working our way up to the entire input strings. The time complexity of this approach is the same as that of the top-down approach, $\mathcal{O}(m \cdot n)$.

Implementation: [MED - Bottom-up](https://github.com/pl3onasm/CLRS/blob/main/algorithms/dynamic-programming/min-edit-dist/med-4.c)
