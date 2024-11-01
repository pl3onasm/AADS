/* 
  file: imp.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: application of the FFT algorithm to multiply 
    two natural numbers
  time complexity: O(n log n), where n is the number of digits 
    of the maximum of the two input numbers
*/

#include "fftlib/fft.h"
#include "natlib/nat.h"
#include "../../lib/clib.h"
#include <math.h>

//===================================================================
// Converts a natural number to an array of complex numbers in
// reverse order so that the number can be interpreted as the
// coefficients of a polynomial 
// P(x) = x[0] + x[1] * x + x[2] * x^2 + ...
cdbl *convertToComplex(Nat *n) {
  
  cdbl *cN = safeCalloc(n->size, sizeof(cdbl));
  for (size_t i = 0; i < n->size; ++i) 
    cN[i] = n->digits[n->size - i - 1] - '0';

  return cN;
}

//===================================================================
// Converts an array of complex coefficients to a natural number
// by evaluating the polynomial at x = 10
Nat *convertToNat(cdbl *coeffs, size_t len) {
  
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
  cdbl *cX = convertToComplex(x);
  cdbl *cY = convertToComplex(y);

    // compute the convolution of the two arrays
  cdbl *conv = convolve(cX, cY, x->size, y->size);

    // convert the result back to a natural number
  Nat *prod = convertToNat(conv, x->size + y->size);

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