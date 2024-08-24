$\huge{\color{Cadetblue}\text{Maximum Subarray}}$

<br/>

$\Large{\color{rosybrown}\text{Problem}}$

Given an array of numbers, find a contiguous subarray with the largest sum. For example, given the array $[-2, 1, -3, 4, -1, 2, 1, -5, 4]$, the contiguous subarray with the largest sum is $[4, -1, 2, 1]$, with sum 6. Note that there may be multiple subarrays with the same maximum sum; in this case, we only return one of them.

Note that when all the numbers are negative, the maximum subarray will consist of a single element: the maximum number in the input array. Also, if the array contains only positive numbers, the maximum subarray will always be the entire input array.

<br/>

$\Large{\color{darkseagreen}\text{Quadratic Solution}}$

A naive solution is to try all possible subarrays and find the one with the largest sum. This can be done in $\Theta(n^2)$ time by using two nested loops and keeping track of the largest sum found so far.

Implementation: [Quadratic MSP](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/max-subarray/msp-1.c)

<br/>

$\Large{\color{darkseagreen}\text{Linearithmic Solution}}$

Using a divide-and-conquer approach, the problem can be solved in $\Theta(n \log n)$ time. If we ${\color{orchid}\text{divide}}$ the array into two halves at each recursive step, the maximum subarray can then be in the left half, in the right half, or it can span these two halves. The first two cases are two instances of the original problem, and can be solved (${\color{orchid}\text{conquered}}$) recursively. The third case is part of the ${\color{orchid}\text{combination}}$ step, and can be solved in linear time by finding the maximum subarray that starts from the middle element and stretches to the left as far as possible, and the maximum subarray that starts from the middle element and stretches to the right as far as possible, and then adding these two parts together. Finally, after solving the three cases, we finish by comparing the three solutions and return the maximum of the three.

Since the problem is divided into two subproblems of half the size at each step, and the cost of combining the solutions is linear, the recurrence relation is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + \Theta(n) \space}
$$

Using the master theorem, case 2, yields the solution $\Theta(n \log n)$.

Implementation: [Linearithmic MSP](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/max-subarray/msp-2.c)

<br/>

$\Large{\color{darkseagreen}\text{Linear Solution}}$

The algorithm can be implemented in linear time by iterating over the array and keeping track of a running sum and the maximum sum found so far along with the corresponding subarray bounds for these two sums (by storing their start and end indices). At the start of the algorithm, the running sum is initialized to zero, and the maximum sum is initialized to the smallest possible value.  

At each iteration, the running sum is updated by adding the current element to it only if the resulting sum is non-negative. If adding the current element would turn the running sum negative, then the running sum is reset to zero, and the lower bound of the running subarray is reset to the next element to start a new running subarray in order to get a fresh start at finding the maximum sum.

As the algorithm iterates over the array, the running sum is continuously compared to the maximum sum found so far and if at any point the running sum is found to be greater, the maximum sum is updated to the running sum, and the maximum subarray bounds are updated to the running subarray bounds.

The algorithm is implemented using a single loop that iterates over the array once, and the time complexity is therefore in $\Theta(n)$.

Implementation: [Linear MSP](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/max-subarray/msp-3.c)
