# ${\color{Cadetblue}\text{Inversion count}}$

## ${\color{rosybrown}\text{Problem}}$

Given an array of integers, find the number of ${\color{peru}\text{inversions}}$, i.e. the number of pairs of indices $i < j$ such that $A[i] > A[j]$. In other words, find the number of pairs of elements that are ${\color{peru}\text{out of order}}$. For example, given the array $[2,3,8,6,1]$, the number of inversions is 5, since the pairs $(2,1)$, $(3,1)$, $(8,6)$, $(8,1)$, and $(6,1)$ are all inversions.

## ${\color{darkseagreen}\text{Solution}}$

A solution in linearithmic time that makes use of the divide-and-conquer paradigm can be realized by a modified [merge sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/merge-sort). It draws on the observation that the merge step in this sorting algorithm is in fact already checking for inversions when merging two subarrays into a single sorted array. After all, merging is done with the left and right subarrays already sorted on their own, comparing at each step the current element in the left subarray to the one in the right subarray to determine which element to add next to the combined sorted array. 

Now, if the element in the left subarray turns out to be less than the element in the right subarray, it is in fact less than all the remaining elements in the right subarray (because this subarray is already sorted in increasing order) and so the element from the left subarray is added to the combined sorted array without counting any inversions. If, however, the next element in the right subarray is less than the current element in the left subarray, it is less than all the remaining elements in the left subarray (because this subarray is also already sorted in increasing order), meaning that the element in the right subarray forms an inversion with all those elements. In this case, the number of inversions is therefore incremented by the number of remaining elements in the left subarray.[^1]

Since merge sort recursively ${\color{peru}\text{divides}}$ the array into smaller subarrays in order to merge the conquered, sorted subarrays back together in the end, we are able to count the number of inversions in the array by counting the number of inversions in each (${\color{peru}\text{conquered}}$) subarray and in the ${\color{peru}\text{recombination}}$ of these subarrays. This can be done by adding a counter to the merge step that increments by the number of remaining elements in the left subarray whenever an inversion is found. The total number of inversions in the input array is then the sum of the number of inversions in each subarray and the number of inversions in the recombination of these subarrays.  

The time complexity of this algorithm is $\mathcal{O}(n \log n)$, since the merge step takes $\mathcal{O}(n)$ time and the recursion bottoms out after $\mathcal{O}(\log n)$ levels of recursion.

Implementation: [Inversion count](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/inversion-count/inversioncount.c)

[^1]: Note that this is true in the case where we understand an inversion as a violation of an increasing order. If, instead, we were to work with a decreasing order, inversions would only occur if the next element in the right subarray is greater than the next element in the left subarray: this would then be the case where the element in the right subarray is greater than all the remaining elements in the left subarray and where the number of inversions would be incremented by the number of remaining elements in the left subarray.
