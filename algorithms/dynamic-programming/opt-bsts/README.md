${\color{Cadetblue}\text{\huge Optimal Binary}}$  
${\color{Cadetblue}\text{\huge Search Trees (OBST)}}$

<br />

${\color{rosybrown}\text{\Large Problem}}$

Given a sorted sequence of $n$ distinct keys $K = \langle k_0, k_1, \ldots, k_{n-1} \rangle$ in increasing order, and a sequence of $n$ probabilities $P = \langle p_0, p_1, \ldots, p_{n-1} \rangle$ that sum to 1, the task is to build an optimal binary search tree (OBST) for $K$.

An OBST is a binary search tree for which the nodes are arranged on levels such that the expected search cost is minimized. The expected search cost of a node is the sum of the probabilities of all the keys in the node's subtree, plus the cost of the node itself. The cost of a node is the depth of the node in the tree. The expected search cost of a node is the sum of the expected search costs of its left and right children, plus the cost of the node itself. The expected search cost of the root is the cost of the root plus the expected search costs of its children. The expected search cost of the OBST is the expected search cost of the root. 

The expected search cost of a node $k_i$ is $d_i + 1$, where $d_i$ is the depth of the node in the tree. The depth of the root is 0. The depth of a node is the number of edges on the path from the root to the node. The expected search cost of the root is $e[1, n]$, where $e[1, n]$ is the expected search cost of the OBST for the keys $k_1, k_2, \ldots, k_n$. 

The expected search cost of the OBST for the keys $k_i, k_{i+1}, \ldots, k_j$ is $e[i, j]$. The expected search cost of the OBST for the keys $k_i, k_{i+1}, \ldots, k_j$ is the sum of the probabilities of the keys in the subtree rooted at $k_i$, plus the expected search cost of the OBST for the keys $k_i, k_{i+1}, \ldots, k_{r-1}$, plus the expected search cost of the OBST for the keys $k_{r+1}, k_{r+2}, \ldots, k_j$, where $r$ is the root of the subtree. 

The expected search cost of the OBST for the keys $k_i, k_{i+1}, \ldots, k_j$ is given by the following recurrence relation:

$$
e[i, j] = \begin{cases}
0 & \text{if } j = i - 1 \\
\min_{i \leq r \leq j} \left\{ e[i, r-1] + e[r+1, j] + w(i, j) \right\} & \text{if } i \leq j
\end{cases}
$$

where $w(i, j) = \sum_{l=i}^{j} p_l$.

The expected search cost of the OBST for the keys $k_1, k_2, \ldots, k_n$ is $e[1, n]$.

The expected search cost of the OBST for the keys $k_1, k_2, \ldots, k_n$ can be computed using dynamic programming. The expected search cost of the OBST for the keys $k_1, k_2, \ldots, k_n$ can be computed in $O(n^3)$ time using dynamic programming.

<br />

${\color{darkseagreen}\text{\Large The key idea}}$

<br />

${\color{darkseagreen}\text{\Large Brute force}}$

<br />

${\color{darkseagreen}\text{\Large Top-down}}$

<br />

${\color{darkseagreen}\text{\Large Bottom-up}}$
