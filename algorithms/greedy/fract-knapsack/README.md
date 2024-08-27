$\huge{\color{Cadetblue}\text{Fractional knapsack}}$  

<br />

${\Large\color{rosybrown}\text{Problem}}$

We are given a set $S$ of $n$ items, each with a weight $w_i$ and a value $v_i$. Also given is a knapsack of a fixed capacity $W$. Our task is to find a maximum-valued subset of $S$ such that the total weight of the subset is less than or equal to $W$. Unlike the [0-1 knapsack problem](https://github.com/pl3onasm/CLRS/tree/main/algorithms/dynamic-programming/knapsack), we are allowed to break items (take fractional amounts of them) in order to maximize the total value of the knapsack.

<br />

${\Large\color{darkseagreen}\text{Linearithmic greedy}}$

A greedy strategy to attack this problem is to first sort the items in descending order of their value per unit weight. We then start adding items to the knapsack in this order, taking as much of each item as possible without exceeding the knapsack capacity. If we can't add the item in its entirety anymore, we simply add a fraction of it such that the knapsack is filled to capacity.

The ${\color{peru}\text{greedy choice}}$ made at each step is to add the item with the highest value per unit weight among the remaining items. This choice is locally optimal, since it maximizes the value of the knapsack at that step. The choice is also safe to make, since the items are sorted in descending order of their value per unit weight, so we know that by the end of the process, we will have chosen the optimal subset of items.

Implementation: [FKP - Linearithmic greedy](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/fract-knapsack/fkp-1.c)  

Note: this greedy approach works for the fractional knapsack problem, but not for the [0-1 knapsack problem](https://github.com/pl3onasm/CLRS/tree/main/algorithms/dynamic-programming/knapsack), where items cannot be broken into fractions. The main reason why the greedy approach works for the fractional knapsack problem is that in this case, the greedy choice made at each step is guaranteed to be ${\color{peru}\text{globally optimal}}$. This is not the case for the 0-1 knapsack problem, as items can only be fully included or excluded from the knapsack, so that when you get to the next step, you may find that the greedy choice you made in one of the previous steps to fully include the item with the highest value per unit weight was not optimal after all, because you are now left with empty space in the knapsack that could have been filled with a heavier item of lower value per unit weight that, combined with the other items in the knapsack, would have resulted in a higher total value of the knapsack. Thus, in the 0-1 case, when considering to include an item in the knapsack, you are forced to compare the solution of including the item with the solution of not including it before you can safely make a decision that is globally optimal.

<br />

${\Large\color{darkseagreen}\text{Linear greedy (CLRS Ex 16/15.2-6)}}$  

The greedy approach described above works fine, but can be improved upon. The bottleneck of the algorithm is the sorting step, which runs in $\mathcal{O}(n\log n)$ time and dominates the overall time complexity. This storting step, however, is not necessary.  

The idea here is based on the [quickselect algorithm](https://github.com/pl3onasm/CLRS/tree/main/algorithms/divide-and-conquer/quickselect), but instead of searching for the $i$-th smallest element in the array, we are now searching for an optimal pivot item that meets two conditions:
- the items to its left are the most valuable items in the list, and the items to its immediate right are the items with equal value, followed by the items with lower value 
- the knapsack can be filled to capacity with the items to the left of the pivot, plus as much of the items with equal value to its immediate right as needed to complete the knapsack

To achieve this, we first ${\color{peru}\text{partition}}$ the items list around a randomly chosen pivot item, such that by the end of this partitioning process, we have the following three sets in their correct order: a set $S_G$ of items with a value per unit weight greater than that of the pivot item and a total weight of $W_G$, followed by a set $S_E$ of items with a value per unit weight equal to that of the pivot item and a total weight of $W_E$, and finally a set $S_L$ of items with a value per unit weight less than that of the pivot item and a total weight of $W_L$.  

We then proceed to compare the found set weights with the remaining capacity of the knapsack. If $W_G > W$, we have chosen more high-value items than can fit in the knapsack, so we discard the items in $S_L$ and recurse on the set $S_G$. If, on the other hand, $W_G + W_E < W$, we have not chosen enough high-value items to fill the knapsack, so we take all items in $S_G$ and $S_E$ and recurse on the set $S_L$ with a knapsack capacity reduced by $W_G + W_E$. Finally, if $W_G \leq W \leq W_G + W_E$, we take all items in $S_G$ and as much of the items in $S_E$ as needed to fill the knapsack to capacity, and we are done.

Just like the quickselect algorithm, the complexity of this algorithm is expected to be $\mathcal{O}(n)$ on average. It can, however, be made to run in worst-case linear time by modifying the partitioning process and choose the pivot item in a deterministic way as described under the quickselect algorithm.

Implementation: [FKP - Linear Greedy](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/fract-knapsack/fkp-2.c)  
