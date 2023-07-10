____________________________________
*Minimum Spanning Trees (MST)*
____________________________________

## Problem

Given a connected, undirected graph $G = (V, E)$, in which each edge $(u, v) \in E$ has a weight $w(u, v)$, find a spanning tree with minimum total weight. A *spanning tree* is a subset of $n - 1$ edges that forms a tree (i.e. an acyclic connected graph) that includes every vertex. The weight of a spanning tree is the sum of the weights of its edges: $\sum_{(u, v) \in T} w(u, v)$. Note that a spanning tree always has $n - 1$ edges. A *minimum* spanning tree is just a spanning tree with minimum total weight. Also note that a graph may have more than one minimum spanning tree.

## Prim's algorithm

In [Kruskal's algorithm](https://github.com/pl3onasm/AADS/tree/main/algorithms/graphs/MST-kruskal), we start with a forest and then try merging disjoint sets representing trees into bigger ones. Prim's algorithm doesn't start with a forest, but with an arbitrary single vertex and then tries to grow a single tree from it by adding edges to this tree, one by one. At each step, we add the edge with the smallest weight that connects a vertex in the tree to a vertex not in the tree. After $n - 1$ such steps, we then have a minimum spanning tree.

In order to keep track of the nodes that are not in the tree yet and the weights of the edges connecting them to the tree, we use a *min-priority queue*. The key of each vertex in this queue is the weight of the lightest edge connecting it to a vertex in the tree. At each step, we extract the vertex with the smallest key from the priority queue, add it to the MST, and update the keys of its non-tree neighbors. If the key of a neighbor is greater than the weight of the edge connecting it to the vertex we just added to the tree, we update the key of the neighbor to the weight of the edge. We also keep track of the parent of each vertex, so that we can reconstruct the minimum spanning tree after we have found it.

Using a binary min-heap, the running time of this algorithm is $O(E \log V)$, since we need to perform $|E|$ operations on a binary min-heap, each of which takes $O(\log V)$ time, which in the case of dense graphs dominates the number of extract-min operations, which take $O(V \log V)$ time in total.

Implementation: [MST - binary Prim](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mst/prim-1.c)

We can also use a Fibonacci heap to implement Prim's algorithm. This will result in a faster running time, since the Fibonacci heap has a better amortized running time than the binary min-heap. For more information on Fibonacci heaps, see CLRS³, chapter 19. Unfortunately, they were omitted from CLRS⁴. The running time of this version of the algorithm is $O(E + V \log V)$, which is a significant improvement over the $O(E \log V)$ running time of the binary heap version in the case of dense graphs.

Implementation: [MST - Fibonacci Prim](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mst/prim-2.c)
