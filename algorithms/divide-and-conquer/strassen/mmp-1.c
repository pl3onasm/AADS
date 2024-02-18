/* file: mmp-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: standard matrix multiplication for input
    matrices of arbitrary size, using a divide-and-conquer approach
   time complexity: O(n³), where n is the padding size
*/

#include "../../../lib/clib/clib.h"

void mul(int **A, int **B, int **C, size_t n, 
          size_t i, size_t k, size_t j) {
  /* recursively computes the product of n x n matrices A and B
     and stores the result in n x n matrix C */

  // base case
  if (n == 1) {
    C[i][j] += A[i][k] * B[k][j];
    return;
  }
  // partition A, B and C into 4 submatrices using indices
  // and recursively multiply the submatrices
  size_t n2 = n / 2;
  size_t i2 = i + n2;
  size_t j2 = j + n2;
  size_t k2 = k + n2;

  // C₁₁ = A₁₁ * B₁₁ + A₁₂ * B₂₁
  mul(A, B, C, n2, i, k, j);
  mul(A, B, C, n2, i, k2, j);
  // C₁₂ = A₁₁ * B₁₂ + A₁₂ * B₂₂
  mul(A, B, C, n2, i, k, j2);
  mul(A, B, C, n2, i, k2, j2);
  // C₂₁ = A₂₁ * B₁₁ + A₂₂ * B₂₁
  mul(A, B, C, n2, i2, k, j);
  mul(A, B, C, n2, i2, k2, j);
  // C₂₂ = A₂₁ * B₁₂ + A₂₂ * B₂₂
  mul(A, B, C, n2, i2, k, j2);
  mul(A, B, C, n2, i2, k2, j2);
}

size_t padSize(size_t m, size_t l) {
  /* computes p = 2^k, where p is the smallest power of 2
     greater than or equal to max(m, l) */
  m = MAX(m, l);
  size_t p = 1;
  while (p < m) p <<= 1;
  return p;
}

int main(int argc, char *argv[]) {
  size_t m, n, k, l;   // matrix dimensions

  (void)! scanf("%lu %lu", &m, &n);
  (void)! scanf("%lu %lu", &k, &l);

  if (n != k) { 
    printf("Incompatible matrices.\n");
    return 0;
  }

  size_t p = padSize(m, l);

  // allocate pxp matrices
  CREATE_MATRIX(int, A, p, p);
  CREATE_MATRIX(int, B, p, p);
  CREATE_MATRIX(int, C, p, p);

  // read matrices
  READ_MATRIX(A, "%d", m, n);
  READ_MATRIX(B, "%d", k, l);

  mul(A, B, C, p, 0, 0, 0);
  PRINT_MATRIX(C, "%d", m, l);

  FREE_MATRIX(A, p);
  FREE_MATRIX(B, p);
  FREE_MATRIX(C, p);

  return 0; 
}
  
