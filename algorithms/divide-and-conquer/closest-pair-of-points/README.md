# Closest Pair of Points

## Problem

Given a set of points in the plane, find the closest pair of points, where closeness is measured in terms of Euclidean distance.

## Linearithmic Solution

A divide and conquer approach to this problem is to first recursively divide the set of points into two equal-sized subsets $P_L$ and $P_R$ around the median x-coordinate. After hitting the base case of a subset with two or three points, we can then recursively find the closest pair of points in each subset. Let $\delta$ be the minimum of the two closest distances from the recursive calls: $\delta = \min(d_L, d_R)$. Now, in order to find the closest pair in the entire plane, we still need to consider those pairs where one point lies in $P_L$ and the other in $P_R$. Since the distance between any two points in $P_L$ or $P_R$ is at least $\delta$, we only need to consider the points within a strip of width $2\delta$ around the median x-coordinate to find a closer pair. After sorting these points by y-coordinate, we can scan through them by sliding a window of over the strip, checking the distance between each point and its seven neighbors. The closest pair in the entire plane can then be found by recursively taking the minimum of the closest pair in the left and right subsets and the closest pair in the strip.

If we only presort the points by x-coordinate to find the median x-coordinate, we would need to sort the points by y-coordinate in each recursive call to find the closest pair in the strip. This would take $O(n \log n)$ time in each recursive call, resulting in a recurrence relation of $T(n) = 2T(n/2) + O(n \log n)$ and a total running time of $O(n \log^2 n)$ by the master theorem. However, if we preprocess the points by sorting them by both x-coordinate and y-coordinate, we can find the closest pair in the strip in linear time. This results in a recurrence relation of $T(n) = 2T(n/2) + O(n)$ and a total running time of $O(n \log n)$ by the master theorem.

Implementation: [closest pair of points](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/closest-pair-of-points/closestpair.c)
