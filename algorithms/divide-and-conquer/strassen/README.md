# ${\color{Cadetblue}\text{Matrix Multiplication Problem (MMP)}}$

## ${\color{rosybrown}\text{Description}}$

Strassen's matrix multiplication is a divide and conquer algorithm for multiplying square matrices. It is faster than the naive matrix multiplication algorithm, which is in $O(n^3)$. Strassen's algorithm is in $\Theta(n^{\log{7}}) = O(n^{2.8074}) $.

## ${\color{rosybrown}\text{Standard Matrix Multiplication}}$

If we want to multiply two $n \times n$ matrices $A$ and $B$, we can do so by taking the dot product of each row of $A$ with each column of $B$. This is the naive approach, which is shown in the following pseudocode:

```c
  matrixMultiply(A, B, C, n)
    for i = 1 to n
        for j = 1 to n
          Cᵢⱼ = 0
          for k = 1 to n
            Cᵢⱼ += Aᵢⱼ * Bᵢⱼ
    return C
```

In the above pseudocode, $Cᵢⱼ$ is the element in the $i$ th row and $j$ th column of $C$. Since we have three nested loops iterating over $n$ elements, the time complexity of this algorithm is in $O(n^3)$.

We can turn this algorithm into a divide and conquer algorithm by splitting the matrices $A$ and $B$ into four $n/2 \times n/2$ block-matrices and then multiplying them recursively. Doing so, we make use of the fact that the standard rule for matrix multiplication is also true for block-matrices:

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

Repeatedly dividing the matrices in submatrices of size $n/2 \times n/2$ in order to apply a divide and conquer algorithm presumes that $n$ is a power of two. Only this way can we keep halving the dimensions until we reach the base case, where we have matrices of size $1 \times 1$, which is just scalar multiplication. If $n$ is not a power of two, we can simply pad the original matrices with zeros until we get a matrix of size $2^k \times 2^k$, where $k$ is the smallest integer such that $2^k \geq n$. Padding makes it even possible to generalize the algorithm to input matrices of arbitrary dimensions, while the core algorithm works for matrices of size $2^k \times 2^k$. And it won't affect the time complexity either, since the padding only adds a constant number of operations to the first recursive step of the algorithm.

Looking at the above formula, we can see that we need to compute 8 products of matrices of size $n/2 \times n/2$ at each dividing step. If we use index calculations to partition the matrices at each step, the partitioning itself only takes $\Theta(1)$ time. All this leads to the following recurrence relation:

$$
T(n) = 8T(n/2) + \Theta(1)
$$

By case 1 of the master theorem (CLRS 4.5), we can conclude that $T(n) = \Theta(n^3)$, which is the same time complexity as the naive algorithm.

Implementation: [Matrix Multiplication - D&C](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/strassen/mmp-1.c)

## ${\color{rosybrown}\text{Strassen's Matrix Multiplication}}$

The idea behind Strassen's algorithm is to reduce the number of multiplications performed at each dividing step from 8 to 7. This will mean that we will have to compute more matrix sums and subtractions, but since these operations are much faster than matrix multiplication, we will still end up with a faster algorithm.

At each recursive step, we first compute the following 10 sums of matrices of size $n/2 \times n/2$:

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

Finally, we update the following 4 submatrices of the final matrix $C$:

$$
\begin{align*}
  C_{11} &= P_5 + P_4 - P_2 + P_6\\
  C_{12} &= P_1 + P_2\\
  C_{21} &= P_3 + P_4\\
  C_{22} &= P_5 + P_1 - P_3 - P_7\\
\end{align*}
$$

Steps 1 and 3 both take $\Theta(n^2)$ time. In step 2, we compute seven multiplications of $n/2 \times n/2$ matrices. Thus we get the recurrence relation:

$$
T(n) = 7T(n/2) + \Theta(n^2)
$$

By case 1 of the master theorem (CLRS 4.5), we can conclude that $T(n) = \Theta(n^{\log{7}}) = O(n^{2.8074})$.  

Implementation: [Matrix Multiplication - Strassen](https://github.com/pl3onasm/AADS/blob/main/algorithms/divide-and-conquer/strassen/mmp-2.c)
