_____________________________________
$\Large{\color{darkkhaki}\text{\it Single-source shortest}}$  
$\Large{\color{darkkhaki}\text{\it paths (SSSP)}}$
_____________________________________

<br />

$\Large{\color{rosybrown}\text{Problem}}$

Given a weighted, directed graph $G = (V, E)$, with non-negative weight function $w : E \to \mathbb{R}$, and a source vertex $s \in V$, find shortest paths from this source to all the other $|V| - 1$ vertices in the graph. A *shortest path* is a path from the source to a given vertex with the smallest total weight, which in itself is defined as the sum of the weights of its constituent edges : $w(p) = \sum_{e \in p} w(e)$.

<br />

$\Large{\color{darkseagreen}\text{Dijkstra's solution}}$

Unlike the [Bellman-Ford](https://github.com/pl3onasm/CLRS/tree/main/algorithms/graphs/SSSP-bellman-ford) and the [DAG shortest paths](https://github.com/pl3onasm/CLRS-in-C/tree/main/algorithms/graphs/SSSP-DAG) algorithms, Dijkstra's algorithm doesn't work for graphs with negative weight edges. It is actually a generalization of the [BFS algorithm](https://github.com/pl3onasm/CLRS-in-C/tree/main/algorithms/graphs/bfs), which also finds shortest paths from a given source vertex to all other vertices, but does so in an unweighted graph in which all edges have the same unit weight so that a shortest path is one with the smallest number of edges.

Dijkstra's algorithm works by maintaining a set $S$ of vertices whose shortest paths from the source $s$ have already been found. Initially, $S$ is empty and all distances are set to $\infty$, except for the source $s$, which has a distance of $0$. At each step, the algorithm finds a vertex $u \in V - S$ whose distance estimate from the source is the smallest among all other vertices $v$ in $V - S$. It then adds $u$ to $S$, and updates the distances of all those $v$ in $V - S$ that are adjacent to $u$, by relaxing the edge $(u, v)$. This procedure is repeated until $S = V$, i.e. until all vertices have been added to the set $S$.

The algorithm is greedy in the sense that at each step, it takes the vertex that is closest to the source $s$, and adds it to the set $S$, without considering the vertices that are farther away from the source. This greedy choice is safe, however, because all the edges are non-negative, resulting in the following ${\color{peru}\text{greedy-choice property}}$: for any vertex $v \in V - S$, the shortest path from $s$ to $v$ is also a shortest path from $s$ to some vertex $u \in S$ followed by the edge $(u, v)$. This property is what makes the algorithm work.

Thanks to this greedy strategy, the algorithm is faster than the Bellman-Ford algorithm. The time complexity using a binary min-heap is $\mathcal{O}(E \log V)$. This is fine for sparse graphs, but for dense graphs, the time complexity can be improved to $\mathcal{O}(E + V \log V)$ using a Fibonacci heap.

Note the strong similarity between Dijkstra's algorithm and [Prim's algorithm](https://github.com/pl3onasm/CLRS/tree/main/algorithms/graphs/MST-prim) for finding a minimum spanning tree of a graph. Both algorithms maintain a min-priority queue of unprocessed vertices, and at each step, they take the vertex $u$ having the smallest key value, add it to the set $S$ of optimal vertices, and update the key values of the vertices adjacent to $u$ by relaxing the edges connecting them to $u$ (if necessary). Both algorithms terminate when the set $S$ contains all vertices in the graph $G$ and both have the same time complexity. The only difference is that Dijkstra's algorithm takes a source vertex $s$ as input and keeps grabbing the vertex with the smallest distance estimate to this particular $s$, whereas Prim's algorithm starts with an arbitrary vertex and keeps grabbing the vertex that is closest to the growing tree, i.e. the vertex with the lightest edge connecting it to the tree grown so far.

Implementation using a binary min-heap: [dijkstra - binheap](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/SSSP-dijkstra/dijkstra-1.c)

Implementation using a Fibonacci heap: [dijkstra - fibheap](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/SSSP-dijkstra/dijkstra-2.c)

<br />

$\Large{\color{darkseagreen}\text{Video}}$

[![Problem](https://img.youtube.com/vi/pSqmAO-m7Lk/0.jpg)](https://www.youtube.com/watch?v=pSqmAO-m7Lk)
