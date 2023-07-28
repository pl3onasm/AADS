# Closest Pair of Points

## Problem

Given a set of points in the plane, find the closest pair of points, where closeness is measured in terms of Euclidean distance. 

## Divide and Conquer Approach

A divide and conquer approach to this problem is to divide the set of points into two subsets of equal size, and then recursively compute the closest pair in each subset. Let the distance between the closest pair of points in the left subset be d<sub>L</sub>, and the distance between the closest pair of points in the right subset be d<sub>R</sub>. Let d be the minimum of d<sub>L</sub> and d<sub>R</sub>. Then the closest pair of points is either a pair of points in the left or right subset whose distance is d, or it is a pair of points with one point in the left subset and one point in the right subset.

