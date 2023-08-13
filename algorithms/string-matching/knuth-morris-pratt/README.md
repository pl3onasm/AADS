# Knuth Morris Pratt Algorithm (KMP)

## Problem

Given a pattern $P[0..m-1]$ and a text $T[0..n-1]$, find all occurrences of $P$ in $T$. It is assumed that $n \geq m$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts, i.e. the valid shifts for $P$ in $T$.

## Knuth Morris Pratt Algorithm

The algorithm simulates the operation of a string-matching automaton with $m$ states, but redefines the transition function $\delta$ as follows:

$$
\delta(q, a) = \begin{cases}
q + 1 & \text{if } q < m \text{ and } a = P[q + 1] \\
q' + 1 & \text{if } q' > 0 \text{ and } a = P[q' + 1] \\
0 & \text{otherwise}
\end{cases}
$$

where $q' < q < m$ and $q' \in \pi^*[q]$.

Implementation: [KMP Algorithm](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/knuth-morris-pratt/kmp.c)
