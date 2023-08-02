# Counting Sort

Counting sort is a linear-time sorting algorithm that sorts an array $A[0:n-1]$ in ascending order by exploiting the fact that the input consists of non-negative integers in a relatively small range $[0, k]$, where $k = O(n)$.

The algorithm starts by creating an auxiliary array $C[0:k]$ of size $k+1$, which serves as a histogram: $C[i]$ represents the number of elements in $A$ that are equal to $i$, i.e. the frequency of $i$ in $A$. This array $C$ is then modified so that each element $C[i]$ comes to represent the number of elements in $A$ that are less than or equal to $i$. This is done by iterating through $C$ and adding each count to the sum of the previous counts, i.e. $C[i] = C[i] + C[i-1]$. This information is then used to place each element from $A$ directly in its correct position in the auxiliary array $B[0:n-1]$, which is the sorted output array. For this, the algorithm iterates through $A$ in reverse order, placing each element $A[i]$ in the position $B[C[A[i]]-1]$, and then decrementing $C[A[i]]$ by 1. This ensures that elements with equal keys are placed in the same relative order in $B$ as they were in $A$ and that the algorithm is stable.

Since the algorithm independently iterates through $A$ and $C$, it runs in $O(n+k)$ time. If $k = O(n)$, then the algorithm runs in $O(n)$ time. However, if $k$ is much larger than $n$, then the algorithm runs in $O(k)$ time, which may be much slower than other sorting algorithms. Counting sort is often used as a subroutine in [radix sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/radix-sort), because it is very efficient when $k = O(n)$ and because it is stable, which is necessary for radix sort to work correctly.

Implementation: [Counting Sort](https://github.com/pl3onasm/CLRS/blob/main/algorithms/sorting/counting-sort/countingsort-1.c)

An adaptation can be made in cases where the array $A$ contains negative integers or has a minimum element that is much larger than 0. This is done by shifting the elements in the array to the right or left by a constant equal to the minimum element in the array. If the minimum element is negative, then the array is shifted to the right by the absolute value of the minimum element. If the minimum element is positive, then the array is shifted to the left by the minimum element. Either way, the minimum element is shifted to the first position in the output array $B$.  
The algorithm then works as before, except that the size of the auxiliary array $C$ is now $k+1$, where $k$ is the difference between the maximum and minimum elements in the array.

Implementation: [Adapted Counting Sort](https://github.com/pl3onasm/CLRS/blob/main/algorithms/sorting/counting-sort/countingsort-2.c)
