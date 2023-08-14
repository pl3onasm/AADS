# Knuth Morris Pratt Algorithm (KMP)

## Problem

Given a pattern $P[0..m-1]$ and a text $T[0..n-1]$, find all occurrences of $P$ in $T$. It is assumed that $n \geq m$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts, i.e. the valid shifts for $P$ in $T$.

## Knuth Morris Pratt Algorithm

If we look at the [string-matching automaton](https://github.com/pl3onasm/CLRS/tree/main/algorithms/string-matching/finite-automata), it is clear that the preprocessing step to compute $\delta$ is the most expensive part of the algorithm. In fact, it does way too much work, since it computes transitions for every possible character in the alphabet $\Sigma$ (resulting in a table containing $|\Sigma| \cdot m$ entries, most of which are simply zero), even though we only need to know the next state for the characters in $P$. Furthermore, it does not take into account the structure of the pattern $P$ itself, but only the fact that it is a fixed string of length $m$.

This is where the Knuth Morris Pratt algorithm comes in: it does not compute the transition function $\delta$ in advance, but instead computes it on the fly during the matching process, by using a precomputed prefix function $\pi : \lbrace 0, 1, \dots, m \rbrace \rightarrow \lbrace 0, 1, \dots, m \rbrace$, which maps a state $q$ to the length of the longest prefix of $P$ that is also a suffix of $P_q$, thus effectively using the information about the pattern $P$ to guide the matching process by indicating the next shift to try in case of a mismatch. This prefix function is defined as:

$$
\pi(q) = \max \lbrace k : k < q \land P_k \sqsupset P_q \rbrace
$$

The matching process then works in the same way as a [string-matching automaton](https://github.com/pl3onasm/CLRS/tree/main/algorithms/string-matching/finite-automata) with $m$ states, where the transition function is computed on the fly and is redefined to be:

$$
\delta(q, a) = \begin{cases}
q + 1 & \text{if } q < m \land a = P[q] \\
q' + 1 & \text{if } q' > 0 \\
0 & \text{otherwise}
\end{cases}
$$

where $q' = \max \lbrace k: k \in \pi^\ast(q) \land a = P[k] \rbrace$.[^1]

[^1]: Since we are working with 0-based indices (contrary to the book), P[q] is in fact the (q+1)-th character of P. Prefix function values, just like state values, pertain to string lengths, not indices. Thus, there is always a difference of 1 when switching between state or prefix function values and indices.

The iterated prefix function $\pi^\ast$ can be recursively defined as:

$$
\pi^\ast(q) = \begin{cases}
\lbrace 0 \rbrace & \text{if } \pi(q) = 0 \\
\lbrace \pi(q) \rbrace \cup  \pi^\ast(\pi(q)) & \text{otherwise}
\end{cases}
$$

The running time of the algorithm is $\Theta(n)$, since the prefix function can be computed in $\Theta(m)$ time and the matching process takes $\Theta(n)$ time.

Implementation: [KMP Algorithm](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/knuth-morris-pratt/kmp.c)
