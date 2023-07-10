_________________________________________
*All-Pairs Shortest Paths Problem (APSP)*
_________________________________________

# Problem

Given a weighted graph $G=(V,E)$ with $n = |V|$ vertices and a weight function $w: E \rightarrow \mathbb{R}$, find the shortest paths (i.e. the minimum-weight paths) between all pairs of vertices. The weight of a path is the sum of the weights of its constituent edges: $w(p) = \sum_{e \in p} w(e)$.

## Bottom-Up DP Solution

First of all, observe that any shortest path consists of at most $r = n-1$ edges. Also note that the shortest path $p$ between any two vertices $i$ and $j$ can be decomposed into a shortest subpath $p_1$ from $i$ to some vertex $k$, plus one more edge from $k$ to $j$. This decomposition can be repeated $r$ times for all $r = 1, 2, \ldots, n-1$, giving us a shortest path $p$ with $r$ edges.  
If we denote the weight of the shortest path between $i$ and $j$ using at most $r$ edges by $d_{ij}^{(r)}$, then $d_{ij}^{(0)}$ is the weight of the shortest path between $i$ and $j$ using at most $0$ edges, which is $0$ if $i = j$ and $\infty$ otherwise. From this base case, we can then extend paths by adding one more edge at each step, until we reach the desired number of edges $r = n-1$. The weight of the shortest path between any two vertices $i$ and $j$ is then given by the minimum between the weight of the shortest path between $i$ and $j$ using at most $r$ edges, and the shortest path between $i$ and $k$ using at most $r-1$ edges. We thus get the following recurrence:

$$
d_{ij}^{(r)} = \min \lbrace d_{ik}^{(r-1)} + w_{kj} : 1 \leq k \leq n \rbrace
$$

The solution after $n-1$ iterations is then $d_{ij}^{(n-1)} = \delta_{ij}$, which is the weight of the shortest path between any two vertices $i$ and $j$. Note that for $k = 1$, we have $D^{1} = W$, which is the adjacency matrix of the graph, where each entry $w_{ij}$ equals the weight of the edge $(i, j)$, or $\infty$ if there is no edge between $i$ and $j$.

The resulting procedure for computing the final distance matrix $\Delta$, containing all the weights of the shortest paths between any two vertices $i$ and $j$, resembles the standard matrix multiplication algorithm, but iterated $n-1$ times (once for each additional edge in the paths) and instead of taking the sum of the products of the corresponding entries, we take for each entry $d_{ij}$ the minimum between the previous path weight and the weight of each possible extension of the path by one edge. This correspondence with matrix multiplication makes that the final distance matrix can in fact be thought of as the adjacency matrix $W$ to the power $n-1$, i.e. $\Delta = W^{n-1}$. Thus, the time complexity of the entire algorithm is $\Theta(n^4)$, since we have to repeat $n-1$ times a procedure that takes $\Theta(n^3)$ time (due to the three nested loops in the matrix multiplication algorithm).

If additionally we want to know the actual vertices lying on the shortest paths between any two vertices $i$ and $j$, we also need to keep track of the consecutive choices for the vertex $k$ in order to be able to reconstruct those paths. This means we need to compute an extra predecessor matrix $P$, where $p_{ij}$ equals vertex number $k$ such that $i \leadsto k \to j$ is the shortest path between $i$ and $j$.

Implementation: [APSP - Bottom-Up DP](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/APSP-floyd/apsp-1.c)

**Note:** The pseudocode in CLRS is needlessly cumbersome, since by inserting a simple if statement in the path extension procedure (to check if the relevant edges are actually in the graph), we can keep using the same matrix $D$ for each iteration and update it in place.
