_________________________________________
*All-Pairs Shortest Paths Problem (APSP)*
_________________________________________

# Problem

Given a weighted graph $G=(V,E)$ with $n = |V|$ vertices and a weight function $w: E \rightarrow \mathbb{R}$, find the shortest paths (i.e. the minimum-weight paths) between all pairs of vertices in $G$. The weight of a path is the sum of the weights of its constituent edges: $w(p) = \sum_{e \in p} w(e)$.

## Johnson's Algorithm

The [Floyd-Warshall algorithm](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/APSP-floyd) works well for dense graphs, but it is not the best solution for sparse graphs. In this case, we can use Johnson's algorithm, which uses both [Bellman-Ford](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/SSSP-bellman) and [Dijkstra](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/SSSP-dijkstra)'s algorithms to find the shortest paths between all pairs of vertices in a graph.

The key idea is to transform the original graph $G$ into a new graph $G'$, and reweight it so that all edge weights turn non-negative and we are able to use Dijkstra's algorithm from each vertex in $G'$ as a subroutine to find the shortest paths between all pairs of vertices in $G$.

The algorithm first creates $G'$ by extending $G$ with a new vertex $s$ and edges $(s,v)$ with weight $0$ for each vertex $v \in V$. It then runs Bellman-Ford's algorithm on $G'$ with source $s$ to find the shortest paths from $s$ to all other vertices. If there is a negative-weight cycle in $G'$, this is reported and the algorithm terminates. Otherwise, we define a new weight function $w': E \rightarrow \mathbb{R}$ by $w'(u,v) = w(u,v) + h(u) - h(v)$, where $h(u)$ is the length of the shortest path from $s$ to $u$ in $G'$. This new weight function is non-negative, and preserves shortest paths in $G$.

Next, we run Dijkstra's algorithm from each vertex $u \in G.V$ (i.e. excluding $s$ and the edges $(s,v)$ belonging to $G'$), using $w'$ as the weight function, to obtain the shortest paths between all pairs of vertices. Finally, in order to recover the shortest paths in the original graph $G$, we need to undo the reweighting, by applying the formula: $w(p) = w'(p) - h(u) + h(v)$, where $u$ and $v$ are the endpoints of the path $p: u \rightarrow v$.

The total running time of Johnson's algorithm using a binary heap is $O(VE \log V)$. If we use a Fibonacci heap, the running time is further reduced to $O(V^2 \log V + VE)$. Thus, the algorithm is asymptotically faster than Floyd-Warshall's algorithm for sparse graphs, but is slower for dense graphs. The implementation below uses a binary heap. Refer to [Dijkstra - Fibonacci heap](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/SSSP-dijkstra/dijkstra-2.c) for an implementation of Dijkstra's algorithm using a Fibonacci heap.

Implementation: [Johnson's algorithm](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/APSP-johnson/johnson.c)
