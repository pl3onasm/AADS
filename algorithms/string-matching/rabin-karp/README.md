# Rabin-Karp algorithm for string matching (RBA)

## Problem

Given a pattern $P$ and a text $T$, find all occurrences of $P$ in $T$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts (so-called *shifts*). The pattern $P$ and the text $T$ are both strings of length $m$ and $n$, respectively.

## Brute-force

The brute-force algorithm for string matching simply checks all possible shifts of $P$ in $T$ and compares all the characters in the substrings of $T$ with $P$ at each and every shift. The complexity of this algorithm is thus $O((n-m+1)m)$, where $n$ is the length of $T$ and $m$ is the length of $P$.

Implementation: [RBA - Brute-force](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/rabin-karp/rba-1.c)

## Rabin-Karp algorithm

The Rabin-Karp algorithm avoids having to compare the substrings of $T$ with $P$ at each shift by using a hash function. The idea is to precompute the hash value of $P$ and of $T[0..m-1]$ (the first substring of $T$ of length $m$). Given the fact that the algorithm uses a polynomial rolling hash function, we can then recompute the hash values of all the other substrings of $T$ in constant time as we slide the window over $T$. At each shift, we thus compare the hash values of the current substring of $T$ of length $m$ with the hash value of $P$, and only if these hash values match do we then compare all the characters in the substring with $P$ in $O(m)$ time, in order to check if the shift is valid and the match wasn't just a spurious hit (due to a hash collision).

The precomputation of the hash values of $P$ and $T[0..m-1]$ can be done in $O(m)$ time. The expected time complexity of the algorithm is then $O(n+m)$, since the hash values of the substrings of $T$ are recomputed in constant time. The worst-case time complexity occurs when all the substrings of $T$ have the same hash value as $P$, in which case the algorithm degenerates to the brute-force algorithm, i.e. $O((n-m+1)m)$. This happens when the text $T$ consists of $n$ copies of the pattern $P$ or when the hash function is poor and produces too many collisions. However, we can greatly reduce the probability of hash collisions by using a good hash function (e.g. a merely additive rolling hash function, i.e. a hash function that only adds the value of the new character and subtracts the value of the oldest character in the window, would be a poor one) and a large prime number as the modulus (so that the hash values are uniformly distributed).

Implementation: [RBA - Rabin-Karp](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/rabin-karp/rba-2.c)
