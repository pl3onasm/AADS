${\color{Cadetblue}\text{\huge Wildcard Pattern}}$  
${\color{Cadetblue}\text{\huge Matching (WPM)}}$

<br />

${\color{rosybrown}\text{\Large Problem}}$

Given a text $T$ and a wildcard pattern $P$, of length $n$ and $m$ respectively, we want to determine whether the pattern matches the text. The pattern may contain any letter of the alphabet, the wildcard character `*` which matches any sequence of characters (including the empty sequence), and the wildcard character `?` which matches any single character of the alphabet.

For example, the pattern `a*d?c` matches the text `abcdec`, but not the text `abcdc`, nor the text `abcdd`.

<br />

${\color{darkseagreen}\text{\Large The key idea}}$

The idea is to think of the subproblems in terms of ${\color{peru}\text{prefixes}}$ of the text and the pattern. To consider each subproblem, we start at the end of the text and the pattern, and work our way towards the beginning. At each step, we consider the last character of the text and the pattern, and determine whether they match. If they do, we move one step back in both the text and the pattern. If they don't, we consider the wildcard characters `*` and `?` in the pattern, and move back in the text accordingly. If we reach both the beginning of the text and the pattern, with all characters matching, then the pattern matches the text.

The subproblems are ${\color{peru}\text{overlapping}}$, as the same prefixes of the text and the pattern may be considered multiple times during the matching process. The subproblems also exhibit ${\color{peru}\text{optimal substructure}}$, as the result of the matching of the prefixes can be expressed in terms of the matching of their prefixes. This makes the problem a good fit for dynamic programming.

As a result, we can define a recursive function $M(i, j)$ that returns whether the prefixes of the text and the pattern $T_i$ and $P_j$ match as follows:  

$$
\color{darkslateblue}\huge\boxed{\color{rosybrown}\normalsize \space
M(i,j) = \begin{cases}
\small {\color{cornflowerblue}\text{true}} & \scriptsize \text{if } i =  0 \land j = 0 \\
\small {\color{orchid}\text{false}} & \scriptsize \text{if } i \neq 0 \land j = 0  \\
\small M(i, j - 1) & \scriptsize \text{if } i = 0 \land j \neq 0\\
\small M(i, j -1)  & \scriptsize \text{if } P[j - 1] =  \text{'*'} \\
\quad \small \lor \space M(i - 1, j) \\
\small M(i-1, j-1) & \scriptsize \text{if } T[i-1] = P[j-1] \\
& \space \scriptsize \lor \space P[j - 1] =  \text{'?'}\\
\small {\color{orchid}\text{false}} & \scriptsize \text{otherwise}
\end{cases}\space}
$$

<br />

To obtain the answer to the original problem, we call $M(n, m)$. The base cases of the function occur when both the text and the pattern are empty, in which case the function returns true, or when the pattern is empty but the text is not, in which case the function returns false.  

A special recursive case of the function occurs when the text is empty but the pattern is not, in which case the function keeps moving back in the pattern and returns true iff the remaining characters of the pattern are found to be all `*` wildcards.

The other recursive cases of the function occur when the last characters of the text and the pattern match, or when the last character of the pattern is a `?` wildcard. In these cases, we move back in both the text and the pattern. If, on the other hand, the last character of the pattern is a `*` wildcard, we consider two possibilities: either the `*` wildcard matches the last character of the text, in which case we move back in the text, or the `*` wildcard matches an empty sequence, in which case we move back in the pattern. The function returns true if any of these constraints are met, and false otherwise.  

<br />

${\color{darkseagreen}\text{\Large Brute force}}$

The brute force approach simply implements the recursive function $M(i, j)$ as described above. It generates all possible renditions of the pattern and compares them to the text, ignoring the fact that the same prefixes of the text and the pattern may be considered multiple times. The time complexity of this approach is in the order of $\mathcal{O}(2^{n+m})$. The worst-case time complexity occurs when the pattern consists of only `*` wildcards.  

Implementation: [WPM - Brute force](https://github.com/pl3onasm/CLRS/blob/main/algorithms/dynamic-programming/wd-matching/wpm-1.c)  

<br />

${\color{darkseagreen}\text{\Large Top-down}}$

The top-down approach optimizes the brute force approach by ${\color{peru}\text{memoizing}}$ the results of the subproblems in a table. This way, the same subproblem is not solved multiple times. The time complexity of this approach is $\mathcal{O}(n \cdot m)$, since there are $n \cdot m$ unique subproblems that need to be solved, and each of them is solved only once.

Implementation: [WPM - Top-down](https://github.com/pl3onasm/CLRS/blob/main/algorithms/dynamic-programming/wd-matching/wpm-2.c)  

<br />

${\color{darkseagreen}\text{\Large Bottom-up}}$

The bottom-up approach relies on the same recursive function $M(i, j)$, but solves the subproblems in ${\color{peru}\text{topological order}}$ determined by the lengths of the prefixes: it starts with the base cases and works its way up to the entire text and pattern. The time complexity of this approach is the same as that of the top-down approach, $\mathcal{O}(n \cdot m)$.

Implementation: [WPM - Bottom-up](https://github.com/pl3onasm/CLRS/blob/main/algorithms/dynamic-programming/wd-matching/wpm-3.c)  

Since reconstruction is not applicable to the WPM problem, we can optimize the space complexity of the bottom-up approach to $\mathcal{O}(m)$ by using only one row of the table at a time. This is possible because the value of $M(i, j)$ depends only on the values of $M(i-1, j-1)$, $M(i-1, j)$, and $M(i, j-1)$, which can be stored in variables. The time complexity of this optimized approach is still $\mathcal{O}(n \cdot m)$.

Implementation: [WPM - Bottom-up (Optimized)](https://github.com/pl3onasm/CLRS/blob/main/algorithms/dynamic-programming/wd-matching/wpm-4.c)  
