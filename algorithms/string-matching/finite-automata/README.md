# String-matching Automata

## Problem

Given a pattern $P[0..m-1]$ and a text $T[0..n-1]$, find all occurrences of $P$ in $T$. It is assumed that $n \geq m$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts, i.e. the valid shifts for $P$ in $T$.

## Approach based on a finite automaton

A finite automaton is a state machine for processing information and is used in many areas of computer science. It reads input symbols and makes transitions from one state to another according to a transition function. Its behavior can be modeled as a directed graph. The nodes of the graph are the states $q \in Q$ of the automaton, and the edges are the transitions from one state to another. The automaton starts in a designated start state $q_0$ and reads one input symbol at a time. Based on the current state $q$ and the current input symbol $a \in \Sigma$, the automaton makes a transition $q \rightarrow \delta(q, a)$ to another state or the same state according to a transition function $\delta : Q \times \Sigma \rightarrow Q$. The automaton accepts a string if it ends in a state that belongs to the set of accepting states $A \subseteq Q$. An automaton thus also defines a final-state function $\phi : \Sigma^* \rightarrow Q$ that maps a string to the state that the automaton ends in when it has read the string. The automaton accepts a string $x$ if $\phi(x) \in A$.

In order to use a finite automaton for string matching, the automaton must be constructed so that it accepts all strings that end in a state that corresponds to a valid shift for $P$ in $T$. This automaton is constructed from the pattern $P$ in a preprocessing phase and then used to scan the text $T$ for occurrences of $P$ in a matching phase in $O(n)$. The set $Q$ of states of the automaton is the set $\lbrace 0, 1, ..., m \rbrace$, representing the possible lengths of a prefix of $P$ that has been matched against $T$ so far. The start state is $q_0 = 0$ and the set of accepting states is $A = \lbrace m \rbrace$, since a string should only be accepted when the entire pattern $P$ of length $m$ has been matched against $T$.

The transition function $\delta$ is defined as follows:

$$
\delta(q, a) = \begin{cases}
q + 1 & \text{if } q < m \text{ and } a = P[q + 1] \\
\sigma(P_qa) & \text{otherwise}\\
\end{cases}
$$

where $\sigma : \Sigma^*\rightarrow \{0, 1, ..., m\}$ is an auxiliary function, called the suffix function, which maps a string $x \in \Sigma^*$ to the length of the longest prefix of $P$ that is also a suffix of $x$:

$$
\sigma(x) = \max \lbrace k : P_k \sqsupset x \rbrace
$$

During the matching phase, the automaton takes characters from $T$ one at a time and tries to build a match of $P$ against the most recently read characters. At any point in this phase, the automaton is in a state $q$ that represents the length of the longest prefix of $P$ that is also a suffix of the most recently read characters. As the automaton reads the next character $a$ from $T$, it makes a transition from state $q$ to $q + 1$ if $a$ extends the match of $P$ against $T$ by one character, i.e. if $P_qa$ is a prefix of $P$. Otherwise, the automaton makes a transition to state $\sigma(P_qa)$, which is the longest prefix of $P$ that is also a suffix of $P_qa$. Each time the automaton makes a transition to state $m$ after having read a overall total of $i$ characters from $T$ so far, it has reached an accepting state where $\phi(T_i) = m$, and thus found an occurrence of $P$ in $T$ with shift $i - (m - 1) = i - m + 1$.

The values of the transition function $\delta$ can be retrieved in constant time during the matching phase by building a transition table of size $m \times |\Sigma|$ in the preprocessing phase. This table is populated by considering all possible values of $q$ and $a$ and computing $\delta(q, a)$ for each combination of $q$ and $a$ using the definition of $\delta$ above. The running time of the preprocessing phase is $O(m^3|\Sigma|)$, since there are $m$ possible values of $q$, $d = |\Sigma|$ possible values of $a$, and each value of $\sigma(P_qa)$ can be computed in $O(m)$ time. The running time of the matching phase is $O(n)$, since each character of $T$ is read once and each transition can be performed in constant time using the transition table.

Implementation: [SMA 1](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/finite-automata/sma-1.c)

It is possible to precompute the transition function in $O(m^2|\Sigma|)$ time and even in $O(m|\Sigma|)$ time by using some shortcuts in the computation of $\sigma(P_qa)$.

Implementation: [SMA 2](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/finite-automata/sma-2.c)

Implementation: [SMA 3](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/finite-automata/sma-3.c)
