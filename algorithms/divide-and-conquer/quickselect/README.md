# ${\color{Cadetblue}\text{Quickse} \text{lect}}$

## ${\color{rosybrown}\text{Problem}}$

Given an array of numbers, find the $k$-th order statistic, i.e. the $k$-th smallest element. In other words, find the element that would be at index $k - 1$ if the array were sorted. For example, given the array $[5, 1, 4, 2, 3]$ and $k = 3$, the third smallest element is $3$ at index $2$ if the array were sorted in increasing order.

## ${\color{darkseagreen}\text{Solution in expected}}$ ${\color{darkseagreen}\text{linear time}}$

The name quickselect is derived from the fact that it uses the same partitioning procedure as the [quicksort algorithm](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/quick-sort) in order to select the $k$-th smallest element. The key idea is that it is not necessary to sort the entire input array, but only to recursively partition the array around a randomly picked pivot until the pivot is the $k$-th smallest element.

At each recursive step, the array is partitioned around a pivot element such that in the resulting partition all elements to the left of the pivot are smaller and all elements to the right are larger (or equal) than the pivot. As we are looking for the $k$-th smallest element, the number $i$ of elements less than or equal to the pivot is compared to the order statistic $k$ at each step. If they are equal, the pivot is the $k$-th smallest element and the algorithm terminates. If $k$ is smaller, the algorithm is recursively called on the left subarray. If $k$ is larger, the order statistic is updated to be $k - i$, since $i$ elements were found to be smaller than the pivot and can thus be discarded from the search for the $k$-th smallest element, followed by a recursive call on the right subarray.

The algorithm is expected to run in linear time, since the partitioning is expected not to be skewed, meaning that on average we get a balanced partitioning around the pivot at each recursive step. The worst-case running time is quadratic, however, as it is possible that the pivot happens to be the smallest or largest element at each recursive step, resulting in a subarray of size $n - 1$ each time and an overall running time of $T(n) = T(n - 1) + \Theta(n) = \Theta(n^2)$.

Implementation: [Quickselect 1](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/quickselect/qselect-1.c)

## ${\color{darkseagreen}\text{Solution in linear time}}$

The worst-case running time of the algorithm can be improved from quadratic to linear by using a different partitioning scheme. Instead of picking a random pivot, the idea here is to pick a very specific pivot that ensures that the size of the subarray on which the algorithm is recursively called is at most $7n/10$ long at each step, ensuring that the partitioning is balanced enough to avoid the quadratic worst-case running time. This particular pivot is chosen as the median of the medians of groups of $5$ elements, which is guaranteed to be close to the median of the array and thus ensures a balanced partitioning. The ingenuity of the algorithm lies in the way it finds this pivot, which is done in linear time, by first calling itself to find the median of the medians of groups of $5$ elements, and then using this median as the pivot to partition the array around it in search of the $k$-th order statistic.

Thus, the algorithm starts by recursively finding the median of the medians of 5-element groups. In order to do this, the array is first trimmed so that the size of the remaining array becomes a multiple of $5$. This trimming is done by extracting at most $4$ minima and putting them at the leftmost positions of the array. The remaining elements are then partitioned into groups of 5 elements, which are sorted to find the median of each group, after which the algorithm is recursively called to find the median of the medians. This median is then used as the pivot to partition the array around it, after which the algorithm is recursively called on the left or right subarray depending on the order statistic $k$. The rest of the algorithm is the same as before.

Using this particular partitioning scheme ensures that the size of the subarray on which the algorithm is recursively called is at most $7n/10$ long, with $n$ being the size of the current array at each step. This is because the number $g$ of groups of 5 elements is such that $g \leq n/5$ (since the array is trimmed to be a multiple of $5$), and because the pivot is the median of the medians of these $g$ groups of 5 elements, so that $3g/2$ elements are sure to be smaller than the pivot, and another $3g/2$ to be larger, which means that the size of the subarray (left or right) on which the algorithm is recursively called is at most $5g - 3g/2$ $= 7g/2 \leq 7n/10$.

Taking this into account, we get the following recurrence relation for the worst-case running time:

$${\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n/5) + T(7n/10) + \Theta(n) \space}}$$

The first term is the time needed to find the median of the medians, the second term is the time spent on recursive calls on subarrays of size at most $7n/10$, and the last term represents the time spent outside of recursive calls, i.e. on partitioning the array around the pivot, extracting at most $4$ minima to make the size of the array a multiple of $5$, and the sorting of $g$ subarrays of size 5 (all three of these operations are linear in the size of the array).  
In order to solve this recurrence relation, we need to show that $T(n) = \mathcal{O}(n)$. This can be done using the substitution method. The base case is $T(1) = \Theta(1)$, and we assume that $T(n/5) \leq cn/5$ and $T(7n/10) \leq c7n/10$ for some constant $c$. Using this induction hypothesis, we need to show that $T(n) \leq cn$ for large enough $c$. We have:

$$
\begin{align*}
T(n) &= T(n/5) + T(7n/10) + \Theta(n) \\
&\leq c(n/5) + c(7n/10) + \Theta(n) \\
&= cn/5 + 7cn/10 + \Theta(n) \\
&= 9cn/10 + dn \\
&\leq cn
\end{align*}
$$

where the last inequality holds for $c \geq 10d$. This shows that $T(n) = \mathcal{O}(n)$, and thus the worst-case running time of the algorithm is indeed linear.

Implementation: [Quickselect 2](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/quickselect/qselect-2.c)
