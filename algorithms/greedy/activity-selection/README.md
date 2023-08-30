# Activity-selection problem

## Problem

Given is a set of $n$ competing activities $A = \lbrace a_1, a_2, \dots, a_n \rbrace$, where each activity $a_i$ is defined by a pair of integers $(s_i, f_i)$, where $s_i$ is the start time and $f_i$ is the finish time. The problem is to select a maximum-size subset of mutually compatible activities. Two activities $a_i$ and $a_j$ are compatible if the intervals $[s_i, f_i)$ and $[s_j, f_j)$ don't overlap, i.e. $s_j \geq f_i$ or $s_i \geq f_j$. We assume that the activities are already sorted by monotonically increasing finish time $f_i$.

## DP solution (CLRS Ex 15.1-1)

The problem can be solved using dynamic programming, since it exhibits optimal substructure, and the subproblems overlap.

- The optimal subctructure resides in the fact that an optimal solution to the entire problem consists of optimal solutions to its subproblems. If we select a certain activity to be included in the solution, then the problem is split into two subproblems: the set of activities that finish before the selected activity, and the set of activities that start after the selected activity, and the optimal solution to the entire problem is then the union of the optimal solutions to the two subproblems, plus the selected activity.
- The overlap comes from the fact that we don't know a priori which activity to select in order to get the optimal solution, so we have to try all of them, and the subproblems are not independent, since they share the same overall set of activities. In other words, we have to solve the same subproblems multiple times.

If we consider the set of activities $A = \lbrace a_1, a_2, \dots, a_n \rbrace$, we can choose to include activity $a_k$ in the solution, or not. If we do, we split the problem into two subproblems: the set of activities that finish before $a_k$ starts, and the set of activities that start after $a_k$ finishes, and then continue to recursively solve the two subproblems.  
If we denote a subset of activities having start times after activity $a_i$ and finish times before activity $a_j$ by $A_{ij}$, and let $s[i,j]$ be the size of the optimal solution to the problem of selecting a maximum-size subset of mutually compatible activities from the set $A_{ij}$, then we have the following recurrence relation:

$$
s[i,j] =  
\begin{cases}
0 & \text{if } A_{ij} = \emptyset \\
\max \lbrace s[i,k] + s[k,j] + 1 : i < k < j \rbrace & \text{if } A_{ij} \neq \emptyset
\end{cases}
$$

where $k$ is the index of the activity that we choose to include in the solution. As noted above, we need to try all possible values of $k$ in order to find the optimal solution.

In order to simplify the implementation, we need to add two sentinel activities, one with start time $0$ and finish time $0$, and another one with start time $\infty$ and finish time $\infty$. We use the example from section 15.1 of the textbook to illustrate the implementation. It contains the following set of activities:

| Activity | Start time | Finish time |
| :--- | :--- | :--- |
| $a_1$ | 1 | 4 |
| $a_2$ | 3 | 5 |
| $a_3$ | 0 | 6 |
| $a_4$ | 5 | 7 |
| $a_5$ | 3 | 9 |
| $a_6$ | 5 | 9 |
| $a_7$ | 6 | 10 |
| $a_8$ | 7 | 11 |
| $a_9$ | 8 | 12 |
| $a_{10}$ | 2 | 14 |
| $a_{11}$ | 12 | 16 |

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
Implementation using bottom-up DP: [ASP - Bottom-up DP](https://github.com/pl3onasm/AADS/blob/main/algorithms/greedy/activity-selection/asp-1.c)

Note the similarity with the [bottom-up DP implementation for the matrix-chain multiplication problem](https://github.com/pl3onasm/AADS/blob/main/algorithms/dynamic-programming/matrix-chain-mult/mcm-3.c).  
The time complexity in both cases is the same: $O(n^3)$.
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  

## Greedy solution

We actually don't need to try all possible values of $k$ in order to find the optimal solution. Each time we select an activity, we can make the *greedy choice* of selecting the activity that finishes first, and then continue to solve the subproblem of selecting a maximum-size subset of mutually compatible activities from the set of activities that start after the selected activity finishes. We can prove that this greedy choice is optimal. It also leads to a linear-time solution, since we only need to scan each activity once, thanks to the fact that the activities are ordered by monotonically increasing finish time.

Impementing the greedy algorithm can be done using a top-down approach, as opposed to the bottom-up approach used for the DP solution. This is because any solution will include the first activity, since it finishes first, and then we can recursively solve the subproblem of selecting a maximum-size subset of mutually compatible activities from the set of activities that start after the first activity finishes.  

A top-down design is typical for greedy algorithms: we can just make the greedy choice, the one that seems optimal locally, and then recursively solve the remaining subproblem to get the global optimal solution. This is in contrast to the bottom-up approach used for DP, where we start by solving the smallest subproblems, so we can make an optimal choice based on the optimal solutions to the subproblems, and thus build our way up to an optimal solution for the entire problem.

Just as before, we assume that the activities are sorted by their finish time $f_i$. This time, we only need to add one sentinel activity for the recursive version (one with start time $0$ and finish time $0$ for the recursive version), and none for the iterative version. We use the same example as before to illustrate the implementation. The time complexity is $\Theta(n)$, which is a big improvement over the $O(n^3)$ time complexity of the DP solution.  

Recursive implementation using the greedy approach: [ASP - Recursive top-down greedy](https://github.com/pl3onasm/AADS/blob/main/algorithms/greedy/activity-selection/asp-2.c)  

Iterative implementation using the greedy approach: [ASP - Iterative top-down greedy](https://github.com/pl3onasm/AADS/blob/main/algorithms/greedy/activity-selection/asp-3.c)  
