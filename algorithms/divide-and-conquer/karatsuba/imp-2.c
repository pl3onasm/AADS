/* 
  file: imp-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: Karatsuba multiplication for natural numbers
  time complexity: O(n^1.585), where n is the number 
    of digits of the maximum of the two input numbers
*/

#include "../../../lib/clib/clib.h"
#include "nat.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

//===================================================================
// Computes the product x * y using Karatsuba's algorithm
Nat *karatsuba(Nat *x, Nat *y) {
  
  if (isZero(x) || isZero(y)) 
    return zero();

  if (x->size == 1 && y->size == 1)
    return intToNat((x->digits[0] - '0') * (y->digits[0] - '0'));
 
  size_t exp = MAX(x->size, y->size) / 2;
  Nat *s1, *s2, *s3, *s4;
  
    // split x and y into two parts:
    // x = x1 * 10^exp + x0
    // y = y1 * 10^exp + y0
  Nat *x1 = newNat(x->size), *x0 = newNat(x->size);
  Nat *y1 = newNat(y->size), *y0 = newNat(y->size);
  
  splitNat(x, exp, x1, x0);
  splitNat(y, exp, y1, y0);
  
    // compute first component z1 = x1 * y1 
  Nat *z1 = karatsuba(x1, y1);    

    // compute third component z3 = x0 * y0 
  Nat *z3 = karatsuba(x0, y0);    

    // compute second component 
    // z2 = (x1 + x0) * (y1 + y0) - z1 - z3
  Nat *p = karatsuba((s1 = addNat(x1, x0)), (s2 = addNat(y1, y0)));
  Nat *z2 = subNat((s3 = subNat(p, z1)), z3);

    // combine the three components to get the final result
    // z = [z1 * 10^(2*exp)] + [z2 * 10^exp] + z3
  mulByPow10(z1, 2 * exp);
  mulByPow10(z2, exp);
  Nat *z = addNat((s4 = addNat(z1, z2)), z3);

    // free all intermediate results
  freeNat(x0); freeNat(x1); freeNat(y0);
  freeNat(y1); freeNat(z1); freeNat(z2);
  freeNat(z3); freeNat(s1); freeNat(s2);
  freeNat(s3); freeNat(s4); freeNat(p);
  
  return z;
}

//===================================================================

int main() {

  Nat *x = readNat();
  Nat *y = readNat();

  Nat *prod = karatsuba(x, y);
  showNat(prod);
  
  freeNat(x);
  freeNat(y);
  freeNat(prod);
  return 0;
}