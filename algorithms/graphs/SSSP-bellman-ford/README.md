_____________________________________
$\Large{\color{darkkhaki}\text{\it Single-source shortest}}$  
$\Large{\color{darkkhaki}\text{\it paths (SSSP)}}$
_____________________________________

<br />

$\Large{\color{rosybrown}\text{Problem}}$

Given a weighted, directed graph $G = (V,E)$, which may contain negative-weight edges and negative-weight cycles, and a source vertex $s \in V$, find the shortest paths from $s$ to all other vertices in the graph.

<br />

$\Large{\color{darkseagreen}\text{Bellman-Ford algorithm (BFA)}}$

The Bellman-Ford algorithm deploys a bottom-up [dynamic programming](https://github.com/pl3onasm/CLRS/tree/main/algorithms/dynamic-programming) approach for finding the shortest paths in the particular case where $G$ may contain negative edges and negative-weight cycles. It works by repeatedly relaxing all the edges of the graph. Such a relaxation of an edge $(u,v)$ consists of potentially updating the shortest path estimate and current predecessor of $v$ using $u$ as an intermediate vertex. After $|V|-1$ iterations, the shortest paths are found, and the algorithm concludes by checking for negative-weight cycles. If there are none, the algorithm returns the shortest paths and their weights. Otherwise, the algorithm returns a message that the graph contains a negative-weight cycle, as the shortest paths are undefined in this case.

Detecting negative-weight cycles is done by iterating once more over all the edges and checking if any distance estimate can still be updated. If there is such an estimate, then that means that there is a negative-weight cycle in the graph. This is because the longest possible path without a cycle has $|V|-1$ edges, and if the first $|V|-1$ iterations of the algorithm did not find a negative-weight cycle, then the $|V|$-th iteration will not update any distance estimates. However, if there is a negative-weight cycle, then the $|V|$-th iteration will update some distance estimates, as the cycle can be repeated as many times as desired.

The running time of this algorithm is $\mathcal{O}(V^2 + VE)$, which becomes $\mathcal{O}(VE)$ if the graph is dense. This is because the first part dominates the running time, as the algorithm performs $|V|-1$ iterations, and each iteration takes $\mathcal{O}(V + E)$ time.

Implementation: [Bellman-Ford](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/SSSP-bellman/bfa.c)

<br />

$\Large{\color{darkseagreen}\text{Video}}$

[![Problem](https://img.youtube.com/vi/lyw4FaxrwHg/0.jpg)](https://www.youtube.com/watch?v=lyw4FaxrwHg)
