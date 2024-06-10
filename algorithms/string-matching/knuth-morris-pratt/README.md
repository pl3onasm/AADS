# ${\color{Cadetblue}\text{Knuth Morris Pratt}}$ ${\color{Cadetblue}\text{Algorithm (KMP)}}$

## ${\color{Rosybrown}\text{Problem}}$

Given a pattern $P[0:m-1]$ and a text $T[0:n-1]$, find all occurrences of $P$ in $T$. It is assumed that $n \geq m$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts, i.e. the valid shifts for $P$ in $T$.

## ${\color{darkseagreen}\text{The KMP algorithm}}$

If we look at the [string-matching automaton](https://github.com/pl3onasm/CLRS/tree/main/algorithms/string-matching/finite-automata), it is clear that the preprocessing step to compute $\delta$ is the most expensive part of the algorithm. In fact, it does way too much work, since it computes transitions for every possible character in the alphabet $\Sigma$ (resulting in a table containing $|\Sigma| \cdot m$ entries, most of which are simply zero), even though we only need to know the next state for the characters in $P$. Furthermore, it does not take into account the structure of the pattern $P$ itself, but only the fact that it is a fixed string of length $m$.

This is where the KMP algorithm comes in: it does not compute an exhaustive transition function $\delta$ in advance, but instead computes the transitions on the fly during the matching process, by using a ${\color{darkcyan}\text{precomputed}}$ ${\color{darkcyan}\text{prefix function }\pi}$ $: \lbrace 0, 1, \dots, m \rbrace \rightarrow \lbrace 0, 1, \dots, m \rbrace$, which maps a state $q$ to the length of the longest prefix of $P$ that is also a ${\color{peru}\text{proper}}$ suffix of $P_q$. In other words, it tells us how the pattern matches against shifts of itself, thus effectively using information on the structure of the pattern $P$ itself to guide the overall matching process by indicating the next shift to try in case of a mismatch. This prefix function is defined as follows:

$$
\color{darkslategray}\boxed{\color{darkcyan}\space \pi(q) = \color{darkgoldenrod}\max \lbrace k : k < q \land P_k \sqsupset P_q \rbrace \space}\\
$$

In case of a mismatch, the algorithm will then shift the pattern $P$ by $q - \pi(q)$ characters to the right, where $q$ is the current state of the automaton, so that the next character of $T$ is compared with $P[\pi(q)]$. This is possible because the prefix function $\pi$ tells us that the first $\pi(q)$ characters of $P$ are also the last $\pi(q)$ characters of $P_q$, so that we can safely skip the first $\pi(q)$ characters of $P$ in the next comparison. If the comparison is successful, the state is incremented by one, and the next character of $T$ is compared with the updated $P[q]$ (which is now the next character of $P$). If the state reaches $m$, we have found an occurrence of $P$ in $T$ and the shift is added to the result. If the comparison fails, the pattern is shifted again by $q - \pi(q)$ characters to the right, until there is a match or the pattern is shifted past the current position in $T$.

Thus, the matching process actually simulates a finite automaton with $m$ states, where the ${\color{rosybrown}\text{transition function}}$ is computed on the fly and is redefined to be:

$$
{\color{rosybrown}\delta(q, a) = } \begin{cases}
q + 1 & \text{if } q < m \land a = P[q] \\
q' + 1 & \text{if } q' > 0 \\
0 & \text{otherwise}
\end{cases}
$$

where $q' = \pi(q)$ is the next state to try in case of a mismatch, and can be defined as:

$q' = \max \lbrace k: k \in \pi^\ast(q) \land a = P[k] , 0 \rbrace$. [^1]

[^1]: Since we are working with 0-based indices (contrary to the book), P[q] is in fact the (q+1)-st character of P. Prefix function values, just like state values, pertain to string lengths, not indices. Thus, there is always a difference of 1 when switching between state or prefix function values and string indices.

The ${\color{palevioletred}\text{iterated prefix function } \pi^\ast}$ generates the set of all possible states to try in case of a mismatch, by recursively applying the prefix function to the current state until the obtained state is 0. It can be defined as:

$$
{\color{palevioletred}\pi^\ast(q) = }\begin{cases}
\lbrace 0 \rbrace & \text{if } \pi(q) = 0 \\
\lbrace \pi(q) \rbrace \cup  \pi^\ast(\pi(q)) & \text{otherwise}
\end{cases}
$$

The running time of the algorithm is $\Theta(n)$, since the prefix function can be computed in $\Theta(m)$ time and the matching process takes $\Theta(n)$ time.

Implementation: [KMP Algorithm](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/knuth-morris-pratt/kmp.c)
