# ${\color{Cadetblue}\text{Closest Pair of Points}}$

## ${\color{rosybrown}\text{Problem}}$

Given a set of points in the plane $P$, find the closest pair of points, where closeness is measured in terms of Euclidean distance.

## ${\color{darkseagreen}\text{Quadratic Solution}}$

A brute-force approach to solving this problem would be to compute the distance between all pairs of points in $P$ and return the closest pair. This would take $\mathcal{O}(n^2)$ time.

Implementation: [closest pair of points (brute-force)](cpp-1.c)

## ${\color{darkseagreen}\text{Linearithmic Solution}}$

A divide and conquer strategy starts by sorting all the points in the plane $P$ by their x-coordinate. Next, we recursively keep dividing $P$ at each step into two equal-sized subplanes $P_L$ and $P_R$ around the median x-coordinate in $P$ at that step. Once we reach the base case of a subplane having only two or three points, we simply compute the distance between all these points and return the closest pair in constant time using a brute-force approach. As recursive calls then start to return, we obtain the closest pairs in the right and left subplanes, with minimum distances $d_L$ and $d_R$, respectively.  

Now, let $\delta$ be the minimum of the two closest distances from the recursive calls at each level of the recursion tree: $\color{peru}\delta = \min(d_L, d_R)$. In order to find the closest pair in the entire plane $P_L \cup P_R$, we then still need to consider those pairs of points where one point lies in $P_L$ and the other in $P_R$. Since the distance between any two points that lie both in $P_L$ or both in $P_R$ is at least $\delta$, the only points that really still need to be considered to find a pair of points closer than $\delta$ are those within a strip of width $2\delta$ around the median x-coordinate, through which the dividing line between $P_L$ and $P_R$ passes. To find the closest pair in this strip, we first sort the points in the strip by their y-coordinate, and then scan through them by sliding ${\color{peru}\text{a window of size }}$ $\color{peru} 2 \delta \times \delta$ over the strip, each time checking the distance between the current point and its neighboring points within the window [^1] whilst keeping track of the closest pair and the minimum distance found so far as we go. The closest pair in the plane $P_L \cup P_R$ is then the pair of points that has the minimum distance between the closest pair in the left and right subplanes and the closest pair in the strip. As recursion unwinds, we thus eventually find the closest pair of points in the entire plane $P$.

If we choose to only presort the points by their x-coordinate, in order to be able to repeatedly find the median x-coordinate as we divide the points into subsets, this means that we would need to sort the points in the strip by their y-coordinate at each level of the recursion in order to find the closest pair in the strip. This would take $\mathcal{O}(n \log n)$ time at each recursive step, resulting in a recurrence relation of the form $T(n) = 2T(n/2) + \mathcal{O}(n \log n)$, which yields a total running time of $\mathcal{O}(n \log^2 n)$ by case 2 of the master theorem.

However, if we preprocess the points by sorting them once by x-coordinate and once by y-coordinate, we can find the closest pair in the strip in linear time. This results in a recurrence relation of the form:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + \mathcal{O}(n) \space}
$$

By case 2 of the master theorem, we then get a much better total running time of $\mathcal{O}(n \log n)$. Since preprocessing the points also takes $\mathcal{O}(n \log n)$ time, the running time of the entire algorithm is then $\mathcal{O}(n \log n)$.

Implementation: [closest pair of points (divide and conquer)](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/closest-pair-of-points/closestpair.c)

#### ${\color{darkseagreen}\text{Video}}$

[![Problem](https://img.youtube.com/vi/6u_hWxbOc7E/0.jpg)](https://www.youtube.com/watch?v=6u_hWxbOc7E)

[^1]: A window of size 2δxδ is chosen because we already obtained a minimum distance δ from the recursive calls on P<sub>L</sub> and P<sub>R</sub>, and we are now checking the strip to look for points that are closer than δ. In such a window, there can be at most eight points, given that the points in both window halves (of size δxδ), which entirely lie in P<sub>L</sub> or P<sub>R</sub>, are spaced at least δ apart and that points on the dividing line between P<sub>L</sub> and P<sub>R</sub> can be in both window halves. In practice, this means that we can just scan through the points in the strip and check the distance between the current point and its next seven neighbors at each step. This is possible because the points in the strip are already sorted by their y-coordinate.  
