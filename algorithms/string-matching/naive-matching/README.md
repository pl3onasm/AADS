# Naive String Matching

## Problem

Given a pattern $P$ and a text $T$, find all occurrences of $P$ in $T$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts (so-called *shifts*). The pattern $P$ and the text $T$ are both strings of length $m$ and $n$, respectively.

## Brute-force solution

The brute-force algorithm for string matching simply checks all possible shifts of $P$ in $T$ and compares all the characters in the substrings of $T$ with $P$ at each and every shift. The complexity of this algorithm is thus $O((n-m+1)m)$, where $n$ is the length of $T$ and $m$ is the length of $P$.

Implementation: [Naive String Matching](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/naive-matching/nsm.c)
