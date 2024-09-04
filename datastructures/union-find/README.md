$\huge{\color{Cadetblue}\text{Union find}}$

<br/>

Union find is a data structure that keeps track of a set of elements partitioned into a number of disjoint subsets. It provides two main operations:

- Find: Determine which subset a particular element is in.
- Union: Join two subsets into a single subset.

The union-find data structure is used to solve problems that involve finding connected components in a graph, such as finding the minimum spanning tree of a graph, or determining whether a graph is connected.


<br/>

$\Large{\color{darkseagreen}\text{Complexity}}$

| ${\color{cornflowerblue}\text{Operation}}$  | ${\color{cadetblue}\text{Complexity}}$ |
|:---|:---:|
| ${\color{cornflowerblue}\text{Find}}$     | $\mathcal{O}(\alpha(n))$ |
| ${\color{cornflowerblue}\text{Union}}$     | $\mathcal{O}(\alpha(n))$ |

<br/>

where $\alpha(n)$ is the inverse Ackermann function, which grows extremely slowly and is considered to be a constant for all practical purposes. As a result, the time complexity of the find and union operations is effectively $\mathcal{O}(1)$.

<br/>

$\Large{\color{darkseagreen}\text{Example applications}}$

- [Kruskal's algorithm](../../algorithms/graphs/MST-kruskal/README.md)

<br/>

$\Large{\color{darkseagreen}\text{Playlist}}$  

[![Union find](https://img.youtube.com/vi/ibjEGG7ylHk/0.jpg)](https://www.youtube.com/watch?v=ibjEGG7ylHk&list=PLDV1Zeh2NRsBI1C-mR6ZhHTyfoEJWlxvq)