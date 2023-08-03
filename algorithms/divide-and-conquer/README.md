# Divide and Conquer

When a problem is too difficult to solve directly, it is often possible to attack the problem by dividing it into subproblems that are themselves smaller instances of the same problem and then solving these subproblems recursively. Such an approach is known as *divide-and-conquer*, and it is typically described by a recurrence relation, which expresses the solution to a problem in terms of solutions to smaller instances of the same problem.

A divide-and-conquer algorithm consists of three steps at each level of the recursion:

1. **Divide** the problem into a number of subproblems that are smaller instances of the same problem.
2. **Conquer** the subproblems by solving them recursively.
3. **Combine** the solutions to the subproblems into a solution for the original problem.

After sufficiently many levels of recursion, the recursion bottoms out and the subproblems become so small that they can be solved directly. As recursion unwinds, the solutions to the subproblems are then combined to give a solution to the original problem.

Note that if the subproblems are not independent (i.e. solutions to subproblems depend on solutions to other subproblems), the divide-and-conquer approach is not suitable, since the same subproblem would be solved multiple times. In such cases, a [dynamic programming](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming) approach is more appropriate.

## Problems

| **CLRS³** | **CLRS⁴** | **Link** |
|:---:|:---:|:---|
| Chap 4.1 | n/a | [Maximum Subarray](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/divide-and-conquer/max-subarray)
| Chap 4.2 | Chap 4.1-2 | [Strassen's Matrix Multiplication](https://github.com/pl3onasm/AADS/tree/main/algorithms/divide-and-conquer/strassen)
| Ex 2.3-5 | Ex 2.3-6 | [Binary Search](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/divide-and-conquer/binsearch)
| Prob 2-4 | Prob 2-4 | [Inversion Count](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/divide-and-conquer/inversion-count)
| Chap 9.2-3 | Chap 9.2-3 | [Quickselect](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/divide-and-conquer/quickselect)
| Chap 33.4 | n/a | [Closest Pair of Points](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/divide-and-conquer/closest-pair-of-points)
| Chap 2.3 | Chap 2.3 | [Merge Sort](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/sorting/merge-sort)
| Chap 7.1-3 | Chap 7.1-3 | [Quicksort](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/sorting/quick-sort)
