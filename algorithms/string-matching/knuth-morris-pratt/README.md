# Knuth Morris Pratt Algorithm (KMP)

## Problem

Given a pattern $P[0..m-1]$ and a text $T[0..n-1]$, find all occurrences of $P$ in $T$. It is assumed that $n \geq m$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts, i.e. the valid shifts for $P$ in $T$.

## Knuth Morris Pratt Algorithm

The algorithm works by precomputing a prefix function $\pi$ for the pattern $P$, which is defined as follows:

$$
\pi(q) = \max \lbrace k : k < q \text{ and } P_k \sqsupset P_q \rbrace
$$

This prefix function $\pi$ can be computed in $O(m)$ time. The matching process then works in the same way as a [string-matching automaton](https://github.com/pl3onasm/CLRS/tree/main/algorithms/string-matching/finite-automata) with $m$ states, where the transition function is redefined to be:

$$
\delta(q, a) = \begin{cases}
q + 1 & \text{if } q < m \land a = P[q + 1] \\
q' + 1 & \text{if } q' > 0 \land a = P[q' + 1] \\
q' & \text{if } q' = 0 \lor q = m \\
\end{cases}
$$

where $q' < q < m$ and $q' \in \pi^\ast(q)$.

The iterated prefix function $\pi^\ast$ is defined as follows:

$$
\pi^\ast(q) = \begin{cases}
\lbrace 0 \rbrace & \text{if } \pi(q) = 0 \\
\pi^\ast(\pi(q)) \cup \lbrace \pi(q) \rbrace & \text{otherwise}
\end{cases}
$$

Implementation: [KMP Algorithm](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/knuth-morris-pratt/kmp.c)
