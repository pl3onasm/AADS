$\huge{\color{Cadetblue}\text{Binary Search Trees}}$

<br/>

A binary search trees (BST) is a type of binary tree that satisfies the ${\color{peru}\text{binary search property}}$: for each node $x$, all nodes in the left subtree of $x$ have keys less than the key of $x$, and all nodes in the right subtree of $x$ have keys greater than the key of $x$. This property allows for efficient search, insert, and delete operations, which have an average time complexity of $\mathcal{O}(\log{n})$. However, in the worst case, the time complexity of these operations can be $\mathcal{O}(n)$, if the tree is unbalanced and degenerates into a linked list. To avoid this, various self-balancing binary search trees have been developed, such as the AVL tree, the [red-black tree](../rbtrees/README.md), and the splay tree.

<br/>

$\Large{\color{darkseagreen}\text{Complexity}}$

| ${\color{cornflowerblue}\text{Operation}}$  | ${\color{cadetblue}\text{Average}}$ | ${\color{cadetblue}\text{Worst-case}}$ |
|:---|:---:|:---:|
| ${\color{cornflowerblue}\text{Search}}$     | $\mathcal{O}(\log{n})$ | $\mathcal{O}(n)$ |
| ${\color{cornflowerblue}\text{Insert}}$     | $\mathcal{O}(\log{n})$ | $\mathcal{O}(n)$ |
| ${\color{cornflowerblue}\text{Delete}}$     | $\mathcal{O}(\log{n})$ | $\mathcal{O}(n)$ |

<br/>

$\Large{\color{darkseagreen}\text{Example applications}}$

- [student database](application/students.c)

<br/>

$\Large{\color{darkseagreen}\text{Video}}$  

[![Binary search trees](https://img.youtube.com/vi/LwpLXm3eb6A/0.jpg)](https://www.youtube.com/watch?v=JfSdGQdAzq8&list=PLDV1Zeh2NRsCYY48kOkeLQ-cg9-eqInzs)
