${\color{Cadetblue}\text{\huge Minimal Palindromic}}$  
${\color{Cadetblue}\text{\huge Partitioning (MPP)}}$

<br />

${\color{rosybrown}\text{\Large Problem}}$

Given a string $S$ of length $n$, find the minimum number of cuts required to partition the string into palindromic substrings. A palindromic substring is a substring that reads the same forwards and backwards. Thus, a single character is a palindrome, as is any substring of length $2$ that consists of two identical characters.

For example, for the string `minimum` the minimum number of cuts required is $2$ resulting in a partitioning of $3$ palindromic substrings. An optimal partitioning is `m` | `ini` | `mum`. This partitioning is not unique, however. Another optimal partitioning for the same string is `minim` | `u` | `m`.

<br />

${\color{darkseagreen}\text{\Large The key idea}}$

Let $P(i, j)$ be a function that returns ${\color{cornflowerblue}\text{true}}$ if the substring $S[i:j]$ is a palindrome and ${\color{orchid}\text{false}}$ otherwise. This function can be precomputed in $\mathcal{O}(n^2)$ time, using the following recurrence relation:

$$
P(i, j) = \begin{cases}
{\color{cornflowerblue}\text{true}} & \scriptsize \text{if } i = j \\
{\color{cornflowerblue}\text{true}} & \scriptsize \text{if } S[i] = S[j] \\
& \space \scriptsize \land \space P(i+1, j-1) \\
{\color{orchid}\text{false}} & \scriptsize \text{otherwise}
\end{cases}
$$

<br />

Now we have a means to determine the basic building blocks of the problem (i.e., palindromic substrings) in constant time. The next step then is to identify the subproblems of the MPP problem. The idea here is to think of the problem in terms of ${\color{peru}\text{prefixes}}$ of the input string $S$, and of each subproblem as the task of finding the minimum number of cuts required to partition the prefix $S[0:i]$ into palindromic substrings, for some $0 \leq i < n$. In other words, we want find the minimum number of atomic building blocks (palindromic substrings) required to construct the prefix $S[0:i]$.

Obviously, if the prefix $S[0:i]$ is already a palindrome, then no cuts are required. Otherwise, we need to consider all possible ways to partition this prefix into palindromic substrings in order to find the partition with the minimum number of cuts: that is, we need to consider all possible cuts between $S[0:j]$ and $S[j+1:i]$, for all $0 \leq j < i$ and then take the minimum of all such combinations.

Just as in the case of the [rod-cutting problem](https://github.com/pl3onasm/CLRS/tree/main/algorithms/dynamic-programming/rod-cutting), we can simplify this task by considering all possible combinations of an uncut piece (i.e. a substring that is already a palindrome) and the remaining piece (prefix) that needs to be partitioned further. That is, we can compute the combinations by considering all possible cuts between the remaining prefix $S[0:j]$, our new subproblem, and the uncut piece $S[j+1:i]$, for which $P(j+1, i)$ holds.

Clearly, the subproblems are ${\color{peru}\text{overlapping}}$: the same subproblem can be encountered multiple times in the process of solving the original problem. They also exhibit ${\color{peru}\text{optimal substructure}}$, since the optimal partitioning of a prefix $S[0:i]$ can be constructed from the optimal partitioning of its subproblems.

Now, let $C(i)$ be the minimum number of cuts required to partition the prefix $S[0:i]$ into palindromic substrings. The recurrence relation for the MPP problem can then be expressed as follows:

$$
\color{darkslateblue}\huge\boxed{\color{rosybrown}\normalsize \space
C(i) = \begin{cases}
\small 0 & \scriptsize \text{if } P(0, i) \\
\small \min \lbrace C(j) + 1 :  & \scriptsize \text{otherwise}\\
\quad \small 0 \leq j < i \land P(j+1, i) \rbrace
\end{cases}\space}
$$

<br />

The base case is when the prefix $S[0:i]$ is already a palindrome, in which case no cuts are required. The smallest such prefix is a single character (i.e., $i = 0$), which is always a palindrome. The recursive case is when the prefix $S[0:i]$ is not a palindrome. In that case, we need to make a cut and add $1$ to the minimum number of cuts required to partition the remaining subproblem $S[0:j]$ into palindromic substrings. The answer to the original problem is obtained by computing $C(n-1)$ for the entire string $S$.

<br />

${\color{darkseagreen}\text{\Large Brute force}}$

Solving the MPP problem by brute force is simply a matter of implementing the recurrence relation above, without any optimization. The time complexity of this approach is $\mathcal{O}(2^n)$, since there are $n - 1$ points between each pair of characters in the string $S$ where a cut can be made, and at each of these points we are faced with a Shakespearean dilemma: to cut or not to cut. This results in a total of $2^{n-1}$ possible combinations of cuts, which is the number of leaves in the recursion tree.

Implementation: [MPP - Brute Force](https://github.com/pl3onasm/CLRS/blob/main/algorithms/dynamic-programming/min-pal-part/mpp-1.c)

<br />

${\color{darkseagreen}\text{\Large Top-down}}$

The brute force approach can be optimized by using ${\color{peru}\text{memoization}}$ to store the results of subproblems that have already been solved. This way, we avoid recomputing the same subproblem multiple times. As a result, the time complexity of the top-down approach is $\mathcal{O}(n^2)$, since there are $n^2$ ${\color{peru}\text{unique}}$ subproblems that need to be solved: $n$ recursive calls, each with $n$ possible subproblems, where each subproblem is uniquely identified by its combination of indices $i$ and $j$.

Implementation: [MPP - Top-down](https://github.com/pl3onasm/CLRS/blob/main/algorithms/dynamic-programming/min-pal-part/mpp-2.c)

<br />

${\color{darkseagreen}\text{\Large Bottom-up}}$

The bottom-up approach is a more natural way to solve the MPP problem. It is based on the observation that the subproblems can be solved in a ${\color{peru}\text{topological order}}$ that is determined by the lengths of the prefixes. That is, we first solve the subproblems for prefixes of length $1$, then for prefixes of length $2$, and so on, until we reach the prefix of length $n$. This way, we can ensure that the results of all subproblems that are needed to solve a particular subproblem have already been computed when we get to that subproblem. This is in contrast to the top-down approach, where the order in which the subproblems are solved is determined by the order in which they are encountered during the recursion. The time complexity, however, remains the same, and is in $\mathcal{O}(n^2)$.

Implementation: [MPP - Bottom-up](https://github.com/pl3onasm/CLRS/blob/main/algorithms/dynamic-programming/min-pal-part/mpp-3.c)
