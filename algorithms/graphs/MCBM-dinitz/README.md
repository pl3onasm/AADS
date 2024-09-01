_____________________________________
$\Large{\color{darkkhaki}\text{\it Maximum cardinality}}$  
$\Large{\color{darkkhaki}\text{\it bipartite matching (MCBM)}}$  
_____________________________________

<br/>

$\huge{\color{Cadetblue}\text{Unweighted MCBM}}$ 

<br/>

$\Large{\color{rosybrown}\text{Problem}}$

Given a bipartite graph $G = (R \cup L, E)$, find a maximum cardinality matching $M$. A bipartite graph is a graph whose vertices can be divided into two disjoint vertex sets $R$ and $L$ such that every edge connects a vertex in $R$ to one in $L$. A matching then is a subset of edges $M \subseteq E$ in which no two edges share a common vertex and a maximum cardinality matching is simply a matching that consists of a maximum subset $M$ of edges in $E$.

<br/>

$\Large{\color{darkseagreen}\text{Adapted Maximum Flow}}$  

We can solve the problem by adapting the maximum flow algorithm. For this, we need to build a flow network from the given bipartite graph so that the edges are directed from $L$ to $R$ and the graph gets extended with a $\color{mediumorchid}{\text{super source }s}$ and a $\color{mediumorchid}{\text{super sink }t}$ with edges from $s$ to every vertex in $L$ and edges from every vertex in $R$ to $t$. The capacity of all the edges in the network is set to 1. We then proceed by running [Dinic's algorithm](../MF-dinitz/dinitz.c) on this network: the resulting maximum flow is the cardinality of the maximum matching, and the edges with flow are the edges that are part of this matching. The algorithm runs in $\mathcal{O}(E \sqrt{V})$ time, thanks to the fact that the graph is unweighted (i.e. all edges have unit capacity) and bipartite, which speeds up the network flow computations.

Implementation: [MCBM - Maxflow](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/MCBM-dinitz/dinitz.c)

<br/>

$\Large{\color{darkseagreen}\text{Video}}$

[![Problem](https://img.youtube.com/vi/GhjwOiJ4SqU/0.jpg)](https://www.youtube.com/watch?v=GhjwOiJ4SqU)