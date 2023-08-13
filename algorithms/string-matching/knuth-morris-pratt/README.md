# Knuth Morris Pratt Algorithm (KMP)

## Problem

Given a pattern $P[0..m-1]$ and a text $T[0..n-1]$, find all occurrences of $P$ in $T$. It is assumed that $n \geq m$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts, i.e. the valid shifts for $P$ in $T$.

## Knuth Morris Pratt Algorithm

The algorithm works by precomputing a prefix function $\pi : \lbrace 0, 1, \dots, m \rbrace \rightarrow \lbrace 0, 1, \dots, m \rbrace$ for the pattern $P$, which is defined as follows:

$$
\pi(q) = \max \lbrace k : k < q \land P_k \sqsupset P_q \rbrace
$$

The matching process then works in the same way as a [string-matching automaton](https://github.com/pl3onasm/CLRS/tree/main/algorithms/string-matching/finite-automata) with $m$ states, where the transition function is computed on the fly and is redefined to be:

$$
\delta(q, a) = \begin{cases}
q + 1 & \text{if } q < m \land a = P[q + 1] \\
\pi(q) & \text{if } q = m \\
q' + 1 & \text{if } q' > 0 \\
0 & \text{otherwise}
\end{cases}
$$

where $q' = \max \lbrace k: k \in \pi^\ast(q) \land a = P[k + 1] \rbrace$.

The iterated prefix function $\pi^\ast$ can be recursively defined as:

$$
\pi^\ast(q) = \begin{cases}
\lbrace 0 \rbrace & \text{if } \pi(q) = 0 \\
\lbrace \pi(q) \rbrace \cup  \pi^\ast(\pi(q)) & \text{otherwise}
\end{cases}
$$

The running time of the algorithm is $\Theta(n)$, since the prefix function can be computed in $\Theta(m)$ time and the matching process takes $\Theta(n)$ time.

Implementation: [KMP Algorithm](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/knuth-morris-pratt/kmp.c)
