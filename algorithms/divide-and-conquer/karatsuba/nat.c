/* 
  file: nat.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: arbitrary precision arithmetic for natural numbers
*/

#include "../../../lib/clib/clib.h"
#include "nat.h"


Nat *newNat(size_t size) {
  // creates a new natural number with the given capacity
  Nat *n = safeMalloc(sizeof(Nat));
  n->digits = safeMalloc(size * sizeof(char));
  n->size = 0;  
  n->capacity = size;
  return n;
}

void freeNat(Nat *n) {
  // frees the memory allocated for n
  free(n->digits);
  free(n);
}

void checkCapacity(Nat *n, size_t cap) {
  // resizes if necessary
  if (n->capacity < cap) {
    n->capacity = 2 * cap;
    n->digits = safeRealloc(n->digits, n->capacity * sizeof(char));
  }
}

Nat *readNat() {
  // reads a natural number from stdin
  char ch = ' ';
  while (isspace(ch)) 
    ch = getchar();

  if (! isdigit(ch)) {  
    fprintf(stderr, "Invalid input\n");
    exit(1);
  }

  Nat *n = newNat(100);

  while (isdigit(ch)) {
    checkCapacity(n, n->size);
    n->digits[n->size++] = ch;
    ch = getchar();
  } 
  return n;
}

void printNat(Nat *n) {
  // prints the natural number n
  if (n->size == 0) {
    printf("0\n");
    return;
  }
  for (size_t i = 0; i < n->size; ++i) 
    printf("%c", n->digits[i]);
  printf("\n");
}

Nat *addNat(Nat *x, Nat *y) {
  // adds two natural numbers x and y
  size_t sumLen = MAX(x->size, y->size);
  Nat *sum = newNat(sumLen + 1);  // +1 for possible carry
  int digitSum = 0;
  for (size_t i = 0; i < sumLen; i++) {
    if (i < x->size) digitSum += x->digits[x->size - i - 1] - '0';
    if (i < y->size) digitSum += y->digits[y->size - i - 1] - '0';
    sum->digits[sumLen - i - 1] = digitSum % 10 + '0';
    digitSum /= 10;    // set carry
  }
  if (digitSum) {
    // move all digits one position to the right
    memmove(sum->digits + 1, sum->digits, sumLen + 1);
    sum->digits[0] = digitSum + '0';
    sumLen++;
  }  
  sum->size = sumLen;
  return sum;
}

Nat *subNat(Nat *x, Nat *y) {
  // subtracts two natural numbers x and y, where x >= y
  if (x->size < y->size) {
    fprintf(stderr, "Subtraction not supported for negative numbers\n");
    exit(1);
  }

  Nat *diff = newNat(x->size);
  diff->size = x->size;
  int digitDiff = 0;

  for (size_t i = 0; i < x->size; ++i) {
    digitDiff += x->digits[x->size - i - 1] - '0';
    if (i < y->size) digitDiff -= y->digits[y->size - i - 1] - '0';
    diff->digits[x->size - i - 1] = (digitDiff + 10) % 10 + '0';
    digitDiff = (digitDiff < 0) ? -1 : 0;   // set borrow
  }

  // remove any leading zeros
  int offset = 0;
  while (offset < diff->size && diff->digits[offset] == '0') 
    offset++;

  // move all digits offset positions to the left
  diff->size -= offset;
  memmove(diff->digits, diff->digits + offset, diff->size);

  return diff;
}

void splitNat(Nat *n, int exp, Nat *x, Nat *y) {
  // splits n into two parts, x and y, such that n = x * 10^exp + y
  size_t nSize = n->size;
  while (nSize) {
    if (exp-- > 0) 
      y->digits[y->size++] = n->digits[--nSize];
    else 
      x->digits[x->size++] = n->digits[--nSize];
  }

  x = reverseNat(x);
  y = reverseNat(y);
}

void mulByPow10 (Nat *n, int exp) {
  // multiplies n by 10^exp
  if (isZero(n)) 
    return;
  checkCapacity(n, n->size + exp);
  while (exp--) 
    n->digits[n->size++] = '0';
}

bool isZero(Nat *n) {
  // checks if n is zero
  return n->size == 0;
}

Nat *zero() {
  // returns the natural number 0
  return newNat(1);
}

Nat *reverseNat(Nat *n) {
  // reverses the digits of n
  for (size_t i = 0; i < n->size / 2; ++i) {
    char temp = n->digits[i];
    n->digits[i] = n->digits[n->size - i - 1];
    n->digits[n->size - i - 1] = temp;
  }
  return n;
}

Nat *intToNat(int x) {
  // converts an integer to a natural number
  if (x == 0) 
    return zero();

  Nat *n = newNat(10);  // 10 digits suffice for any int
  while (x) {
    n->digits[n->size++] = x % 10 + '0';
    x /= 10;
  }
  return reverseNat(n);
}
