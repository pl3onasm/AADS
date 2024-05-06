_______________________________________________
*Maximum cardinality bipartite matching (MCBM)*
_______________________________________________

# ${\color{Cadetblue}\text{Unweighted preferred MCBM}}$

## ${\color{rosybrown}\text{Problem}}$

Given a bipartite graph $G = (R \cup L, E)$, find a maximum cardinality matching $M$. A bipartite graph is a graph whose vertices can be divided into two disjoint vertex sets $R$ and $L$ such that every edge connects a vertex in $R$ to one in $L$. A matching then is a subset of edges $M \subseteq E$ in which no two edges share a common vertex and a maximum cardinality matching is simply a matching that consists of a maximum subset $M$ of edges in $E$.

## ${\color{darkseagreen}\text{Gale-Shapley algorithm}}$

In this version of the MCBM problem, we are given an unweighted $\color{orchid}{\text{complete}}$ bipartite graph $G = (R \cup L, E)$, meaning that there is an edge with unit weight from every vertex in $L$ to every vertex in $R$. We are also given a $\color{orchid}{\text{preference list}}$ for each vertex $u$ in $L \cup R$ which specifies the order in which the vertices in the other vertex set, different from the one $u$ itself belongs to, are preferred by $u$. The goal then is to find a maximum cardinality matching $M$ in which the preference list of each vertex in $L$ and $R$ is optimally satisfied.  

A matching is said to be $\color{orchid}{\text{stable}}$ if no better matching can be found by swapping two matched pairs. If there is such a pair $(u, v)$ in the matching $M$, then we say that $u$ and $v$ are $\color{orchid}{\text{blocking}}$ each other. The aim is to find a maximum cardinality stable matching, and this can be done using a simple greedy algorithm known as the Gale-Shapley algorithm which is guaranteed to find a stable matching in $\mathcal{O}{(VE)}$ time.

The algorithm is based on the principle of $\color{orchid}{\text{proposals}}$ and $\color{orchid}{\text{rejections}}$. We start with an empty matching and then we let the vertices in $L$ propose to the vertices in $R$ according to the proposer's preference list. If the vertex in $R$ is unmatched, then it accepts the proposal and the matching is updated accordingly. If the vertex in $R$ is already matched, then it checks if the proposer is preferred over its current match according to its own preference list. If this is the case, then the vertex in $R$ accepts the proposal and the matching is updated accordingly, otherwise the proposal is rejected and the matching remains unchanged. The algorithm terminates when no more proposals can be made and the matching is stable.

Implementation: [MCBM - Gale-Shapley](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/MCBM-gale-shapley/gale-shapley.c)
