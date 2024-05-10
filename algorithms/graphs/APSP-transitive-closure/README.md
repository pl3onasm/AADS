# ${\color{Cadetblue}\text{Transitive closure of}}$ ${\color{Cadetblue}\text{a graph (TRC)}}$

## ${\color{rosybrown}\text{Problem}}$

Given a directed graph $G = (V,E)$, find the transitive closure of the graph, i.e. a matrix $M$ that indicates whether there is a $\color{orchid}{\text{path}}$ (direct or indirect) from vertex $i$ to vertex $j$ in the graph. In other words, each entry $m_{ij}$ should show whether $j$ is reachable from $i$ or not. This is why the obtained matrix is also called the $\color{peru}{\text{reachability matrix}}$.

## ${\color{darkseagreen}\text{Using Floyd-Warshall}}$

We can obtain the transitive closure of a graph by running the [Floyd-Warshall algorithm](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/APSP-floyd) on the given graph. If there is a path from $i$ to $j$, then the distance matrix $D$ will have a finite value at the position $d_{ij}$. Otherwise, the distance will be infinite. The result is a matrix $M$ where $m_{ij}$ is either infinite if there is no path from $i$ to $j$ or finite with $m_{ij}$ indicating the length of the shortest path from $i$ to $j$ in terms of the number of edges needed to go from $i$ to $j$.

Time complexity: $O(V^3)$

Implementation: [Transitive Closure - FW](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/APSP-tr-closure/trc-1.c)

## ${\color{darkseagreen}\text{Alternative solution}}$

If we are merely interested in the existence of a path from $i$ to $j$, the previous method can be optimized by using a boolean matrix. This is done by modifying the Floyd-Warshall algorithm: instead of storing the distance between vertices, we store a boolean value indicating whether there is a path between them. More specifically, we replace the operations $\textit{min}$ and $+$ with the logical operations $\lor$ and $\land$ in the previous algorithm. The result is then a matrix $M$ where $m_{ij}$ is either 0 if there is no path from $i$ to $j$ or 1 in case there is such a path.

Time complexity: $O(V^3)$

Implementation: [Transitive Closure - Optimized](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/APSP-tr-closure/trc-2.c)

## ${\color{darkseagreen}\text{Note}}$

The book CLRS always includes the edge $(i,i)$ in the graph, which is why the diagonal of the matrix is always 1. Technically, this is taking the reflexive-transitive closure of the graph. If we are only interested in the transitive closure, this can be done by initializing the matrix accordingly.
