# ${\color{Cadetblue}\text{Rabin-Karp algorithm}}$ ${\color{Cadetblue}\text{(RKA)}}$

## ${\color{Rosybrown}\text{Problem}}$

Given a pattern $P[0:m-1]$ and a text $T[0:n-1]$, find all occurrences of $P$ in $T$. It is assumed that $n \geq m$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts, i.e. the valid shifts for $P$ in $T$.

## ${\color{darkseagreen}\text{Rabin-Karp algorithm}}$

The Rabin-Karp algorithm avoids having to compare all the characters of $T[s:s + m -1]$ with $P$ at each shift $s$ by using a hash function. The idea is to precompute the hash value of $P$ and of $T_m$ (the m-character prefix of $T$), and then to recompute the hash value of $T[s:s + m -1]$ at each shift $s$ in constant time by using a $\color{orchid}{\text{rolling hash function}}$[^1]. Only if the hash values of $P$ and $T[s:s + m -1]$ match, do we then compare all the $m$ characters in $P$ with $T[s:s + m -1]$ in order to check if the shift is valid and the match wasn't just a $\color{orchid}{\text{spurious hit}}$ (a false positive) due to a hash collision (two different strings having the same hash value).

The precomputation of the hash values of $P$ and $T_m$ can be done in $\mathcal{O}(m)$ time. The expected time complexity of the algorithm is then $\mathcal{O}(n+m)$, since the hash values of the substrings of $T$ are recomputed in constant time as the text window of size $m$ slides to the right by one character at each shift $s$, and only a constant number of valid shifts are expected on average. The worst-case time complexity occurs when $T[s:s + m -1]$ has the same hash value as $P$ for all shifts $s$, in which case the complexity of the algorithm degenerates to $\mathcal{O}((n-m+1)m)$, the same as that of the naive algorithm. This happens when the text $T$ consists of $n$ copies of the pattern $P$ or when the hash function is a poor one and produces too many collisions.

Fortunately, we can greatly reduce the probability of hash collisions by using a good hash function. For example, a merely $\color{orchid}{\text{additive}}$ rolling hash function is definitely not good enough. Such a hash function represents a string as the sum of the values of its characters and recomputes the hash value by adding the value of the new character in the window and subtracting the value of the character that just left the window. This is a poor hash function because it is easy to find two strings that have the same hash value. A much better hash function is the $\color{orchid}{\text{polynomial}}$ rolling hash function, which the Rabin-Karp algorithm uses. The hash value of a string $S$ of length $m$ is then computed as follows:  

$$
\color{mediumpurple}\boxed{\color{rosybrown}\space h(S) = \sum_{i=0}^{m-1} (S[i] \cdot d^i) \text{ mod } p \space}\\
$$

where:

- $d = |\Sigma|$ is the size of the alphabet  
- $p$ is a large prime number (so that the hash values are uniformly distributed)  
- $S[i]$ is the value of the $i$-th character of the string $S$ taken from the alphabet $\Sigma$.  

The precomputation of the hash values of $P$ and $T_m$ can be done in $\mathcal{O}(m)$ time by applying $\color{orchid}{\text{Horner's rule}}$ to the above formula:  

$h(S) = (((S[0] \cdot d + S[1]) \cdot d + S[2]) \cdot d$  
$\qquad\qquad + \dots + S[m-1]) \text{ mod } p$  

After that, the hash value of the $m$-sized text window at each new shift $s+1$ can be recomputed in constant time as follows:

$h(T[s+1:s + m]) = $  
$\qquad\qquad (d \cdot (h(T[s:s + m -1])$  
$\qquad\qquad - T[s] \cdot k) + T[s + m]) \text{ mod } p$  

where $k$ is precomputed as  

$k = d^{m-1} \text{ mod } p$.  

The subtraction of $T[s] \cdot k$ is necessary to remove the contribution of the character that just left the window, and the addition of $T[s + m]$ adds the contribution of the new character that just entered the text window. The hash values of $P$ and $T[s:s + m -1]$ can then be compared in constant time to check if the shift is valid. Note that these hash values are computed modulo $p$ at each step to avoid overflow and to keep the values within a reasonable range. The probability of a hash collision is thus reduced to $1/p$.

Implementation: [Rabin-Karp](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/rabin-karp/rka.c)

[^1]: A hash function h : Σ* → ℤ, maps a string s to an integer h(s). A rolling hash function is one whose value can be recomputed in constant time as the text window of length m slides to the right by one character.
