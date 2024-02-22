# ${\color{Cadetblue}\text{Closest Pair of Points}}$

## ${\color{rosybrown}\text{Problem}}$

Given a set of points in the plane, find the closest pair of points, where closeness is measured in terms of Euclidean distance.

## ${\color{darkseagreen}\text{Linearithmic Solution}}$

A divide and conquer approach to this problem is to first sort all points by x-coordinate and then recursively divide the set of points into two equal-sized subsets $P_L$ and $P_R$ around the median x-coordinate. After hitting the base case of a subset with two or three points, where we can simply compute the distance between all pairs of points and return the closest pair, we can then recursively find the closest pair of points in each subset. Let $\delta$ be the minimum of the two closest distances from the recursive calls: $\delta = \min(d_L, d_R)$. Now, in order to find the closest pair in the entire plane, we still need to consider those pairs where one point lies in $P_L$ and the other in $P_R$. Since the distance between any two points in $P_L$ or $P_R$ is at least $\delta$, we only need to consider the points within a strip of width $2\delta$ around the median x-coordinate to find a closer pair. After having sorted these points by y-coordinate, we can scan through them by sliding a window of $2\delta \times \delta$ over the strip, checking the distance between each point and its next seven neighbors.[^1] The closest pair in the entire plane can then be found by recursively taking the minimum of the closest pair in the left and right subsets and the closest pair in the strip.

If we only presort the points by x-coordinate so as to be able to repeatedly find the median x-coordinate so we can divide the points into subsets, we would need to sort the points by y-coordinate in each recursive call to find the closest pair in the strip. This would take $\mathcal{O}(n \log n)$ time in each recursive call, resulting in a recurrence relation of $T(n) = 2T(n/2) + \mathcal{O}(n \log n)$ which yields a total running time of $\mathcal{O}(n \log^2 n)$ by case 2 of the master theorem.

However, if we preprocess the points by sorting them once by x-coordinate and once by y-coordinate, we can find the closest pair in the strip in linear time. This results in a recurrence relation:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + \mathcal{O}(n) \space}
$$

By case 2 of the master theorem, we then get a total running time of $\mathcal{O}(n \log n)$. Since preprocessing the points also takes $\mathcal{O}(n \log n)$ time, the total running time is then $\mathcal{O}(n \log n)$.

Implementation: [closest pair of points](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/closest-pair-of-points/closestpair.c)

[^1]: The window size of 2δxδ is chosen because we already found a minimum distance of δ from the recursive calls on P<sub>L</sub> and P<sub>R</sub>, and we are now checking the strip to look for points that are closer than δ. In such a window, there can be at most eight points, given that the points in both window halves (of size δxδ), which entirely lie in P<sub>L</sub> or P<sub>R</sub>, are spaced at least δ apart and that points on the dividing line between P<sub>L</sub> and P<sub>R</sub> can be in both window halves. Hence, we only need to check the distance between each point and its next seven neighbors, as the points in the strip are already sorted by y-coordinate.
