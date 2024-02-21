/* 
  file: imp-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: standard long multiplication
  time complexity: O(n²), where n is the number 
    of digits in the input numbers
*/

#include "../../../lib/clib/clib.h"
#include "nat.h"

Nat *mulNat(Nat *a, Nat *b) {
  Nat *res = newNat(a->size + b->size);
  res->size = a->size + b->size;

  // multiply each digit of a with each digit of b
  // and add the result to the appropriate position in res
  // intermediate results are stored in res->digits as integers
  // and then converted back to char at the end
  // max int value is 9 * 9 = 81, so no overflow
  for (size_t i = a->size - 1; i < a->size; i--) {
    for (size_t j = b->size - 1; j < b->size; j--) {
      int s = (a->digits[i] - '0') * (b->digits[j] - '0') + res->digits[i + j + 1];
      res->digits[i + j + 1] = s % 10;
      res->digits[i + j] += s / 10;
    }
  }

  for (size_t i = 0; i < res->size; i++) 
    res->digits[i] += '0';    // convert back to char
  if (isZero(res)) res->size = 1;
  return res;
}

int main() {
  Nat *a = readNat();
  Nat *b = readNat();

  Nat *prod = mulNat(a, b);
  printNat(prod);
  
  freeNat(a);
  freeNat(b);
  freeNat(prod);
  return 0;
}