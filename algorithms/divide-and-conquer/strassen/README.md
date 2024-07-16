${\color{Cadetblue}\text{\huge Matrix Multiplication}}$

<br/>

${\color{rosybrown}\text{\Large Problem}}$

Given two square matrices $A$ and $B$ of size $n \times n$, we want to compute their product $C = A \cdot B$. The result should be a square matrix of size $n \times n$.

<br/>

${\color{darkseagreen}\text{\Large Standard Matrix}}$ ${\color{darkseagreen}\text{\Large Multiplication}}$

If we want to multiply two $n \times n$ matrices $A$ and $B$, we can do so by taking the ${\color{peru}\text{dot product}}$ of each row of $A$ with each column of $B$. This is the naive approach, which is shown in the following pseudocode:

```c
  matrixMultiply(A, B, C, n)
    for i = 1 to n
      for j = 1 to n
        Cᵢⱼ = 0
        for k = 1 to n
          Cᵢⱼ += Aᵢⱼ * Bᵢⱼ
    return C
```

In the above pseudocode, $Cᵢⱼ$ is the element in the $i$-th row and $j$-th column of $C$. Since we have three nested loops iterating over $n$ elements, the time complexity of this algorithm is in $\mathcal{O}(n^3)$.

We can turn this algorithm into a divide and conquer algorithm by splitting the matrices $A$ and $B$ into four $n/2 \times n/2$ ${\color{peru}\text{block-matrices}}$ and then multiplying them recursively. Doing so, we make use of the fact that the standard rule for matrix multiplication is also true for block-matrices:  

$$ \begin{align*}
   &\quad
   \begin{pmatrix}
   A_{11} & A_{12}\\
   A_{21} & A_{22}\\
   \end{pmatrix}
   \cdot
   \begin{pmatrix}
   B_{11} & B_{12}\\
   B_{21} & B_{22}\\
   \end{pmatrix}\\
   \quad\\
    &=
  \begin{pmatrix}
  A_{11}B_{11} + A_{12}B_{21} & A_{11}B_{12} + A_{12}B_{22}\\
  A_{21}B_{11} + A_{22}B_{21} & A_{21}B_{12} + A_{22}B_{22}\\
  \end{pmatrix}\\
  \quad\\
    &=
  \begin{pmatrix}
  C_{11} & C_{12}\\
  C_{21} & C_{22}\\
  \end{pmatrix}
  \end{align*}
$$

Repeatedly dividing the matrices in submatrices of size $n/2 \times n/2$ in order to be able to apply a divide and conquer algorithm, however, presumes that $n$ is a power of two. Only this way can we keep halving the matrix dimensions until we reach the base case, where we have matrices of size $1 \times 1$, which is nothing but ${\color{peru}\text{scalar multiplication}}$. If $n$ is not a power of two, we can simply pad the original matrices with zeros until we get a matrix of size $2^k \times 2^k$, where $k$ is the smallest integer such that $2^k \geq n$. Padding the matrices this way makes it even possible to generalize the algorithm to input matrices of arbitrary dimensions, while the core algorithm works for matrices of size $2^k \times 2^k$. The padding does not affect the time complexity either, since it only adds a constant number of operations to the first recursive step of the algorithm.

Looking at the above formula, we can see that we need to compute 8 products of matrices of size $n/2 \times n/2$ at each dividing step. If we use index calculations to partition the matrices at each step, the partitioning itself only takes $\Theta(1)$ time. All this leads to the following recurrence relation:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 8T(n/2) + \Theta(1)\space}\\
$$

By case 1 of the master theorem (CLRS 4.5), we can conclude that $T(n) = \Theta(n^3)$, which is the same time complexity as the naive algorithm, and so the question arises whether it is possible to break this cubic barrier.

Implementation: [Matrix Multiplication - D&C](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/strassen/mmp-1.c)

<br/>

${\color{darkseagreen}\text{\Large Strassen's Matrix}}$ ${\color{darkseagreen}\text{\Large Multiplication}}$

The idea behind Strassen's algorithm is to reduce the number of (quadratic time) multiplications performed at each dividing step from 8 to 7 by substituting one of the multiplications with a number of (linear time) additions and subtractions. This way, it is possible to reduce the time complexity of the matrix multiplication algorithm to below $\Theta(n^3)$.

The ${\color{peru}\text{reduction}}$ ${\color{peru}\text{of the number}}$ ${\color{peru}\text{of multiplications}}$ at each recursive step is achieved by first computing the following 10 sums of matrices of size $n/2 \times n/2$:

$$
\begin{align*}
  S_1 &= B_{12} - B_{22}\quad&
  S_2 &= A_{11} + A_{12} \\
  S_3 &= A_{21} + A_{22}\quad&
  S_4 &= B_{21} - B_{11} \\
  S_5 &= A_{11} + A_{22}\quad&
  S_6 &= B_{11} + B_{22} \\
  S_7 &= A_{12} - A_{22}\quad&
  S_8 &= B_{21} + B_{22} \\
  S_9 &= A_{11} - A_{21}\quad&
  S_{10} &= B_{11} + B_{12}
\end{align*}
$$

Next, we compute the following 7 products of matrices of size $n/2 \times n/2$:

$$
\begin{align*}
  P_1 &= A_{11} \cdot S_1\quad& P_2 &= S_2 \cdot B_{22}\\
  P_3 &= S_3 \cdot B_{11}\quad& P_4 &= A_{22}\cdot S_4\\
  P_5 &= S_5 \cdot S_6\quad& P_6 &= S_7 \cdot S_8\\
  P_7 &= S_9 \cdot S_{10}\quad& &
\end{align*}
$$

Finally, we conclude the recursive step by using the results of the above calculations to update the following 4 submatrices of the final matrix $C$:

$$
\begin{align*}
  C_{11} &= P_5 + P_4 - P_2 + P_6\\
  C_{12} &= P_1 + P_2\\
  C_{21} &= P_3 + P_4\\
  C_{22} &= P_5 + P_1 - P_3 - P_7\\
\end{align*}
$$

Steps 1 and 3 of the procedure both take $\Theta(n^2)$ time. In step 2, we compute seven multiplications of $n/2 \times n/2$ matrices. Thus we get the following recurrence relation:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 7T(n/2) + \Theta(n^2) \space}
$$

By case 1 of the master theorem (CLRS 4.5), we conclude that $T(n)$ $= \Theta(n^{\log{7}})$ $= \mathcal{O}(n^{2.8074})$, which is a significant improvement over the naive algorithm.

Implementation: [Matrix Multiplication - Strassen](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/strassen/mmp-2.c)
