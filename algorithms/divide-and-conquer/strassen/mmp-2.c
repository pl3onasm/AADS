/* file: mmp-2.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Strassen's matrix multiplication algorithm
                generalized to matrices of any size
   time complexity: O(n^2.8074)
*/

#include "../../../lib/clib/clib.h"

void strassen(int **A, int **B, int **C, size_t sz){
  /* computes C = A x B using Strassen's algorithm */

  // base case
  if (sz == 1) {
    C[0][0] = A[0][0] * B[0][0];
    return;
  }  
 
  size_t n = sz/2; 

  // allocate submatrices
  CREATE_3DMATRIX(int, Sub, 4, n, n);
  CREATE_3DMATRIX(int, S, 10, n, n);
  CREATE_3DMATRIX(int, P, 7, n, n);
  
  // compute S₁ to S₁₀ and submatrices A₁₁, A₂₂, B₁₁, B₂₂
  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j){
      S[0][i][j] = B[i][j+n] - B[i+n][j+n];   // S₁ = B₁₂ - B₂₂
      S[1][i][j] = A[i][j] + A[i][j+n];       // S₂ = A₁₁ + A₁₂
      S[2][i][j] = A[i+n][j] + A[i+n][j+n];   // S₃ = A₂₁ + A₂₂
      S[3][i][j] = B[i+n][j] - B[i][j];       // S₄ = B₂₁ - B₁₁
      S[4][i][j] = A[i][j] + A[i+n][j+n];     // S₅ = A₁₁ + A₂₂
      S[5][i][j] = B[i][j] + B[i+n][j+n];     // S₆ = B₁₁ + B₂₂
      S[6][i][j] = A[i][j+n] - A[i+n][j+n];   // S₇ = A₁₂ - A₂₂
      S[7][i][j] = B[i+n][j] + B[i+n][j+n];   // S₈ = B₂₁ + B₂₂
      S[8][i][j] = A[i][j] - A[i+n][j];       // S₉ = A₁₁ - A₂₁
      S[9][i][j] = B[i][j] + B[i][j+n];       // S₁₀ = B₁₁ + B₁₂
      Sub[0][i][j] = A[i][j];                 // A₁₁ 	      
      Sub[1][i][j] = B[i+n][j+n];             // B₂₂
      Sub[2][i][j] = A[i+n][j+n];             // A₂₂
      Sub[3][i][j] = B[i][j];                 // B₁₁
    }
  
  // recursively compute P₁ to P₇
  strassen(Sub[0], S[0], P[0], n);            // P₁ = A₁₁ x S₁
  strassen(S[1], Sub[1], P[1], n);            // P₂ = S₂ x B₂₂
  strassen(S[2], Sub[3], P[2], n);            // P₃ = S₃ x B₁₁
  strassen(Sub[2], S[3], P[3], n);            // P₄ = A₂₂ x S₄
  strassen(S[4], S[5], P[4], n);              // P₅ = S₅ x S₆
  strassen(S[6], S[7], P[5], n);              // P₆ = S₇ x S₈
  strassen(S[8], S[9], P[6], n);              // P₇ = S₉ x S₁₀
  
  // update the result matrix C
  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j){
      C[i][j] += P[4][i][j] + P[3][i][j]      // C₁₁ = P₅ + P₄ - P₂ + P₆
                - P[1][i][j] + P[5][i][j];  
      C[i][j+n] += P[0][i][j] + P[1][i][j];   // C₁₂ = P₁ + P₂
      C[i+n][j] += P[2][i][j] + P[3][i][j];   // C₂₁ = P₃ + P₄
      C[i+n][j+n] += P[4][i][j] + P[0][i][j]  // C₂₂ = P₅ + P₁ - P₃ - P₇
                    - P[2][i][j] - P[6][i][j];
    }

  // free memory
  FREE_3DMATRIX(Sub, 4, n);
  FREE_3DMATRIX(S, 10, n);
  FREE_3DMATRIX(P, 7, n);
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
    return 1;
  }

  size_t p = padSize(m, l);

  // allocate pxp matrices
  CREATE_MATRIX(int, A, p, p);
  CREATE_MATRIX(int, B, p, p);
  CREATE_MATRIX(int, C, p, p);

  // read matrices
  READ_MATRIX(A, "%d", m, n);
  READ_MATRIX(B, "%d", k, l);

  strassen(A, B, C, p);
  PRINT_MATRIX(C, "%d", m, l);

  FREE_MATRIX(A, p);
  FREE_MATRIX(B, p);
  FREE_MATRIX(C, p);

  return 0;
}


