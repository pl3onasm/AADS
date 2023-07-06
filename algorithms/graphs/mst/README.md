# Minimum spanning trees (MST)

## Problem

Given a connected, undirected graph $G = (V, E)$, in which each edge $(u, v) \in E$ has a weight $w(u, v)$, find a spanning tree with minimum total weight. A *spanning tree* is a subset of $n - 1$ edges that forms a tree (i.e. an acyclic connected graph) that includes every vertex. The weight of a spanning tree is the sum of the weights of its edges: $\sum_{(u, v) \in T} w(u, v)$. Note that a spanning tree always has $n - 1$ edges. A *minimum* spanning tree is just a spanning tree with minimum total weight. Also note that a graph may have more than one minimum spanning tree.

## Kruskal's algorithm

In Kruskal's algorithm, we start with a forest of trees, where each tree is a single vertex, and sort the edges in increasing order of their weights. We then try adding an edge to the forest, that is, we try merging the two trees that the edge connects. We do this for all the edges, one by one, in increasing order of their weights. If adding an edge would create a cycle, we don't add it and move on to the next edge. We continue until we have added $n - 1$ edges and have found a minimum spanning tree.

The running time of this algorithm is $O(E \log E)$, since we need to sort the edges in increasing order of their weights, and we need to perform $|E|$ operations on a disjoint-set data structure, each of which takes $O(\alpha(V))$ time, where $\alpha(V)$ is the inverse Ackermann function. This is a very slow function, but it is still bounded by a constant, so we can ignore it. Thus, the running time of Kruskal's algorithm is $O(E \log E)$, which is the same as $O(E \log V)$, since $E \leq V^2$.

In order to implement Kruskal's algorithm, we need a data structure that can efficiently perform the following operations:

* Find the set that a given element belongs to
* Merge two sets into one
* Create a new set containing a single element

The most efficient data structure for this is the *disjoint-set data structure*. We can implement this using a *union-find* data structure. This data structure is a forest of trees, where each tree represents a set. Each node in the tree has a pointer to its parent and a rank, which is the length of the longest path from the node to a leaf. So a singleton set is represented by a tree with a single node, whose parent is itself and whose rank is 0. The union operation is performed by merging the trees with the smaller rank into the tree with the larger rank. The find operation is performed by following the parent pointers until we reach a node whose parent is itself. This node is the root of the tree, i.e. the representative element of the set.  
Implementing the disjoint-set data structure this way results in a better running time than implementing it using linked lists. This is mainly due to the use of two heuristics: *path compression* and *union by rank*. Path compression is achieved by making each node in the path from the node to the root point directly to the root. Union by rank is performed by merging the tree with the smaller rank into the tree with the larger rank, and adding 1 to the rank of the resulting tree if the ranks are equal.

For a detailed explanation of the union-find data structure and the heuristics, see CLRS³ 21.3, or CLRS⁴ 19.3.

Implementation: [MST - Kruskal](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mst/mst-1.c)

## Prim's algorithm

In Prim's algorithm, we don't start with a forest and then try merging disjoint sets representing trees into bigger ones, but we start with an arbitrary single vertex and try to grow a single tree from it by adding edges to this tree, one by one. At each step, we add the edge with the smallest weight that connects a vertex in the tree to a vertex not in the tree. After $n - 1$ such steps, we then have a minimum spanning tree.

In order to keep track of the nodes that are not in the tree yet and the weights of the edges connecting them to the tree, we use a *min-priority queue*. The key of each vertex in this queue is the weight of the lightest edge connecting it to a vertex in the tree. At each step, we extract the vertex with the smallest key from the priority queue, add it to the MST, and update the keys of its non-tree neighbors. If the key of a neighbor is greater than the weight of the edge connecting it to the vertex we just added to the tree, we update the key of the neighbor to the weight of the edge. We also keep track of the parent of each vertex, so that we can reconstruct the minimum spanning tree after we have found it.

Using a binary min-heap, the running time of this algorithm is $O(E \log V)$, since we need to perform $|E|$ operations on a binary min-heap, each of which takes $O(\log V)$ time, which in the case of dense graphs dominates the number of extract-min operations, which take $O(V \log V)$ time in total.

Implementation: [MST - binary Prim](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mst/mst-2.c)

We can also use a Fibonacci heap to implement Prim's algorithm. This will result in a faster running time, since the Fibonacci heap has a better amortized running time than the binary min-heap. For more information on Fibonacci heaps, see CLRS³, chapter 19. Unfortunately, they were omitted from CLRS⁴. The running time of this version of the algorithm is $O(E + V \log V)$, which is a significant improvement over the $O(E \log V)$ running time of the binary heap version in the case of dense graphs.

Implementation: [MST - Fibonacci Prim](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/mst/mst-3.c)
