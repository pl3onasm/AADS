# Maximum Cardinality Bipartite Matching (MCBM)

(Under construction)

## Problem

Given a bipartite graph $G = (R \cup L, E)$, find a maximum cardinality matching $M$. A bipartite graph is a graph whose vertices can be divided into two disjoint vertex sets $R$ and $L$ such that every edge connects a vertex in $R$ to one in $L$. A matching then is a subset of edges $M \subseteq E$ in which no two edges share a common vertex and a maximum cardinality matching is simply a matching that consists of a maximum subset $M$ of edges in $E$.

## Weighted MCBM

In this version of the problem, the edges come with a cost and we are asked to find a maximum cardinality matching $M$ such that the total cost of the edges in $M$ is minimized or maximized. The following algorithms can be used to solve this problem.

### Min/max-cost Max-flow Algorithm (CP 9.25)

We can solve the problem by using the min/max-cost max-flow algorithm. For this, we simply add a super source $s$ and a super sink $t$ to the given graph and then add an edge from $s$ to every vertex in $L$ and an edge from every vertex in $R$ to $t$. The capacity of all edges is set to 1 and the cost of each given edge is set to its weight, while the cost of the edges from $s$ to $L$ and from $R$ to $t$ is set to 0.  
To get the min/max-cost max-flow algorithm we then simply replace the BFS in Dinic's algorithm with a Bellman-Ford algorithm, which can handle negative edge weights. The resulting maximum flow is then the cardinality of the maximum matching, and the edges with flow are the edges that are part of this matching. In order to compute the maximum cost instead of the minimum cost, we simply negate the cost of each given edge. The algorithm runs in $O(V^2 E^2)$ time, since the Bellman-Ford algorithm runs in $O(VE)$ time and the DFS that updates the matching runs in $O(VE)$ time as well, thanks to the speedup by remembering the adjacency lists indices which allows to prune the search space of each DFS.

Implementation: [MCBM - Min/max-cost max-flow](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/MCBM-weighted/mcbm-4.c)

### Hungarian Algorithm (Kuhn-Munkres Algorithm) (CLRS⁴ 25.3)

Implementation: [MCBM - Kuhn-Munkres]()
