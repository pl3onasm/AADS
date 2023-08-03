# Depth-first search (DFS)

Unlike [BFS](https://github.com/pl3onasm/CLRS-in-C/tree/main/algorithms/graphs/bfs), DFS explores a graph $G$ all the way down to the deepest node before backtracking and exploring the branches that were left unexplored while going down the tree. In order to avoid getting trapped in cycles, DFS can be made to keep track of the nodes that have already been visited by means of a color attribute for each node, which can be either white, gray or black. White nodes are unvisited, gray ones are being explored in the current DFS tree and black ones have already been explored.

A DFS traversal sets out to explore all white nodes in the graph, and for each one it can find, it calls the *DFS-Visit* procedure, which will explore all nodes that are reachable from the current node, and will mark them as gray and black as it explores them. It will also set the parent attribute of each node to the node that discovered it, generating a predecessor subgraph, which can be used to reconstruct the path from the source node to any other node that is reachable from it.

If the call to *DFS-Visit* doesn't explore all nodes in the graph, it will be called again on a different white node until V is exhausted. This is why a DFS generates multiple depth-first trees, a so-called depth-first forest, if the graph is disconnected. In other words, if there are multiple DFS trees, then they are disjoint, meaning that they don't share any nodes or edges. These trees represent the different components of the graph.

During the execution of *DFS-Visit*, nodes also get a discovery and a finish time, which are the times at which they are first discovered and the time at which they are finished being explored, respectively. These times along with the color state of the nodes can be used to classify edges $(u, v)$ in the graph $G$ as they are explored:

- If $v$ is white, then $v$ is discovered by $u$ and $(u, v)$ is a tree edge.  
  A tree edge is an edge that is part of the DFS tree.
- If $v$ is gray, then $v$ is an ancestor of $u$ and $(u, v)$ is a back edge.  
  A back edge is an edge that connects a node to an ancestor in the DFS tree.
- If $v$ is black and $u.d < v.d$, then $(u, v)$ is a forward edge.  
  A forward edge is an edge that connects a node to a descendant in the DFS tree.
- If $v$ is black and $u.d > v.d$, then $(u, v)$ is a cross edge.  
  A cross edge is an edge that is neither of the previous types.  
  It can go between nodes in the same DFS tree or between nodes in different DFS trees.
  
This classification of edges can be used to get information about the structure of a graph. For example, if a DFS on a directed graph $G$ produces no back edges, then $G$ is a DAG (directed acyclic graph).

The total running time of DFS is $\Theta(V + E)$. The program below implements a DFS to classify edges in a given graph. It outputs the discovery and finish times of each node, as well as the classification of each edge. Note that these results are not unique, as the order in which nodes are explored doesn't have to be unique and may create a different depth-first forest, meaning that another implementation or a different input order could produce different results.

Implementation: [DFS](https://github.com/pl3onasm/AADS/blob/main/algorithms/graphs/dfs/dfs.c)
