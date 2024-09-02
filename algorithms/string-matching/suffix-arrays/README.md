$\huge{\color{Cadetblue}\text{Suffix arrays}}$

<br/>

If besides finding occurrences of a pattern in a text, we are also interested in finding things like the longest repeated substring, the number of (non-duplicate) substrings, the longest common substring between two texts, or the longest palindromic substring in a text, we can use a data structure called a ${\color{mediumorchid}\text{suffix array}}$, denoted $SA[0:n-1]$, which is the lexicographically sorted array of all $n$ suffixes of a given string $T[0:n-1]$. In this suffix array, we do not store the suffixes themselves, but store the starting index of each suffix in the original string. For example, the suffix array for the string $T=$`banana` is the second column in the table below.  
Along with the suffix array, we also want to compute the ${\color{mediumorchid}\text{LCP array}}$ which yields the longest common prefix between adjacent suffixes in the suffix array, i.e. the length of the longest common prefix between $T[SA[i-1]:n-1]$ and $T[SA[i]:n-1]$ for $i=1,2,\ldots,n-1$. For example, the longest common prefix between the suffixes `ana` and `anana` is `ana`, so $LCP[2]=3$, and the longest common prefix between the suffixes `banana` and `na` is the empty string, so $LCP[4]=0$. The third column in the table below is the LCP array for the string `banana`.  
&nbsp;
<div align="center">

 ${\color{peru}\text{i}}$      |  ${\color{peru}\text{SA[i]}}$  | ${\color{peru}\text{LCP[i]}}$  | ${\color{peru}\text{suffix}}$ |
-------|:-----:|:------:|:-------|
0      | 5     | 0      | a      |
1      | 3     | 1      | ana    |
2      | 1     | 3      | anana  |
3      | 0     | 0      | banana |
4      | 4     | 0      | na     |
5      | 2     | 2      | nana   |

</div>

<br/>

$\Large{\color{darkseagreen}\text{Applications}}$

<br/>


$\Large{\color{rosybrown}\text{1. String matching}}$

After constructing the suffix array, we can find all occurrences of a pattern $P[0:m-1]$ in a text $T[0:n-1]$ in $\mathcal{O}(m \log n)$ time. We can do this by performing a ${\color{mediumorchid}\text{binary search}}$ on the suffix array. For example, if we want to find all occurrences of the pattern `ana` in the text `banana`, we can perform a binary search on the suffix array to find an occurrence of `ana`. After that, we can find the range of suffixes that start with `ana` by trying to expand the range to the left and to the right in the suffix array until we find the first and last occurrences of `ana` at indices 1 and 2, respectively. The reason we can work like this is because the suffix array is lexicographically sorted, so that all the suffixes that start with  pattern $P$ will be adjacent to each other in the suffix array. The matching phase takes $\mathcal{O}(m\log n)$ time, as a comparison takes $\mathcal{O}(m)$ time, and we perform $\mathcal{O}(\log n)$ comparisons in the binary search. Expanding the range of suffixes takes $\mathcal{O}(n)$ time in the worst case, as we can have a pattern that matches all the suffixes in the text. Thus, the total time complexity is $\mathcal{O}(m\log n + n) = \mathcal{O}(m\log n)$.[^1]

[^1]: The book mentions O(mlogn + km) time, but as shown in the implementation, this can be easily reduced to O(mlogn) time by using the precomputed LCP array to find the first and last occurrences of P in T.

<br/>


$\Large{\color{rosybrown}\text{2. Longest repeated substring}}$

We can also find the longest repeated substring(s) in a text by finding the maximum value in the LCP array and then going through the LCP array again to find all the indices where the LCP array has this maximum value. If this maximum value appears at index $i$, then the longest repeated substring is the substring starting at index $SA[i]$ with length $LCP[i]$. In the example above, we find the maximum value in the $LCP$ array to be 3. Thus, the longest repeated substring starts at $SA[2] = 1$ and has length $LCP[2] = 3$, and is the substring `ana`.

<br/>

$\Large{\color{rosybrown}\text{3. Number of substrings}}$

Another thing we can easily compute is the cardinality of the set containing all unique substrings of the text, i.e. the number of all substrings in $T$ without counting any duplicates. We can do this by taking the sum of the lengths of the suffixes (which represents the total number of substrings) and then subtracting the sum of the values in the $LCP$ array (which represents the number of duplicate substrings). Thus, the total number of non-duplicate substrings in $T$ is:

$${\color{cornflowerblue}\frac{n (n + 1)}{2}  - \sum_{i=0}^{n-1} LCP[i]}$$

In the example above, the number of substrings is $\frac{6 \cdot 7}{2} - (0 + 1 + 3 + 0 + 0 + 2) = 15$. This is indeed the cardinality of the set: {`b`, `a`, `n`, `ba`, `an`, `na`, `ban`, `ana`, `nan`, `bana`, `anan`, `nana`, `banan`, `anana`, `banana`}.  

<br/>

$\Large{\color{rosybrown}\text{4. Longest palindromic substring}}$

