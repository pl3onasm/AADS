# ${\color{Cadetblue}\text{Maximum Subarray}}$

## ${\color{rosybrown}\text{Problem}}$

Given an array of numbers, find a contiguous subarray with the largest sum. For example, given the array $[-2, 1, -3, 4, -1, 2, 1, -5, 4]$, the contiguous subarray with the largest sum is $[4, -1, 2, 1]$, with sum 6. Note that there may be multiple subarrays with the same maximum sum; in this case, we only return one of them.

## ${\color{darkseagreen}\text{Quadratic Solution}}$

A naive solution is to try all possible subarrays and find the one with the largest sum. This can be done in $\Theta(n^2)$ time by using two nested loops and keeping track of the largest sum found so far.

Implementation: [Quadratic MSP](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/max-subarray/msp-1.c)

## ${\color{darkseagreen}\text{Linearithmic Solution}}$

Using a divide-and-conquer approach, the problem can be solved in $\Theta(n \log n)$ time. If we divide the array into two halves at each step, the maximum subarray can then be in the left half, in the right half, or it can span the two halves. The first two cases are two instances of the original problem, and can be solved recursively. The third case is part of the combination step, and can be solved in linear time by finding the maximum subarray that starts from the middle element and stretches to the left as far as possible, and the maximum subarray that starts from the middle element and stretches to the right as far as possible, and adding these two parts together. At the end, we compare the three cases, and return the maximum of the three.

Since the problem is divided into two subproblems of half the size, and the cost of combining the solutions is linear, the recurrence relation is $T(n) = 2T(n/2) + \Theta(n)$. Using the master theorem, case 2, yields the solution $\Theta(n \log n)$.

Implementation: [Linearithmic MSP](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/max-subarray/msp-2.c)

## ${\color{darkseagreen}\text{Linear Solution}}$

A linear solution can be obtained by making the following observation: if adding a number to the running sum of a subarray causes the sum to turn negative, then the subarray should be reset to an empty array and the running sum should be reset to zero. The maximum can still be updated in this case, if the running sum is greater than the current (negative) maximum, but the subarray itself should be reset because:

- if the next number is negative, the overall result will only decrease
- if the next number is positive, we should start a new subarray from that number, without the negative running sum so far, in order to get the larger sum

Therefore, the algorithm can be implemented in linear time by iterating over the array and keeping track of the maximum sum and the current sum. The maximum sum is updated if the current sum is greater than the maximum sum, and the current sum is reset to zero if it becomes negative.

Implementation: [Linear MSP](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/max-subarray/msp-3.c)
