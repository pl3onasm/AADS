/* 
  file: imp.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: application of the FFT algorithm to multiply 
    two natural numbers
  time complexity: O(n log n log log n)
*/

#include "fftlib/fft.h"
#include "natlib/nat.h"
#include "../../lib/clib.h"
#include <math.h>

//===================================================================
// Converts a natural number a = dₙ₋₁dₙ₋₂...d₁d₀ to an array of 
// complex numbers in reverse order so that its digits 
// {d₀, d₁, ..., dₙ₋₁} can be interpreted as the complex coefficients 
// of a polynomial of the form 
// P(x) = d₀ + d₁*x + d₂*x^2 + ... + dₙ₋₁*x^(n-1)
cdbl *natToComplex(Nat *n) {
  
  cdbl *cN = safeCalloc(n->size, sizeof(cdbl));
  for (size_t i = 0; i < n->size; ++i) 
    cN[i] = n->digits[n->size - i - 1] - '0';

  return cN;
}

//===================================================================
// Converts an array of complex coefficients of a polynomial
// P(x) = d₀ + d₁*x + d₂*x^2 + ... + dₙ₋₁*x^(n-1) to a natural number
// a = dₙ₋₁dₙ₋₂...d₁d₀ by evaluating the polynomial at x = 10
Nat *complexToNat(cdbl *coeffs, size_t len) {
  
  Nat *n = newNat(len);
  int carry = 0;
  for (size_t i = 0; i < len; ++i) {
    int coeff = round(creal(coeffs[i]));
    n->digits[len - i - 1] = (coeff + carry) % 10 + '0';
    carry = (coeff + carry) / 10;
    n->size++;
  }
  if (n->digits[0] == '0') {
    n->start++;
    n->size--;
  }
  return n;
}

//===================================================================
// Returns the product of two natural numbers x and y using the FFT
Nat *multiply(Nat *x, Nat *y) {

    // make complex double arrays of the numbers x and y
  cdbl *cX = natToComplex(x);
  cdbl *cY = natToComplex(y);

    // compute the convolution of the two arrays
  cdbl *conv = convolve(cX, cY, x->size, y->size);

    // convert the result back to a natural number
  Nat *prod = complexToNat(conv, x->size + y->size);

    // free memory
  free(cX); free(cY); free(conv);

  return prod;
}

//===================================================================

int main() {
  
  Nat *x = readNat();
  Nat *y = readNat();
  
  Nat *prod = multiply(x, y);
  
  showNat(prod);

  freeNat(x);
  freeNat(y);
  freeNat(prod);
  return 0;
}