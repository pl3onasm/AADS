# Quickselect

## Problem

Given an array of numbers, find the $k$-th order statistic, i.e. the $k$-th smallest element. In other words, find the element that would be at index $k$ if the array were sorted. For example, given the array $[3, 1, 4, 2, 5]$ and $k = 3$, the third smallest element is 3.

## Expected linear time

The algorithm's name is derived from the fact that it uses the same partitioning scheme as the quicksort algorithm to select the $k$-th smallest element. The key idea is to recursively partition the input array around a randomly picked pivot until the pivot is the $k$-th smallest element.

At each recursive step, the array is partitioned around a pivot element such that all elements to the left of the pivot are smaller and all elements to the right are larger. As we are looking for the $k$-th smallest element, the number $i$ of elements less than or equal to the pivot is compared to $k$. If they are equal, the pivot is the $k$-th smallest element and the algorithm terminates. If $k$ is smaller, the algorithm is recursively called on the right subarray. If $k$ is larger, this index is updated to be relative to the left subarray, since $i$ elements are smaller and thus removed from the left subarray on which the algorithm is recursively called.

The algorithm is expected to run in linear time, since the partitioning scheme is expected not to be skewed, so that on average we get a balanced partitioning at each recursive step. The worst-case running time is quadratic, however, as it is possible that at each recursive step only the pivot is removed from the array, resulting in a new subarray of size $n - 1$ and an overall running time of $T(n) = T(n - 1) + \Theta(n) = \Theta(n^2)$.

Implementation: [Quickselect 1](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/quickselect/qselect-1.c)

## Worst-case linear time

The worst-case running time of the algorithm can be improved to linear time by using a different partitioning scheme. Instead of picking a random pivot, we can use the median of the medians of $g$ subarrays of size 5 after removing enough elements from the array so that the size of the array is a multiple of 5 (these elements need all be minima since we are looking for the $k$-th smallest element). This ensures that all partitions are balanced enough to avoid the quadratic worst-case running time. The pivot can then be found recursively, after which the array is partitioned around it. The rest of the algorithm is the same as before.

Using this particular partitioning scheme ensures that the size of the subarray on which the algorithm is recursively called is at most $7n/10$ long. This is because $n \leq 5g$ and the pivot is the median of the medians of groups of 5 elements, so that $3g/2$ elements are sure to be smaller than the pivot, and another $3g/2$ to be larger, which means that the size of the subarray (left or right) on which the algorithm is recursively called is at most $5g - 3g/2 = 7g/2 \leq 7n/10$.

Taking this into account, we get the following recurrence relation for the worst-case running time:
$$T(n) = T(n/5) + T(7n/10) + \Theta(n)$$ The first term is the time needed to find the median of the medians, the second term is the time spent on recursive calls on subarrays of size at most $7n/10$, and the last term represents the time spent outside of recursive calls, i.e. on partitioning the array around the pivot, extracting at most 4 minima to make the size of the array a multiple of 5, and the sorting of the subarrays of size 5.  
In order to solve this recurrence relation, we need to show that $T(n) = O(n)$. This can be done using the substitution method. The base case is $T(1) = \Theta(1)$, and we assume that $T(n/5) \leq cn/5$ and $T(7n/10) \leq c7n/10$ for some constant $c$. We need to show that $T(n) \leq cn$ for large enough $c$.
We have:

$$
\begin{align*}
T(n) &= T(n/5) + T(7n/10) + \Theta(n) \\
&\leq c(n/5) + c(7n/10) + \Theta(n) \\
&= cn/5 + 7cn/10 + \Theta(n) \\
&= 9cn/10 + dn \\
&\leq cn
\end{align*}
$$

where the last inequality holds for $c \geq 10d$. This shows that $T(n) = O(n)$, and thus the worst-case running time of the algorithm is linear.

Implementation: [Quickselect 2](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/quickselect/qselect-2.c)
