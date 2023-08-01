# Selection Sort

Selection sort is an in-place comparison sort which sorts an array $A[0:n-1]$ in ascending order by partitioning it into two subarrays: $A[0:i]$ and $A[i+1:n-1]$, where $i$ is the current index. The left subarray is sorted and the right subarray is unsorted. At each iteration, the minimum element in the right subarray is found by a simple linear search through the unsorted subarray and then swapped with $A[i+1]$, thus extending the sorted subarray by one element. This is repeated until the entire array is sorted.

The name comes from the fact that we are selecting the minimum element at each iteration and appending it to the sorted subarray. The running time is $O(n^2)$, even if the array is already sorted, since we still need to iterate through the entire array to determine the minimum element at each iteration. Unlike [bubble sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/bubble-sort), selection sort is not a stable sort, since it swaps elements that are not adjacent, thus potentially changing the relative order of elements with the same value.

Implementation: [Selection sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/selection-sort/selectionsort.c)
