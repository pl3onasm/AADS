_____________________________________
$\Large{\color{darkkhaki}\text{\it Maximum cardinality}}$  
$\Large{\color{darkkhaki}\text{\it bipartite matching (MCBM)}}$  
_____________________________________

<br/>

$\Large{\color{rosybrown}\text{Problem}}$

Given a bipartite graph $G = (R \cup L, E)$, find a maximum cardinality matching $M$. A bipartite graph is a graph whose vertices can be divided into two disjoint vertex sets $R$ and $L$ such that every edge connects a vertex in $R$ to one in $L$. A matching then is a subset of edges $M \subseteq E$ in which no two edges share a common vertex and a maximum cardinality matching is simply a matching that consists of a maximum subset $M$ of edges in $E$.

In this version of the problem, the edges come with a cost and we are asked to find a maximum cardinality matching $M$ such that the total cost of the edges in $M$ is minimized or maximized.

<br/>

$\Large{\color{darkseagreen}\text{Min/max-cost Max-flow}}$  
$\Large{\color{darkseagreen}\text{Algorithm (CP 9.25)}}$

We can solve the problem by using the min/max-cost max-flow algorithm. Just like in the [adapted max-flow algorithm](../MCBM-unweighted/mcbm-1.c) for the unweighted MCBM, we build a flow network from the given bipartite graph so that the edges are directed from $L$ to $R$: these edges have unit capacity and their cost is set to their weight. The network is also extended with a $\color{mediumorchid}{\text{super source }s}$ and a $\color{mediumorchid}{\text{super sink }t}$ with edges from $s$ to every vertex in $L$ and edges from every vertex in $R$ to $t$, all with unit capacity and zero cost.  

The min/max-cost max-flow algorithm is in fact a modification of Dinic's algorithm that uses the Bellman-Ford algorithm, which can also handle negative edge weights, instead of the BFS. The algorithm starts by running the Bellman-Ford algorithm to find the shortest path from the source to the sink. If such a path exists, we augment the flow along this path and update the residual graph accordingly. This process is repeated until no more augmenting paths can be found.  

The resulting maximum flow is then the cardinality of the maximum matching, and the edges with flow are the edges that are part of this matching. And if we wish to compute the maximum cost instead of the minimum cost, all we need to do is negate the cost of each given edge before running the algorithm.

The algorithm runs in $\mathcal{O}(V^2 E^2)$ time, since the Bellman-Ford algorithm runs in $\mathcal{O}(VE)$ time and the DFS that updates the matching runs in $\mathcal{O}(VE)$ time as well, thanks to the speedup by remembering the adjacency lists indices which allows to prune the search space of each DFS.

Implementation: [MCBM - Min/max-cost max-flow](https://github.com/pl3onasm/CLRS/tree/main/algorithms/graphs/MCBM-mcmf/mcmf.c)

