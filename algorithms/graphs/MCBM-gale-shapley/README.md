_____________________________________
$\Large{\color{darkkhaki}\text{\it Maximum cardinality}}$  
$\Large{\color{darkkhaki}\text{\it bipartite matching (MCBM)}}$  
_____________________________________

<br/>

$\huge{\color{Cadetblue}\text{Unweighted preferred MCBM}}$ 

<br/>

$\Large{\color{rosybrown}\text{Problem}}$

Given a bipartite graph $G = (R \cup L, E)$, find a maximum cardinality matching $M$. A bipartite graph is a graph whose vertices can be divided into two disjoint vertex sets $R$ and $L$ such that every edge connects a vertex in $R$ to one in $L$. A matching then is a subset of edges $M \subseteq E$ in which no two edges share a common vertex and a maximum cardinality matching is simply a matching that consists of a maximum subset $M$ of edges in $E$.

<br/>

$\Large{\color{darkseagreen}\text{Gale-Shapley algorithm}}$

In this version of the MCBM problem, also known as the stable-marriage problem, we are given an unweighted $\color{orchid}{\text{complete}}$ bipartite graph $G = (R \cup L, E)$, meaning that there is an edge with unit weight from every vertex in $L$ to every vertex in $R$ and vice versa. We are also given a $\color{orchid}{\text{preference list}}$ for each vertex $u$ in $L \cup R$ which specifies the order in which the vertices in the other vertex set, different from the one $u$ itself belongs to, are preferred by $u$. The goal then is to find a maximum cardinality matching $M \subseteq E$ in which the preference list of each vertex in $L$ and $R$ is optimally satisfied.  

A matching is said to be $\color{peru}{\text{stable}}$ if no better matching can be found by swapping two matched pairs. If there is such a pair $(u, v)$ in the matching $M$, then we say that $u$ and $v$ are $\color{peru}{\text{blocking}}$ each other. The aim is to find a maximum cardinality stable matching, and this can be done using a simple greedy algorithm known as the Gale-Shapley algorithm which is guaranteed to find a stable matching in $\mathcal{O}{(VE)}$ time.

The algorithm is based on the principle of $\color{cornflowerblue}{\text{proposals}}$ and $\color{cornflowerblue}{\text{rejections}}$. We start with an empty matching and then we let the vertices in $L$ propose to the vertices in $R$ according to the proposer's preference list. If the vertex in $R$ is unmatched, then it accepts the proposal and the matching is updated accordingly. If the vertex in $R$ is already matched, then it checks if the proposer is preferred over its current match according to its own preference list. If this is the case, then the vertex in $R$ accepts the proposal and the matching is updated accordingly, otherwise the proposal is rejected and the matching remains unchanged. The algorithm terminates when no more proposals can be made and the matching is stable.

A note on the implementation: since the graph is implemented as a hash table of vertices and a random first vertex is chosen to start classifying the vertices as proposers (in $L$) and receivers (in $R$), this means that the output may vary from run to run. This should not be a problem, as the algorithm is guaranteed to find a stable matching regardless of the order in which the vertices are processed. What you probably will notice if you compare the output of different runs is that the matching has a $\color{orchid}{\text{bias}}$ towards the proposers, i.e. the vertices in $L$: the preference lists of the proposers weigh more heavily than those of the receivers. In other words, the proposers are more likely to get a match with a receiver that is higher up in their preference list than the other way around.

Implementation: [MCBM - Gale-Shapley](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/MCBM-gale-shapley/gale-shapley.c)

<br/>

$\Large{\color{darkseagreen}\text{Video}}$

[![Problem](https://img.youtube.com/vi/RLOWSkL-V8M/0.jpg)](https://www.youtube.com/watch?v=RLOWSkL-V8M)
