# ${\color{Cadetblue}\text{Integer multiplication}}$

## ${\color{rosybrown}\text{Problem}}$

Given two numbers $x$ and $y$ of $m$ and $n$ digits, respectively, and where $m$ and $n$ may become arbitrarily large, we want to compute their product $z = x \cdot y$.

## ${\color{darkseagreen}\text{Long multiplication}}$

The most straightforward way to solve this problem is to use the elementary school algorithm for multiplication, also known as the long multiplication algorithm. This algorithm is based on the distributive property of multiplication over addition.

Writing the numbers in base 10, we have:

$\quad x \space = x_{m-1} \cdot 10^{m-1} + \ldots + x_1 \cdot 10 + x_0$  

$\qquad = \sum\limits_{i=0}^{m-1} x_i \cdot 10^i$  
  
$\quad y \space = y_{n-1} \cdot 10^{n-1} + \ldots + y_1 \cdot 10 + y_0$  

$\qquad = \sum\limits_{i=0}^{n-1} y_i \cdot 10^i$  

The product $z = x \cdot y$ is then given by:

$\quad z \space = x \cdot y$  

$\qquad = \left(\sum\limits_{i=0}^{m-1} x_i10^i\right) \cdot \left(\sum\limits_{i=0}^{n-1} y_i10^i\right)$  

$\qquad = \sum\limits_{i=0}^{m-1} \sum\limits_{j=0}^{n-1} x_i \cdot y_j \cdot 10^{i+j}$  

The product $z$ is thus obtained by multiplying each digit of $x$ by each digit of $y$, and then adding the results, taking into account the position of each digit.

In the worst case, we have $m = n$, so that the complexity of the long multiplication algorithm is in $\mathcal{O}{(n^2)}$. The implementation uses char arrays to represent the numbers, as a way to handle numbers of arbitrary size and not to waste memory as we would with integers (which typically take up 4 bytes each, whereas a char takes up only 1 byte).

Implementation: [Long multiplication](imp-1.c)

## ${\color{darkseagreen}\text{Karatsuba multiplication}}$

The Karatsuba algorithm is a divide-and-conquer algorithm and was the first multiplication algorithm to break the $\mathcal{O}{(n^2)}$ barrier. The idea is, just like in most divide-and-conquer algorithms, to reduce the original problem into subproblems of half the size, and then combine the results of the subproblems to obtain the final result. Splitting the digit strings into two halves, we have:

$\quad x = x_1 \cdot 10^{n/2} + x_0$  
$\quad y = y_1 \cdot 10^{n/2} + y_0$  

The product $z = x \cdot y$ is then given by:

$\quad z \space = x \cdot y$  
$\qquad = (x_1 \cdot 10^{n/2} + x_0) \cdot (y_1 \cdot 10^{n/2} + y_0)$  
$\qquad= x_1 \cdot y_1 \cdot 10^n + (x_1 \cdot y_0 + x_0 \cdot y_1) \cdot 10^{n/2}$  
$\qquad\quad+ \space x_0 \cdot y_0$  

Merely turning standard multiplication into a recursive problem like this, does not yield any improvement in the complexity of the algorithm. In fact, the complexity of the algorithm would be the same as the long multiplication algorithm, $\mathcal{O}{(n^2)}$. However, the key insight of Anatoly Karatsuba was to observe that we can compute the middle coefficient using only 1 multiplication instead of 2, after we have computed the other two coefficients. This is done by using the following identity:  

$\quad x_1 \cdot y_0 + x_0 \cdot y_1$ $= (x_1 + x_0) \cdot (y_1 + y_0)$  
$\qquad \qquad \qquad \qquad \quad - \space x_1 \cdot y_1 - x_0 \cdot y_0$

This way, we can compute, at each step, the product $z$ using only three multiplications instead of four, at the cost of a few additions and subtractions, which are much cheaper as they are linear operations.  
By continually dividing the numbers into halves, we eventually reach the base case, when the numbers have only one digit, in which case the product is simply the product of the two digits. As the recursion unwinds, the results of the subproblems are then combined to obtain the final result.

It is precisely the combination of dividing the problem into subproblems half the size and reducing the cost of each recursive step from four multiplications to three that makes the Karatsuba algorithm perform significantly better, much better than the long multiplication algorithm, especially for large numbers. Since at each step we divide the problem into three independent subproblems of the same type, each of half the size, and the cost of combining is linear, the recurrence relation is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 3T(n/2) + \Theta(n)\space}\\
$$

Using the master theorem, case 1, yields the solution $\Theta(n^{\log 3}) \approx \Theta(n^{1.585})$.

Note the similarity between the Karatsuba algorithm and [Strassen's matrix multiplication algorithm](../strassen): both algorithms succeed in reducing the cost of each recursive step by swapping one multiplication for a few additions and subractions, which are linear operations. Whereas Strassen's algorithm reduces the cost of each recursive step to seven-eighths of the original cost, Karatsuba's algorithm reduces the cost of each recursive step to three-quarters of the original cost.  

Just like before, the implementation uses char arrays to represent the numbers in order to keep the memory usage to a minimum.

Implementation: [Karatsuba multiplication](imp-2.c)
