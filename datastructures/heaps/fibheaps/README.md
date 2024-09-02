$\huge{\color{Cadetblue}\text{Fibonacci heaps}}$  

<br/>

This data structure comes in two flavors: a ${\color{peru}\text{min-heap}}$ or a ${\color{peru}\text{max-heap}}$. It is also called a ${\color{peru}\text{mergeable heap}}$, meaning that two Fibonacci heaps can be merged into a single Fibonacci heap in constant amortized time. The Fibonacci heap is a collection (or forest) of rooted trees, placed in a circular doubly linked list. Each of these trees is a min-heap or a max-heap, depending on the flavor of the Fibonacci heap, and their roots are pointers to the minimum or maximum element in the tree. The Fibonacci heap is also a ${\color{peru}\text{lazy data structure}}$, meaning that it does not always maintain the heap property, but only does so when necessary. This allows for faster operations, but also makes the Fibonacci heap more complex than other heap data structures.

<br/>

$\Large{\color{darkseagreen}\text{Example applications}}$

- [Dijkstra's algorithm](../../../algorithms/graphs/SSSP-dijkstra/dijkstra-2.c)
- [Prim's algorithm](../../../algorithms/graphs/MST-prim/prim-2.c)

<br/>

$\Large{\color{darkseagreen}\text{Complexity}}$

| ${\color{cornflowerblue}\text{Operation}}$  | ${\color{cadetblue}\text{Complexity}}$ |
|:---|:---:|
| ${\color{cornflowerblue}\text{Push}}$     | $\mathcal{O}(1)$ |
| ${\color{cornflowerblue}\text{Pop-top}}$| $\mathcal{O}(\log{n})$ |
| ${\color{cornflowerblue}\text{Update-key}}$| $\mathcal{O}(1)$ |
| ${\color{cornflowerblue}\text{Delete}}$     | $\mathcal{O}(\log{n})$ |
| ${\color{cornflowerblue}\text{Union}}$      | $\mathcal{O}(1)$ |

<br/>

The mentioned complexities are ${\color{peru}\text{amortized}}$, i.e. the average time taken per operation over a sequence of operations. The Fibonacci heap is a data structure that supports the above operations in amortized constant time, except for the delete and pop operations, which take $\mathcal{O}(\log {n})$ time. 

<br/>

$\Large{\color{darkseagreen}\text{Video}}$

[![Fibonacci heaps](https://img.youtube.com/vi/6JxvKfSV9Ns/0.jpg)](https://www.youtube.com/watch?v=6JxvKfSV9Ns)