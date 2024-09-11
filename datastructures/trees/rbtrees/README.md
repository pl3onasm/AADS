$\huge{\color{Cadetblue}\text{Red-black Trees}}$

<br/>

A red-black tree is a type of self-balancing binary search tree, which maintains the binary search tree property, and in addition, satisfies the following ${\color{peru}\text{red-black properties}}$:

1. Each node is either red or black.
2. The root is black.
3. Every leaf (NIL node) is black.
4. If a node is red, then both its children are black.
5. For each node, all simple paths from the node to descendant leaves contain the same number of black nodes.

The extra attribute of color for each node allows the red-black tree to maintain a balance between the height of the left and right subtrees of each node. This balance is achieved by performing rotations and color flips on the tree during insertions and deletions, which ensure that the tree remains balanced and its height $h$ always is in $\mathcal{O}(\log{n})$, where $n$ is the number of nodes in the tree. As a result, since the height of the tree is guaranteed to be logarithmic at all times, and the key operations on a BST are performed in $\mathcal{O}(h),$ the search, minimum, maximum, successor, predecessor, insert, and delete operations have a time complexity of $\mathcal{O}(\log{n})$ in the average and worst case.

<br/>

$\Large{\color{darkseagreen}\text{Complexity}}$

| ${\color{cornflowerblue}\text{Operation}}$  | ${\color{cadetblue}\text{Complexity}}$ | 
|:---|:---:|
| ${\color{cornflowerblue}\text{Search}}$     | $\mathcal{O}(\log{n})$ |
| ${\color{cornflowerblue}\text{Insert}}$     | $\mathcal{O}(\log{n})$ |
| ${\color{cornflowerblue}\text{Delete}}$     | $\mathcal{O}(\log{n})$ |
| ${\color{cornflowerblue}\text{Minimum}}$    | $\mathcal{O}(\log{n})$ |
| ${\color{cornflowerblue}\text{Maximum}}$    | $\mathcal{O}(\log{n})$ |
| ${\color{cornflowerblue}\text{Successor}}$  | $\mathcal{O}(\log{n})$ |
| ${\color{cornflowerblue}\text{Predecessor}}$| $\mathcal{O}(\log{n})$ |

<br/>

$\Large{\color{darkseagreen}\text{Example applications}}$

- [student database](application/students.c)

<br/>

$\Large{\color{darkseagreen}\text{Playlist}}$  

[![Red-black trees](https://img.youtube.com/vi/qvZGUFHWChY/0.jpg)](https://www.youtube.com/watch?v=qvZGUFHWChY&list=PL9xmBV_5YoZNqDI8qfOZgzbqahCUmUEin&index=1)

