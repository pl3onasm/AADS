# Breadth-first Search (BFS)

BFS is an algorithm for traversing or searching an unweighted graph $G = (V, E)$. It starts at a given source vertex $s$ and discovers every vertex that is reachable from $s$, thus creating a breadth-first tree with root $s$. Exploration of $G$ is done in a systematic fashion: all neighboring vertices are explored first, before moving to the next level neighbors. In other words, it explores the vertices in the order of their distance from the source vertex $s$, level by level, so that all vertices at distance $k$ from $s$ are explored before any vertex at distance $k + 1$. This is why BFS is also called a level-order traversal and why it generates shortest paths from $s$ to all other vertices in the breadth-first tree, where distance is measured by the number of edges in the unique simple path from $s$ to a vertex in the tree.

During the execution of a BFS, each vertex $v$ is in one of three states, which can be maintained in a color attribute: white for undiscovered, gray for discovered but not processed, and black for processed. Initially, all vertices are white, and they remain white if they are not reachable from the source vertex $s$. When a vertex $v$ is first discovered, all its neighbors are colored gray and added to a first-in, first-out queue $Q$, which is used to manage all the gray vertices that have been discovered but whose adjacency lists have not yet been examined. These vertices are on the so-called frontier of the search: they form a "wavefront" moving out from the source vertex $s$. When $v$'s adjacency list has been processed, $v$ is colored black, and $v$ is removed from $Q$. The algorithm processes each vertex in $Q$ exactly once and terminates when $Q$ is empty, at which point it has discovered every vertex that is reachable from the source vertex $s$.

As a BFS discovers vertices, it partitions the edges of $G$ into tree edges and non-tree edges. A tree edge is an edge $(u, v)$ such that vertex $v$ was first discovered by exploring edge $(u, v)$ in the BFS, at which point $u$ became a parent of $v$ so that $v.\pi = u$. Together all these tree edges form a breadth-first tree, which defines a predecessor subgraph $G_\pi = (V_\pi, E_\pi)$, where $E_\pi$ consists of all tree edges and $V_\pi$ consists of all vertices reachable from $s$. By following the predecessor subgraph from any vertex $v$ back to the root $s$, we can reconstruct a shortest path from $s$ to $v$.

The total running time of BFS is $O(V + E)$, since each vertex $v$ is enqueued and dequeued once and each edge is examined once when $v$'s adjacency list is processed, while the total length of all adjacency lists is $\Theta(E)$.

The below program implements BFS on a directed graph $G$, represented by an array of pointers to vertices which each contain a linked list of adjacent vertices. The color attribute has been omitted, as the parent attribute is sufficient to determine whether a vertex has been discovered or not.  
The input consists of the number of vertices $|V|$, followed by the source and the destination vertex, and finally the edges of the graph. The output is the shortest path from the source to the destination vertex, if one exists.

Implementation: [BFS](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/bfs/bfs.c)

Video:

[![Problem](https://img.youtube.com/vi/oDqjPvD54Ss/0.jpg)](https://www.youtube.com/watch?v=oDqjPvD54Ss)
