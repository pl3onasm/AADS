# Maximum Subarray Problem (MSP)

## Problem

Given an array of numbers, find the contiguous subarray with the largest sum.

## Quadratic Solution

A naive solution is to try all possible subarrays and find the one with the largest sum. This can be done in $\Theta(n^2)$ time by using two nested loops and keeping track of the largest sum found so far.

Implementation: [Quadratic MSP](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/max-subarray/msp-1.c)

## Linearithmic Solution

Using a divide-and-conquer approach, the problem can be solved in $\Theta(n \log n)$ time. The idea is to divide the array into two halves. The maximum subarray can then be in the left half, in the right half, or it can span the two halves. The first two cases are two instances of the original problem, and can be solved recursively. The third case is part of the combination step, and can be solved in linear time by finding the maximum subarray that starts from the middle element and goes to the left, and the maximum subarray that starts from the middle element and goes to the right, and concatenating them. Comparing the three cases, the maximum sum is returned.

Since the problem is divided into two subproblems of half the size, and the cost of combining the solutions is linear, the recurrence relation is $T(n) = 2T(n/2) + \Theta(n)$. Using the master theorem, case 2, yields the solution $\Theta(n \log n)$.

Implementation: [Linearithmic MSP](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/max-subarray/msp-2.c)

## Linear Solution

A linear solution can be obtained by using the following observation: if adding a number to the running sum of a subarray turns the sum negative, then the subarray should be reset to an empty array and the running sum should be reset to zero. The maximum can still be updated in this case, if the running sum is greater than the current (negative) maximum, but the subarray itself should be reset because:

- if the next number is negative, the overall result will only decrease
- if the next number is positive, we should start a new subarray from that number, without the negative numbers before it to get the larger sum.

Therefore, the algorithm can be implemented in linear time by iterating over the array and keeping track of the maximum sum and the current sum. The maximum sum is updated if the current sum is greater than the maximum sum, and the current sum is reset to zero if it becomes negative.

Implementation: [Linear MSP](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/max-subarray/msp-3.c)
