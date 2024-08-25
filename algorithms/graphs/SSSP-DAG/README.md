_____________________________________
$\Large{\color{darkkhaki}\text{\it Single-source shortest}}$  
$\Large{\color{darkkhaki}\text{\it paths (SSSP)}}$
_____________________________________

<br />

$\Large{\color{rosybrown}\text{Problem}}$

Given a weighted, directed acyclic graph $G = (V,E)$, which may contain negative-weight edges, and a source vertex $s \in V$, find the shortest paths from $s$ to all other vertices in the graph.

<br />

$\Large{\color{darkseagreen}\text{DAG Shortest Paths (DSP)}}$

The [Bellman-Ford algorithm](https://github.com/pl3onasm/CLRS/tree/main/algorithms/graphs/SSSP-bellman-ford) can also be used to find the shortest paths in a weighted, directed *acyclic* graph (DAG), but it is not the most efficient algorithm for this particular task. The DAG shortest paths algorithm is more efficient, as it runs in $\mathcal{O}(V+E)$ time, which is faster than the $\mathcal{O}(VE)$ time of the Bellman-Ford algorithm.

The algorithm works by first performing a [topological sort](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/top-sort) on the vertices, and then relaxing the edges in the linear ordering that was found. Recall that a topological sort is a linear ordering of the vertices that preserves any dependencies between them. As the graph is acyclic, the topological sort is well defined: if $v$ is reachable from $u$, then there is no path from $v$ to $u$, and $u$ must precede $v$ in the topological sort order.  
This means that the shortest path from $s$ to $v$ can indeed be found by relaxing the edges in the order given by the topological sort: the order ensures that the shortest path from $s$ to any $u$ that precedes $v$ in the graph is found before the shortest path from $s$ to $v$, which relies on the shortest path from $s$ to any ancestor of $v$ in the graph.
Once the shortest path to $v$ is found, the algorithm can then move on to the next vertex in the topological sort. In this way, the algorithm only needs to perform $|V|$ relaxations, where each relaxation takes $\mathcal{O}(1)$ time, so that the total running time is $\mathcal{O}(V+E)$. Note that a topological sort is not necessarily unique, but any topological sort is guaranteed to work for this algorithm.

Implementation: [DAG Shortest Paths](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/SSSP-DAG/dsp.c)

<br />

$\Large{\color{darkseagreen}\text{Video}}$

[![DAG Shortest Paths](https://img.youtube.com/vi/TXkDpqjDMHA/0.jpg)](https://www.youtube.com/watch?v=TXkDpqjDMHA)