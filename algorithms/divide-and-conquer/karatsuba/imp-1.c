/* 
  file: imp-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: standard long multiplication
  time complexity: O(n²), where n is the number 
    of digits of the maximum of the two input numbers
*/

#include "../../../lib/clib/clib.h"
#include "nat.h"

Nat *mulNat(Nat *x, Nat *y) {
  // computes a x b using standard long multiplication

  Nat *res = newNat(x->size + y->size);
  res->size = x->size + y->size;

  // multiplies each digit of x with each digit of y
  // results are first stored in res->digits as integers
  // and then converted back to char at the end
  for (size_t i = x->size - 1; i < x->size; i--) {
    for (size_t j = y->size - 1; j < y->size; j--) {
      int s = (x->digits[i] - '0') * (y->digits[j] - '0') 
              + res->digits[i + j + 1];
      res->digits[i + j + 1] = s % 10;
      res->digits[i + j] += s / 10;
    }
  }

  // remove leading zeros
  int offset = 0;
  while (offset < res->size && res->digits[offset] == 0) 
    offset++;

  res->size -= offset;
  memmove(res->digits, res->digits + offset, res->size);

  // convert back to char
  for (size_t i = 0; i < res->size; i++) 
    res->digits[i] += '0';    
  
  return res;
}

int main() {
  Nat *x = readNat();
  Nat *y = readNat();

  Nat *prod = mulNat(x, y);
  printNat(prod);
  
  freeNat(x);
  freeNat(y);
  freeNat(prod);
  return 0;
}