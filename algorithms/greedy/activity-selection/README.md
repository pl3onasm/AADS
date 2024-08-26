$\huge{\color{Cadetblue}\text{Activity-sel} \text{{ection problem}}}$  

<br/>

$\Large{\color{rosybrown}\text{Problem}}$

Given is a set of $n$ competing activities $A = \lbrace a_1, a_2, \dots, a_n \rbrace$, where each activity $a_i$ is defined by a half-open interval $[s_i, f_i)$, where $s_i$ is the start time and $f_i$ is the finish time. Our task is to select a maximum-size subset of mutually compatible activities. Two activities $a_i$ and $a_j$ are ${\color{peru}\text{compatible}}$ if the intervals $[s_i, f_i)$ and $[s_j, f_j)$ don't overlap, i.e. $s_j \geq f_i$ or $s_i \geq f_j$. We assume that the activities are already sorted by monotonically increasing finish time $f_i$.

<br/>

$\Large{\color{darkseagreen}\text{DP approach (CLRS Ex 15.1-1)}}$

The problem can be solved using dynamic programming, since it exhibits optimal substructure, and the subproblems overlap.

- The ${\color{peru}\text{optimal substructure}}$ resides in the fact that an optimal solution to the entire problem consists of optimal solutions to its subproblems. If we select a certain activity to be included in the solution, then the problem is split into two subproblems: the set of activities that finish before the selected activity, and the set of activities that start after the selected activity has finished, and the optimal solution to the entire problem is then the union of the optimal solutions to the two subproblems, plus the selected activity.
- The ${\color{peru}\text{overlap}}$ has to do with the fact that we don't know a priori which activity to select in order to get a maximum-size subset of mutually compatible activities. There are multiple activities that can be included in the solution, and we need to try all of them in order to find the global optimal solution. This means that we need to solve the same subproblem multiple times, which is where the overlap comes from.

The problem very much resembles the [matrix-chain multiplication problem](https://github.com/pl3onasm/CLRS/tree/main/algorithms/dynamic-programming/matrix-chain-mult) of multiplying a sequence of matrices $\langle A_1, A_2, \ldots, A_n \rangle$, where for each pair of indices $i$ and $j$, we need to try all possible split point indices $k$ in order to find the optimal solution, the optimal parenthesization, which minimizes the number of scalar multiplications for the subproblem $A_{ij}$ of multiplying the matrices $A_i \times A_{i+1} \times \ldots \times A_j$.

In the case of the activity-selection problem, we are given a set of activities $S = \lbrace a_1, a_2, \ldots, a_n \rbrace$, sorted by monotonically increasing finish time, and for each pair of indices $i$ and $j$, we need to try all possible activity indices $k$ in order to determine which activity to include so that we get the maximum-size subset of mutually compatible activities for the subproblem $S_{ij}$ of selecting activities from $S$ that start after activity $a_i$ finishes and finish before activity $a_j$ starts.

Thus, we can define a function $s(i,j)$ that returns the maximum size of the subset of mutually compatible activities for the subproblem $S_{ij}$, by means of the following recursive formula:

$$\color{darkslateblue}\boxed{\color{rosybrown}\space
s(i,j) =  
\begin{cases}
0 & \scriptsize \text{if } S_{ij} = \emptyset \\
\text{max} \lbrace s(i,k) + s(k,j) + 1 \space\\
\qquad \space : \space i < k < j \rbrace & \scriptsize \text{if } S_{ij} \neq \emptyset
\end{cases}\space}
$$

<br/>

The base case occurs when the subproblem $S_{ij}$ is empty, i.e. $i \geq j$, in which case the size of the subset of mutually compatible activities is $0$. Otherwise, we need to try all possible activity indices $k$ in order to find the optimal choice of activity to include in the solution. The optimal solution is then the maximum of the sum of the optimal solutions to the two subproblems, plus the selected activity. Eventually, the solution to the entire problem is then the optimal solution to the subproblem $S_{1n}$, which is the maximum size of the subset of mutually compatible activities for the entire set of activities $S$.

Implementing this recursive formula by means of a bottom-up DP approach is straightforward: we consider all possible subproblems in a bottom-up fashion, starting from the smallest subproblems, and build our way up to the optimal solution for the entire problem. Its time complexity is $\mathcal{O}(n^3)$, just like in the case of the matrix-chain multiplication problem. If we also want to reconstruct the optimal solution, we need to keep track of the selected activities, which can be done by means of an auxiliary table that stores their indices in the original set of activities.

In order to simplify the implementation, we can make use of ${\color{peru}\text{sentinel activities}}$, one with start time $0$ and finish time $0$, and another one with start time $\infty$ and finish time $\infty$. This way, we can avoid having to check for the base case in the recursive formula, and we can just start the iteration from the first activity, and end it with the last activity.

Implementation: [ASP - Bottom-up DP](https://github.com/pl3onasm/AADS/blob/main/algorithms/greedy/activity-selection/asp-1.c)

<br/>

$\Large{\color{darkseagreen}\text{Greedy approach}}$

While the activity-selection problem exhibits optimal substructure, it also has the property that the greedy choice is optimal. The ${\color{peru}\text{greedy choice}}$ here is not to try all possible values of $k$ in order to find the optimal solution, but to select whichever activity finishes first, and then continue to solve the subproblem of selecting a maximum-size subset of mutually compatible activities from the set of activities that start after the selected activity finishes.  

Since the activities are sorted by finish time, we can just start by selecting the first activity, and then continue down the list, selecting each activity that starts after the previously selected activity has finished. This way, we can be sure that the selected activities are mutually compatible, and that we get a maximum-size subset of mutually compatible activities. A linear scan of the sorted list of activities is sufficient to find the optimal solution, and so the time complexity is $\mathcal{O}(n)$.

This kind of ${\color{peru}\text{top-down design}}$ is typical for greedy algorithms: we just make the greedy choice, the one that seems locally optimal, and then go on to solve the remaining subproblem to ultimately come to a global optimal solution. This is in contrast to the bottom-up approach used for DP, where we start by solving the smallest subproblems, so we can make an optimal choice based on the optimal solutions to the subproblems, and thus build our way up to an optimal solution for the entire problem.

There are two ways to implement this top-down greedy approach: recursively and iteratively. For the recursive version, we still need one sentinel activity, with start time $0$ and finish time $0$, so we can avoid having to check for the base case. For the iterative version, we don't need any sentinel activities.

Recursive implementation: [ASP - Recursive greedy](https://github.com/pl3onasm/AADS/blob/main/algorithms/greedy/activity-selection/asp-2.c)  

Iterative implementation: [ASP - Iterative greedy](https://github.com/pl3onasm/AADS/blob/main/algorithms/greedy/activity-selection/asp-3.c)  
