# Heapsort

Heapsort is an in-place sorting algorithm that sorts an array $A[0:n-1]$ in ascending order by using a binary heap. To achieve this, it first turns the input array $A$ into a binary max-heap structure, after which the root of the heap is the maximum element of $A$. It then places this maximum element in its correct position by swapping it with the last element of the heap, and decreasing the heap size by 1 to exclude it from the rest of the algorithm and consolidate its position in the sorted array.

After swapping the last element of the heap into the root, the heap property may now be violated and we need to restore it by calling *max-heapify* on the root element, so that the largest element is again at the root of the heap. This whole process of swapping the root with the last element and restoring the heap property is repeated until the heap size is 1, at which point the array is sorted.

Since the heapify procedure takes $O(\log n)$ time and is called $n-1$ times, the overall time complexity of this algorithm is $O(n \log n)$.

The algorithm is very similar to [selection sort](https://github.com/pl3onasm/Algorithms-and-data-structures/tree/main/algorithms/sorting/selection-sort), as selection sort could also be implemented by finding the maximum element at each iteration and then swapping it with the last element, thus shrinking the unsorted region by 1, and repeating this process until the unsorted region is empty. The difference is that heapsort uses a heap data structure rather than a linear-time search to find the maximum. This makes heapsort much faster than selection sort.

Implementation: [Heapsort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/heap-sort/heapsort.c)
