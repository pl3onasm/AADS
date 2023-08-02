# Bucket Sort

Bucket sort is a linear-time sorting algorithm that sorts elements by distributing them into buckets based on their values, and then sorting the elements in each bucket. The linear-time performance is achieved by the fact that bucket sort assumes that the input is drawn from a uniform distribution, such that the input can be distributed evenly over the buckets and the size of each bucket remains small, allowing for sorting to be performed in expected constant time.

It is particularly useful when the input is uniformly distributed over a range like $[0,1)$, because then the elements can be immediately inserted into their respective buckets, but it can be generalized to work on any range. Usually, buckets are implemented as linked lists, which are then individually sorted with an insertion sort[^1] adapted to work on linked lists. Once all the buckets are sorted, the elements are concatenated together to form the sorted array.

The implementation of bucket sort in this repository is based on the pseudocode in CLRS, and sorts an array of floating-point numbers in the range $[0,1)$.

Implementation: [Bucket Sort](https://github.com/pl3onasm/CLRS/blob/main/algorithms/sorting/bucket-sort/bucketsort.c)

[^1]: Insertion sort is used because it is efficient on small lists, and the lists are expected to be small since the input is assumed to be uniformly distributed.
