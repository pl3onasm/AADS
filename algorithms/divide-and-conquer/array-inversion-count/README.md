# Inversion count

## Problem

Given an array of integers, find the number of inversions, i.e. the number of pairs of indices $i < j$ such that $A[i] > A[j]$. In other words, find the number of pairs of elements that are out of order. For example, given the array $[2,3,8,6,1]$, the number of inversions is 5, since the pairs $(2,1)$, $(3,1)$, $(8,6)$, $(8,1)$, and $(6,1)$ are all inversions.

## Solution

A solution in linearithmic time is to use a modified merge sort. This draws on the observation that the merge step is in fact already checking for inversions when merging both subarrays into a single sorted array. This is because merging is done with the left and right subarrays sorted on their own, comparing the next element in each subarray to determine which element to add next to the combined sorted array. If the next element in the left subarray is less than the next element in the right subarray, it is less than all the remaining elements in the right subarray (because it is sorted in increasing order) and so there is no inversion in this case. If, however, the next element in the right subarray is less than the next element in the left subarray, it is less than all the remaining elements in the left subarray (because it is sorted in increasing order), meaning that the element in the right subarray forms an inversion with all those elements and the number of inversions is incremented by the number of elements remaining in the left subarray.[^1]

Since merge sort recursively splits the array into smaller subarrays in order to merge the sorted versions of the subarrays once the recursion bottoms out, we need to keep track of the number of inversions in each (intermediate) subarray and their recombination. This can be done by adding a counter to all the involved functions. The total number of inversions is then the sum of the number of inversions in each subarray and the number of inversions found in the recombination of these subarrays.

The time complexity of this algorithm is $O(n \log n)$, since the merge step takes $O(n)$ time and the recursion bottoms out after $O(\log n)$ levels.

Implementation: [Inversion count](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/inversion-count/inversioncount.c)

[^1]: Note that this is true if we understand an inversion as a violation of an increasing order in this case. If, instead, we were to work with a decreasing order, inversions would only occur if the next element in the right subarray is greater than the next element in the left subarray, in which case the number of inversions would also be incremented by the number of elements remaining in the left subarray.
