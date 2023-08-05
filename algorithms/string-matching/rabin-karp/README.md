# Rabin-Karp algorithm for string matching (RBA)

## Problem

Given a pattern $P[0..m-1]$ and a text $T[0..n-1]$, find all occurrences of $P$ in $T$. It is assumed that $n \geq m$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts, i.e. the valid shifts for $P$ in $T$.

## Rabin-Karp algorithm

The Rabin-Karp algorithm avoids having to compare all the characters of $T[s..s + m -1]$ with $P$ at each shift $s$ by using a hash function. The idea is to precompute the hash value of $P$ and of $T_m$ (the m-character prefix of $T$), and then to recompute the hash value of $T[s..s + m -1]$ at each shift $s$ in constant time using a rolling hash function[^1]. Only if the hash values of $P$ and $T[s..s + m -1]$ match, do we then compare all the $m$ characters in $P$ with $T[s..s + m -1]$ in order to check if the shift is valid and the match wasn't just a spurious hit (a false positive) due to a hash collision (two different strings having the same hash value).

The precomputation of the hash values of $P$ and $T_m$ can be done in $O(m)$ time. The expected time complexity of the algorithm is then $O(n+m)$, since the hash values of the substrings of $T$ are recomputed in constant time. The worst-case time complexity occurs when all the substrings of $T$ have the same hash value as $P$, in which case the complexity of the algorithm degenerates to $O((n-m+1)m)$, the same as that of the naive algorithm. This happens when the text $T$ consists of $n$ copies of the pattern $P$ or when the hash function is a poor one and produces too many collisions.

Luckily, we can greatly reduce the probability of hash collisions by using a good hash function. For example, a merely additive rolling hash function is definitely not good enough. Such a hash function represents a string as the sum of the values of its characters and recomputes the hash value by adding the value of the new character in the window and subtracting the value of the character that just left the window. This is a poor hash function because it is easy to find two strings that have the same hash value. A much better hash function is the polynomial rolling hash function, which the Rabin-Karp algorithm uses. The hash value of a string $s$ is then computed as follows:

$$h(s) = \sum_{i=0}^{m-1} (s[i] \cdot d^i) \mod p$$

where $d = |\Sigma|$ is the size of the alphabet, $p$ is a large prime number (so that the hash values are uniformly distributed) and $s[i]$ is the value of the $i$-th character of the string $s$ in the alphabet $\Sigma$.

The precomputation of the hash values of $P$ and $T_m$ can be done in $O(m)$ time by applying Horner's rule to the above formula:

$$h(s) = (((s[0] \cdot d + s[1]) \cdot d + s[2]) \cdot d + \dots + s[m-1]) \mod p$$

After that, the hash value at each new shift $s+1$ can be recomputed in constant time as follows:

$$h(T[s+1..s + m]) = (d \cdot (h(T[s..s + m -1]) - T[s] \cdot q) + T[s + m]) \mod p$$

where $q$ is precomputed as $q = d^{m-1} \mod p$. The subtraction of $T[s] \cdot q$ is necessary to remove the contribution of the character that just left the window, and the addition of $T[s + m]$ to add the contribution of the new character in the window. Note that the hash values are computed modulo $p$ at each step to avoid overflow. The probability of a hash collision is thus reduced to $1/p$.

Implementation: [Rabin-Karp](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/rabin-karp/rba.c)

[^1]: A hash function h maps a string s to an integer h(s). A rolling hash function is one whose value can be recomputed in constant time as the string is shifted by one position.
