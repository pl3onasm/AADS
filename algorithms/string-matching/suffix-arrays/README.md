# Suffix arrays

If besides finding occurrences of a pattern in a text, we are also interested in finding things like the longest repeated substring, the number of (non-duplicate) substrings, the longest common substring between two texts, or the longest palindromic substring in a text, we can use a data structure called a suffix array, which is the *lexicographically sorted* array $SA[0:n-1]$ of all $n$ suffixes of a given string $T[0:n-1]$. In this suffix array, we do not store the suffixes themselves, but we store the starting index of each suffix in the original string. For example, the suffix array for the string $T=$`banana` is the second column in the table below.  
Along with the suffix array, we also want to compute the array $LCP[0:n-1]$ which yields the longest common prefix between adjacent suffixes in the suffix array, i.e. the length of the longest common prefix between $T[SA[i-1]:n-1]$ and $T[SA[i]:n-1]$ for $i=1,2,\ldots,n-1$. For example, the longest common prefix between the suffixes `ana` and `anana` is `ana`, so $LCP[2]=3$, and the longest common prefix between the suffixes `banana` and `na` is the empty string, so $LCP[4]=0$. The third column in the table below is the LCP array for the string `banana`.

<div align="center">

i      | SA[i] | LCP[i] | Suffix |
-------|:-----:|:------:|:-------|
0      | 5     | 0      | a      |
1      | 3     | 1      | ana    |
2      | 1     | 3      | anana  |
3      | 0     | 0      | banana |
4      | 4     | 0      | na     |
5      | 2     | 2      | nana   |

</div>

After constructing the suffix array, we can find all occurrences of a pattern $P[0:m-1]$ in a text $T[0:n-1]$ in $O(m\log n)$ time. We can do this by performing a binary search on the suffix array. For example, if we want to find all occurrences of the pattern `ana` in the text `banana`, we can perform a binary search on the suffix array to find an occurrence of `ana`. After that, we can find the range of suffixes that start with `ana` by trying to expand the range to the left and to the right in the suffix array until we find the first and last occurrences of `ana` at indices 1 and 2, respectively. The reason we can work like this is because the suffix array is lexicographically sorted, so that all the suffixes that start with  pattern $P$ will be adjacent to each other in the suffix array. The matching phase takes $O(m\log n)$ time, as a comparison takes $O(m)$ time, and we perform $O(\log n)$ comparisons in the binary search. Expanding the range of suffixes takes $O(n)$ time in the worst case, as we can have a pattern that matches all the suffixes in the text. Thus, the total time complexity is $O(m\log n + n) = O(m\log n)$. [^1]

We can also find the longest repeated substring(s) in a text by finding the maximum value in the LCP array and then going through the LCP array again to find all the indices where the LCP array has this maximum value. If this maximum value appears at index $i$, then the longest repeated substring is the substring starting at index $SA[i]$ with length $LCP[i]$. In the example above, we find the maximum value in the LCP array to be 3. Thus, the longest repeated substring starts at $SA[2] = 1$ and has length $LCP[2] = 3$, and is the substring `ana`.

Another thing we can easily compute is the cardinality of the set containing all substrings of the text, i.e. the number of all substrings in $T$ without counting any duplicates. We can do this by taking the sum of the lengths of the suffixes (which represents the total number of substrings) and then subtracting the sum of the values in the LCP array (which represents the number of duplicate substrings). Thus, the total number of non-duplicate substrings in $T$ is:

$$\frac{n (n + 1)}{2}  - \sum_{i=0}^{n-1} LCP[i]$$

In the example above, the number of substrings is $\frac{6 \cdot 7}{2} - (0 + 1 + 3 + 0 + 0 + 2) = 15$. Which is indeed the cardinality of the set: {`b`, `a`, `n`, `ba`, `an`, `na`, `ban`, `ana`, `nan`, `bana`, `anan`, `nana`, `banan`, `anana`, `banana`}.

[^1]: The book mentions O(mlogn + km) time, but as shown in the implementation, this can be easily reduced to O(mlogn) time by using the precomputed LCP array to find the first and last occurrences of P in T.

Yet another application is finding the longest palindromic substring in $T$. This can be found by first concatenating the text with its reverse, and computing the SA and LCP arrays for this concatenated text. After that, we simply look for the maximum value in the LCP array for which the values in the suffix array point to suffixes that come from different halves of the concatenated text but from the same substring in the original text (so that we are sure that they point to a single palindromic substring, and not to two substrings at different positions in the original text which happen to be each other's reverse). The latter can be easily checked by verifying that $SA[i] = 2\cdot n - SA[i-1] - LCP[i]$. For example, if $T =$ `banana`, then the concatenated text becomes `bananaananab`, and the longest palindromic substring is `anana` with length 5, which is the maximum value in the LCP array for the concatenated text for which the mentioned check holds.

## Constructing the suffix array

