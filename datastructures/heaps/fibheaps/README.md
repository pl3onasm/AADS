$\huge{\color{Cadetblue}\text{Fibonacci heaps}}$  

<br/>

Just like any other heap, the Fibonacci heap comes in two flavors: a ${\color{peru}\text{min-heap}}$ or a ${\color{peru}\text{max-heap}}$. It is also called a ${\color{peru}\text{mergeable heap}}$, meaning that it supports the union operation, which merges two heaps into one. The Fibonacci heap is a collection (or forest) of rooted trees, placed in a circular doubly linked list. Each of these trees is a min-heap or a max-heap, depending on the flavor of the Fibonacci heap, and their roots are pointers to the minimum or maximum element in the tree. The Fibonacci heap is also a ${\color{peru}\text{lazy data structure}}$, meaning that it does not always maintain the heap property, but only does so when necessary. This allows for faster operations, but also makes the Fibonacci heap more complex than other heap data structures.

<br/>

$\Large{\color{darkseagreen}\text{Complexity}}$

| ${\color{cornflowerblue}\text{Operation}}$  | ${\color{cadetblue}\text{Complexity}}$ |
|:---|:---:|
| ${\color{cornflowerblue}\text{Push}}$     | $\mathcal{O}(1)$ |
| ${\color{cornflowerblue}\text{Pop}}$| $\mathcal{O}(\log{n})$ |
| ${\color{cornflowerblue}\text{Update-key}}$| $\mathcal{O}(1)$ |
| ${\color{cornflowerblue}\text{Delete}}$     | $\mathcal{O}(\log{n})$ |
| ${\color{cornflowerblue}\text{Union}}$      | $\mathcal{O}(1)$ |
| ${\color{cornflowerblue}\text{Contains}}$    | $\mathcal{O}(1)$ |
| ${\color{cornflowerblue}\text{Peek}}$    | $\mathcal{O}(1)$ |


<br/>

The mentioned complexities are ${\color{peru}\text{amortized}}$, i.e. the average time taken per operation over a sequence of operations. The Fibonacci heap is a data structure that supports the above operations in amortized constant time, except for the delete and pop operations, which take $\mathcal{O}(\log {n})$ time.  

The given implementation uses a map to keep track of the nodes in the heap, acting as an interface between the heap and the user data and allowing for updating the keys of the nodes in constant time. This, however, also means that the union operation takes more than constant time, as it involves merging the maps of the two heaps, resulting in a time complexity of $\mathcal{O}(m)$, where $m$ is the number of nodes in the smaller-sized heap. This could be avoided by storing a handle to the heap in the user data, thus eliminating the need for a map and making the union operation constant time, but would make the implementation less practical or user-friendly.

<br/>

$\Large{\color{darkseagreen}\text{Example applications}}$

- [Dijkstra's algorithm](../../../algorithms/graphs/SSSP-dijkstra/README.md)
- [Prim's algorithm](../../../algorithms/graphs/MST-prim/README.md)

<br/>

$\Large{\color{darkseagreen}\text{Video}}$

[![Fibonacci heaps](https://img.youtube.com/vi/6JxvKfSV9Ns/0.jpg)](https://www.youtube.com/watch?v=6JxvKfSV9Ns)