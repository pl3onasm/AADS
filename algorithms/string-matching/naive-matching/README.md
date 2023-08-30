# Naive String Matching

## Problem

Given a pattern $P[0:m-1]$ and a text $T[0:n-1]$, find all occurrences of $P$ in $T$. It is assumed that $n \geq m$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts, i.e. the valid shifts for $P$ in $T$.

## Brute-force solution

The naive string-matching algorithm slides the pattern $P$ one position at a time along the text $T$, at each position comparing the characters in $P$ and $T$ until either $P$ mismatches $T$ or $P$ matches $T$ completely.
Since there are $n-m+1$ possible shifts for $P$ in $T$, and each shift requires $O(m)$ time to compare the characters, the algorithm runs in $O((n-m+1)m)$ time.

Implementation: [Naive String Matching](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/naive-matching/nsm.c)
