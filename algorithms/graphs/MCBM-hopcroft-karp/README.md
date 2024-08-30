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

$\Large{\color{darkseagreen}\text{Hopcroft-Karp}}$

This algorithm actually does the same thing as the previous one, but is presented in a different way. It is based on the idea of finding M-alternating paths to repeatedly increment the cardinality of the matching $M$ until no more such paths can be found. The algorithm is named after John Hopcroft and Richard Karp, who introduced it in 1973.  
The algorithm starts with an empty matching and then tries to repeatedly find an M-augmenting path. Such paths, however, take on a very particular form: they need to start from an unmatched vertex in $L$ and end at an unmatched vertex in $R$, while alternating between edges that are not part of the current matching and edges that are. These so-called $\color{mediumorchid}{\text{M-alternating paths}}$ can be found using a breadth-first search. If any are found, we update the current matching by adding the new path edges to $M$ and removing the edges from $M$ that are shared by the path, i.e. we take the $\color{mediumorchid}{\text{symmetric difference}}$ of the edges in the matching that was found so far and the edges in the newly found path. The latter is accomplished by running a DFS: it follows the edges in the paths that were marked by the BFS and then updates the matching $M$ accordingly if it is indeed an augmenting path. This whole process is repeated until no more augmenting paths can be found. The algorithm runs in $\mathcal{O}(E \sqrt{V})$ time, which is the same as the previous approach.

Implementation: [MCBM - Hopcroft-Karp](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/MCBM-hopcroft-karp/hck.c)

<br/>

$\Large{\color{darkseagreen}\text{Video}}$

[![Problem](https://img.youtube.com/vi/CSUEVu-qUgM/0.jpg)](https://www.youtube.com/watch?v=CSUEVu-qUgM)
