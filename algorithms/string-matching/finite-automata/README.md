# ${\color{Cadetblue}\text{String-matching Automata}}$

## ${\color{Rosybrown}\text{Problem}}$

Given a pattern $P[0:m-1]$ and a text $T[0:n-1]$, find all occurrences of $P$ in $T$. It is assumed that $n \geq m$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts, i.e. the valid shifts for $P$ in $T$.

## ${\color{darkseagreen}\text{String matching with a DFA}}$

A $\color{darkgoldenrod}\text{deterministic finite automaton (DFA)}$ is a state machine for processing information and is used in many areas of computer science. It reads input symbols and makes transitions from one state to another according to a transition function. Its behavior can be modeled as a directed graph. The nodes of the graph are the states $q \in Q$ of the automaton, and the edges are the transitions from one state to another. The automaton starts in a designated start state $q_0$ and reads one input symbol at a time. Based on the current state $q$ and the current input symbol $a \in \Sigma$, the automaton makes a transition $q \rightarrow \delta(q, a)$ to a new state given by a $\color{darkkhaki}\text{transition function }$ $\color{darkkhaki}\delta : Q \times \Sigma \rightarrow Q$. The automaton accepts a string if it ends in a state that belongs to the set of accepting states $A \subseteq Q$. An automaton thus also defines a final-state function $\phi : \Sigma^* \rightarrow Q$ that maps a string to the state that the automaton ends in when it has read the string. The automaton accepts a string $x$ if $\phi(x) \in A$.

In order to use a finite automaton for string matching, the automaton must be constructed so that it accepts all strings that end in a state that corresponds to a valid shift for $P$ in $T$. This automaton is constructed from the pattern $P$ in a preprocessing phase and then used to scan the text $T$ for occurrences of $P$ in a matching phase in $\mathcal{O}(n)$. The set $Q$ of states of this automaton is the set $\lbrace 0, 1, ..., m \rbrace$, representing the possible lengths of a prefix of $P$ that has been matched against $T$ so far. The start state is $q_0 = 0$ and the set of accepting states is $A = \lbrace m \rbrace$, since a substring should only be accepted when the entire pattern $P$ of length $m$ has been matched against $T$.

The $\color{darkkhaki}\text{transition function }\delta$ is defined in terms of a function $\sigma : \Sigma^\ast \rightarrow \lbrace 0, 1, ..., m \rbrace$, called the $\color{peru}\text{suffix function}$, which maps a string $x \in \Sigma^\ast$ to the length of the longest prefix of $P$ that is also a suffix of $x$. In this particular case, the string $x$ is the concatenation of the prefix $P_q$ that has been matched against $T$ so far and the next input symbol $a$. Thus, the transition function $\delta$ is defined as follows, with $k \leq m$: [^1]

$$
\color{mediumpurple}\boxed{\space
\begin{align*}
\color{darkkhaki}\space \delta(q,a) &\color{peru} = \sigma(P_qa)\\
 &\color{rosybrown} =\max \lbrace k : P_k \sqsupset P_qa \rbrace\\
\end{align*}
\space}
$$

During the matching phase, the automaton takes characters from $T$ one at a time and tries to build a match of $P$ against the most recently read characters. At any point in this phase, the automaton is in a state $q$ that represents the length of the longest prefix of $P$ that is also a suffix of the most recently read characters. As the automaton reads the next character $a$ from $T$, it makes a transition from state $q$ to a new state $q' = q + 1$ if $a$ extends the match of $P$ against $T$ by one character, i.e. if $P[q] = a$ so that $P_{q+1} \sqsupset P_qa$.[^2] Otherwise, in the case where $P[q] \neq a$, the automaton makes a transition to the next best state given by the same suffix function $\sigma(P_qa)$, which now yields a new state $q' \leq q$. The automaton continues to read characters from $T$ this way, making appropriate transitions as it moves from state to state. Each time it transitions to a new state $q' = m$ after having read an overall total of $i$ characters from $T$ so far, it has reached an accepting state where $\phi(T_i) = m$, and thus found an occurrence of $P$ in $T$ with shift $i - 1 - (m - 1)$ $= i - m$[^3].

The values of the transition function $\delta$ can be retrieved in constant time during the matching phase by building a $\color{darkkhaki}\text{transition table}$  of size $m \times |\Sigma|$ in the preprocessing phase. This table is populated by considering all possible values of $q$ and $a$ and computing $\delta(q, a)$ for each combination of $q$ and $a$. The running time of this preprocessing phase is in $\mathcal{O}(m^3|\Sigma|)$, since there are $m$ possible values of $q$, $d = |\Sigma|$ possible values of $a$, and for each combination we need to determine the maximum length $k$ such that $P_k \sqsupset P_qa$. This last step takes $\mathcal{O}(m^2)$ time, since we need to try each possible value of $k$ by decrementing $k$ at most $m$ times and at each of these steps we have to check whether $P_k \sqsupset P_qa$ by comparing at most $m$ characters. The running time of the matching phase, on the other hand, is in $\mathcal{O}(n)$, since each character of $T$ is read once and each transition can be performed in constant time using the transition table.

Implementation: [SMA](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/finite-automata/sma.c)

[^1]: We use the notation P<sub>q</sub>a to denote the string P[0:q-1]a, i.e. the q-character prefix of P followed by the character a.

[^2]: Since we are working with 0-based indices (contrary to the book), P[q] is in fact the (q+1)-st character of P. Suffix function values, just like state values, pertain to string lengths, not indices. Thus, there is always a difference of 1 when switching between state or suffix function values and string indices.

[^3]: T<sub>i</sub> denotes the string T[0:i-1]. The shift is computed by subtracting the last index of the pattern from the index of the last character of the text that was read, i.e. i - 1 - (m - 1) = i - m.
