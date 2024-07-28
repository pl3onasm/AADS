${\color{Cadetblue}\text{\huge Minimum Edit}}$  
${\color{Cadetblue}\text{\huge Distance (MED)}}$

<br />

${\color{rosybrown}\text{\Large Problem}}$

Given are two strings $S$ and $T$, and a set of operations each with an associated cost. Our task is to transform the source string $S$ to the target string $T$, by applying a sequence of operations on $S$, such that the total cost of the transformation is minimized.

At each step $i$ of the transformation process, we can choose from the following operations:

- Copy the character $S[i]$ with cost $c_c$
- Insert a character at position $i$ with cost $c_i$
- Delete the character $S[i]$ with cost $c_d$
- Replace the character $S[i]$ with cost $c_r$
- Swap the characters $S[i]$ and $S[i+1]$ with cost $c_s$
- Kill the remaining characters in $S$ with cost $c_k$

For example, given $S =$ `five` and $T =$ `six`, with costs $c_c = 1$, $c_i = 2$, $c_d = 3$, $c_r = 4$, $c_s = 5$, and $c_k = 6$, an optimal operation sequence is:

- `|five` &emsp; Replace `f` by `s`
- `s|ive` &emsp; Copy `i`
- `si|ve` &emsp; Replace `v` by `x`
- `six|e` &emsp; Delete `e`
- `six|`

The total cost of this transformation is $4 + 1 + 4 + 3 = 12$. This is the minimum edit distance between the two strings $S$ and $T$.

<br />

${\color{darkseagreen}\text{\Large The key idea}}$

