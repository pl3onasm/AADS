_______________________________________________
*Maximum cardinality bipartite matching (MCBM)*
_______________________________________________

## Problem

Given a bipartite graph $G = (R \cup L, E)$, find a maximum cardinality matching $M$. A bipartite graph is a graph whose vertices can be divided into two disjoint vertex sets $R$ and $L$ such that every edge connects a vertex in $R$ to one in $L$. A matching then is a subset of edges $M \subseteq E$ in which no two edges share a common vertex and a maximum cardinality matching is simply a matching that consists of a maximum subset $M$ of edges in $E$.

In this version of the problem, the edges come with a cost and we are asked to find a maximum cardinality matching $M$ such that the total cost of the edges in $M$ is minimized or maximized.

### Hungarian Algorithm (CLRS⁴ 25.3)

The algorithm was first proposed by Kuhn in 1955 and later refined by Munkres in 1957. It is therefore also known as the Kuhn-Munkres algorithm, but goes by the name of the Hungarian algorithm in honor of two Hungarian mathematicians who contributed to its development.  
The implementation follows the pseudo code as closely as possible, but with some modifications to make it more readable and to allow for the algorithm to be used in a more general context. The latter is achieved by allowing the algorithm to work with negative weights and to find either the minimum or maximum cost matching. The code is also optimized to run in $\mathcal{O}(n^3)$ time, by implementing the optimization presented in problem 25-2 of the book CLRS⁴. This optimization involves the introduction of a slack attribute for each vertex in the graph. CLRS calls it the $\sigma$ attribute, but following the general convention in the literature (see for example, EA - Assignment Problem), we will call it the slack attribute instead. Also, since the label of a vertex already indicates its name, the term height—a much more visually descriptive term—is used for what CLRS and others call the label of a vertex.

Implementation: [MCBM - Kuhn-Munkres](hungarian.c)