Yet another application is finding the longest palindromic substring in $T$. This can be found by first concatenating the text with its reverse, and computing the $SA$ and $LCP$ arrays for this concatenated text. After that, we simply look for the maximum value in the $LCP$ array for which the corresponding values of the current and preceding index in the suffix array point to suffixes coming from different halves of the concatenated text but from the same substring in the original text (to ensure they point to a single palindromic substring, and not to two substrings at different positions in the original text which happen to be each other's reverse). The latter constraint can be easily imposed by checking that $SA[i] = 2\cdot n - SA[i-1] - LCP[i]$. [^2] For example, if $T =$ `banana`, then the concatenated text becomes `bananaananab`, and the longest palindromic substring is `anana` with length 5, which is the maximum value in the $LCP$ array for the concatenated text that satisfies the constraint above.

[^2]: This is because the suffixes in the first half of the concatenated text are the same as the suffixes in the second half, but in reverse order. Thus, we should find the same starting character at SA[i] in the first half, at SA[i-1] + LCP[i] in the second half, so that the equality for the indices, SA[i] = 2n - SA[i-1] - LCP[i], should hold if the suffixes point to the same substring in the original text.

<br/>

$\Large{\color{darkseagreen}\text{Building the suffix array}}$

The suffix array can be constructed in $\mathcal{O}(n\log n)$ time, using an algorithm that was developed by Manber and Myers. It is based on the idea of sorting the suffixes by their first $2^k$ characters, where $k$ is initially 1 and is doubled in each iteration until $2^k \geq n$. For the sorting, a stable sorting algorithm is used, such as ${\color{peru}\text{radix sort}}$, which in each iteration sorts the suffixes by their first $2^k$ characters by first sorting them by their second $2^{k-1}$ characters, and then by their first $2^{k-1}$ characters.

In order to keep each iteration in $\mathcal{O}(n)$ time, these suffix halves are not sorted explicitly, but are assigned ${\color{peru}\text{ranks}}$, represented by integers in the range $[0, n-1]$ and denoting the current sorting order of the suffixes by their first $2^{k-1}$ characters. Initially, the ranks are given by the ASCII values of the first and second characters of the suffix. Then, in each iteration, these ranks are updated according to the sorting of the suffixes by their first $2^{k-1}$ characters in the previous iteration: the left rank of the first half of the suffix of length $2^k$ is the rank of the suffix of length $2^{k-1}$ that starts at the same position as the suffix of length $2^k$, and the right rank of the second half of the suffix is the rank of the suffix of length $2^{k-1}$ that starts at position $i + 2^{k-1}$. These ranks are then used to sort the suffixes by their first $2^k$ characters. The algorithm continues until $2^k \geq n$, at which point the suffix array is constructed.

Radix sort is used because we need to sort suffixes represented by two integers in the range $[0, n-1]$, which are the ranks of the two halves of the suffix (so the radix $d=2$) with length at most $2^k$. It is important that this sorting algorithm is stable, because we need to sort the suffixes by their second half first (represented by the right rank), and then by their first half (represented by the left rank), so that the suffixes that have the same second half are sorted by their first half and the relative order of the suffixes is preserved.

<br/>

$\Large{\color{darkseagreen}\text{Building the LCP array}}$

Building the $LCP$ array in lexicographical order would take $\mathcal{O}(n^2)$ time, as we would need to explicitly compare each suffix with the next one in the suffix array. However, it is possible to build the $LCP$ array in $\mathcal{O}(n)$ time by constructing it in ${\color{peru}\text{text order}}$, i.e. in the order of the suffixes as they appear in the original text. This allows us to use the fact that the length of the next longest common prefix is at least 1 less than the previous length as we move to the next suffix in the text, since the next suffix in the original text is a suffix of the previous suffix (as at each step, while moving through the text, the first character is removed from the previous suffix). In other words, if a suffix at index $i$ in the text shares a common prefix of length $l$ with its lexicographically predecessor in the text, then the next suffix in the text must share a common prefix of length at least $l-1$ with its lexicographically predecessor in the text.  
Thus, we can avoid comparing each suffix explicitly with the next one by traversing the suffix array in text order, and keep computing the LCP length for the next suffix by taking the LCP length of the previous suffix, decrementing it by 1, and then trying to extend it by comparing the next characters in the text until we find a mismatch. This way, we can build the $LCP$ array in $\mathcal{O}(n)$ time.  

<br/>

$\Large{\color{darkseagreen}\text{Implementation}}$

The implementation reads a text from standard input and computes the suffix array and LCP array for it. It then reads a pattern from standard input and finds all occurrences of the pattern in the text. It also finds the longest repeated substring, the number of unique substrings, and the longest palindromic substring in the text.

Implementation: [Suffix array](https://github.com/pl3onasm/CLRS/blob/main/algorithms/string-matching/suffix-arrays/sa.c)

<br/>

$\Large{\color{darkseagreen}\text{Video}}$

[![Suffix arrays](https://img.youtube.com/vi/zqKlL3ZpTqs/0.jpg)](https://www.youtube.com/watch?v=zqKlL3ZpTqs&list=PLDV1Zeh2NRsCQ_Educ7GCNs3mvzpXhHW5)
