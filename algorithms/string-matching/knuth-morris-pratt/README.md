# Knuth Morris Pratt Algorithm (KMP)




$$
\delta(q, a) = \begin{cases}
q + 1 & \text{if } q < m \text{ and } a = P[q + 1] \\
\pi(P_qa) & \text{otherwise}\\
\end{cases}
$$

Implementation: [KMP Algorithm](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/knuth-morris-pratt/kmp.c)
