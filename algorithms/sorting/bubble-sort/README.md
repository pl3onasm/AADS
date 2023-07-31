# Bubble sort

Bubble sort is a simple algorithm that sorts an array $A[0:n]$ by determining for each index $i$ the minimum element in the subarray $A[i:n]$ and swapping it with $A[i]$. Determining this minimum element for each $i$ requires a linear search through the subarray from $n-1$ down to $i$, swapping adjacent elements as necessary. This is also where the name comes from: as we keep swapping from $n-1$ down to each $i$, smaller elements "bubble" to the front of the array, similar to bubbles rising to the surface of a liquid. It is also sometimes referred to as *sinking sort*, due to larger elements sinking to the bottom of the array.

Bubble sort is an in-place sort, meaning that it does not require any additional memory. However, it is not a stable sort, since it does not preserve the relative order of equal elements in the input. It has a worst-case running time of $O(n^2)$, due to the linear search for the minimum element at each index, even if the array is already sorted. A small optimization can be made by keeping track of whether any swaps were made during a pass through the array. If no swaps were made, then the array is already sorted and the algorithm can terminate early. This reduces the best-case running time to $O(n)$.

Implementation: [Bubble sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/bubble-sort)
