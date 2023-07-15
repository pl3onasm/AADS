# Maximum Cardinality Bipartite Matching (MCBM)

(Under construction)

## Problem

Given a bipartite graph $G = (R \cup L, E)$, find a maximum cardinality matching $M$. A bipartite graph is a graph whose vertices can be divided into two disjoint vertex sets $R$ and $L$ such that every edge connects a vertex in $R$ to one in $L$. A matching then is a subset of edges $M \subseteq E$ in which no two edges share a common vertex and a maximum cardinality matching is simply a matching that consists of a maximum subset $M$ of edges in $E$.

## Unweighted preferred MCBM (CLRS⁴ 25.2)

In this version of the problem, we are given an unweighted *complete* bipartite graph $G = (R \cup L, E)$, meaning that there is an edge from every vertex in $L$ to every vertex in $R$. We are also given a preference list for each vertex $u$ in $L \cup R$ which specifies the order in which the vertices in the vertex set different from the one that $u$ itself belongs to are preferred by $u$. The goal then is to find a maximum cardinality matching $M$ such that the preference list of each vertex in $L$ and $R$ is satisfied. A matching is said to be *stable* if no better matching can be found by swapping two edges in $M$, i.e. all preference lists are satisfied. The aim is to find a maximum cardinality stable matching, and this can be done using an algorithm known as the Gale-Shapley algorithm.  

The algorithm is based on the idea of *proposals* and *rejections*. We start with an empty matching and then we repeatedly propose to each vertex in $L$ a candidate from $R$ that is at the top of its preference list. If the vertex in $R$ is unmatched, then it accepts the proposal and the matching is updated accordingly. If the vertex in $R$ is already matched, then it checks if the vertex in $L$ that proposed to it is preferred over the vertex in $L$ that it is currently matched to. If this is the case, then the vertex in $R$ accepts the proposal and the matching is updated accordingly, otherwise the proposal is rejected. This process is repeated until no more proposals can be made and the matching is stable. The algorithm runs in $O(VE)$ time, since each vertex in $L$ proposes to a vertex in $R$ at most once and each vertex in $R$ accepts or rejects a candidate at most once.

Implementation: [MCBM - Gale-Shapley](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/MCBM-gale-shapley/mcbm-3.c)
