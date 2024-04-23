_________________________________________
*All-Pairs Shortest Paths Problem (APSP)*
_________________________________________

# Problem

Given a weighted graph $G=(V,E)$ with $n = |V|$ vertices and a weight function $w: E \rightarrow \mathbb{R}$, find the shortest paths (i.e. the minimum-weight paths) between all pairs of vertices. The weight of a path is the sum of the weights of its constituent edges: $w(p) = \sum_{e \in p} w(e)$.

## Floyd-Warshall Algorithm

Just like the [algorithms drawing on matrix multiplication](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/APSP-matrixmp), the Floyd-Warshall algorithm is a bottom-up dynamic programming solution to the all-pairs shortest paths problem, but instead of gradually increasing the length of the paths by adding one edge at a time, it tries to find a shorter path between any two vertices $i$ and $j$ by adding one intermediate vertex $k$ at a time. In other words, instead of gradually increasing the upper bound on the number of edges in the shortest paths between any two vertices $i$ and $j$, it gradually increases the upper bound on the number of intermediate vertices in the shortest paths. This is done by trying to find a path between $i$ and $j$ that goes through $k$. If such a path exists, then the distance between $i$ and $j$ is updated to the minimum of the mimimum weight of the path between $i$ and $j$ so far, and the weight of the path through $k$. This is done for all pairs of vertices $i$ and $j$, until we have tried all possible values of $k$.

This leads to the following recurrence relation for the shortest path between any two vertices $i$ and $j$ using at most $k$ intermediate vertices:

$$
d_{ij}^{(k)} =  
\begin{cases}
w_{ij} & \text{if } k = 0 \\
\min \lbrace d_{ij}^{(k-1)}, d_{ik}^{(k-1)} + d_{kj}^{(k-1)} \rbrace  & \text{if } k \geq 1
\end{cases}
$$

After $n$ iterations, we have $d_{ij}^n = \delta_{ij}$, which is the weight of the shortest path between any two vertices $i$ and $j$ in the given graph.

Implementation: [APSP - Floyd-Warshall](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/APSP-floyd/floyd.c)

Notice that the code is very similar to the [algorithms drawing on matrix multiplication](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/APSP-matrixmp), except that here we iterate over the intermediate vertices $k$ first, and then over the pairs of vertices $i$ and $j$. We also omit the outermost loop of $n-1$ or $\lceil \log_2 (n-1) \rceil$ iterations. Thus, the time complexity of the Floyd-Warshall algorithm is able to drop to $\Theta(n^3)$.

**Note**: try switching the order of the loops in the Floyd-Warshall algorithm, for example by iterating over the pairs of vertices $i$ and $j$ first, and then over the intermediate vertices $k$. You will see that this produces wrong results. It shows that, although the code is very similar to the previous solution, the order of the loops is important, and that the Floyd-Warshall algorithm is not a generalization of the matrix multiplication algorithm, but a different interpretation of the APSP problem that leads to a different algorithm with a different time complexity.

Video:

[![Problem](https://img.youtube.com/vi/4NQ3HnhyNfQ/0.jpg)](https://www.youtube.com/watch?v=4NQ3HnhyNfQ)
