$\huge{\color{Cadetblue}\text{String-matching}}$

<br/>

$\Large{\color{Rosybrown}\text{Overview}}$

| ${\color{peru}\text{CLRS}}$ | ${\color{peru}\text{Topic}}$ |
|:---|:---|
| 32.1 | [Naive String-matching](https://github.com/pl3onasm/AADS/tree/main/algorithms/string-matching/naive-matching) |
| 32.2 | [Rabin-Karp Algorithm](https://github.com/pl3onasm/AADS/tree/main/algorithms/string-matching/rabin-karp) |
| 32.3 | [String-matching Automata](https://github.com/pl3onasm/AADS/tree/main/algorithms/string-matching/finite-automata) |
| 32.4 | [Knuth-Morris-Pratt Algorithm](https://github.com/pl3onasm/AADS/tree/main/algorithms/string-matching/knuth-morris-pratt) |
| 32.5 [ed4] | [Suffix Arrays](https://github.com/pl3onasm/CLRS/tree/main/algorithms/string-matching/suffix-arrays) |

<br/>

$\Large{\color{Rosybrown}\text{String-matching Problem}}$

String matching is the problem of finding all occurrences of a string pattern $P$ of length $m$ in a text $T$ of lenght $n$, where $m \leq n$. The characters in the pattern and the text come from a finite set $\Sigma$ called the $\color{orchid}{\text{alphabet}}$.

We say that pattern $P$ occurs with shift $s$ in text $T$ if $0 \leq s \leq n - m$ and $T[s:s + m -1] = P[0:m-1]$. If $P$ occurs with shift $s$ in $T$, then $s$ is called a $\color{orchid}{\text{valid shift}}$ for $P$ in $T$, otherwise $s$ is called an $\color{orchid}{\text{invalid shift}}$ for $P$ in $T$.

In other words, the string-matching problem is to find all valid shifts for $P$ in $T$, i.e. the shift set of $P$ in $T$. Using the terminology below, the string-matching problem is to $\color{mediumpurple}{\text{find all shifts } s}$ $\color{mediumpurple}{\text{such that } P \sqsupset T_{s+m}}$, where $0 \leq s \leq n - m$ and $T_{s + m}$ is the prefix of $T$ of length $s + m$.

<br/>

$\Large{\color{Rosybrown}\text{Terminology and properties}}$

- $\Sigma^*$: the set of all finite-length strings over $\Sigma$
- $\epsilon$: the empty string, i.e. the string of length $0$
- $|x|$: the length of string $x$
- $xy$: the concatenation of strings $x$ and $y$, with $|xy| = |x| + |y|$
- $T_q$: the q-character prefix of string $T$, i.e. $T_q = T[0:q-1]$
- $x \sqsubset y$: string $x$ is a $\color{orchid}{\text{prefix}}$ of string $y$, i.e. $y = xz$ for some string $z \in \Sigma^*$
- $x \sqsupset y$: string $x$ is a $\color{orchid}{\text{suffix}}$ of string $y$, i.e. $y = zx$ for some string $z \in \Sigma^*$
- $x \sqsubset y \Rightarrow |x| \leq |y|$
- $x \sqsupset y \Rightarrow |x| \leq |y|$
- $x \sqsubset y \land |x| < |y|$: $x$ is a proper prefix of $y$
- $x \sqsupset y \land |x| < |y|$: $x$ is a proper suffix of $y$
- $x \in \Sigma^* \Rightarrow \epsilon \sqsubset x$
- $x \in \Sigma^* \Rightarrow \epsilon \sqsupset x$
- $\forall a \in \Sigma: x \sqsupset y \Leftrightarrow xa \sqsupset ya$
- $\forall z \in \Sigma^*: x \sqsupset y \land y \sqsupset z \Rightarrow x \sqsupset z$
- $\forall z \in \Sigma^*: x \sqsubset y \land y \sqsubset z \Rightarrow x \sqsubset z$
