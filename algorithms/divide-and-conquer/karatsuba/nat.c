/* 
  file: nat.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: arbitrary precision arithmetic for natural numbers
*/

#include "../../../lib/clib/clib.h"
#include "nat.h"
#include <ctype.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

//===================================================================
// Creates a new natural number with given capacity
Nat *newNat(size_t capacity) {
  Nat *n = safeCalloc(1, sizeof(Nat));
  n->digits = safeCalloc(capacity, sizeof(char));
  n->size = 0;  
  n->capacity = capacity;
  return n;
}

//===================================================================
// Deallocates the memory used by a natural number
void freeNat(Nat *n) {
  free(n->digits);
  free(n);
}

//===================================================================
// Checks n's capacity and resizes if necessary
void checkCapacity(Nat *n, size_t capacity) {
  if (n->capacity <= capacity) {
    n->capacity = 2 * capacity;
    n->digits = safeRealloc(n->digits, n->capacity * sizeof(char));
    memset(n->digits + n->size, 0, n->capacity - n->size);
  }
}

//===================================================================
// Reads a natural number from stdin
Nat *readNat() {
  char ch = ' ';
  while (isspace(ch)) 
    ch = getchar();

  if (! isdigit(ch)) {  
    fprintf(stderr, "Invalid input\n");
    exit(EXIT_FAILURE);
  }

  Nat *n = newNat(100);

  while (isdigit(ch)) {
    checkCapacity(n, n->size);
    n->digits[n->size++] = ch;
    ch = getchar();
  } 
  return n;
}

//===================================================================
// Shows a natural number
void showNat(Nat *n) {
  if (n->size == 0) {
    printf("0\n");
    return;
  }
  for (size_t i = 0; i < n->size; ++i) 
    printf("%c", n->digits[i]);
  printf("\n");
}

//===================================================================
// Returns the sum of two natural numbers x and y
Nat *addNat(Nat *x, Nat *y) {
  size_t sumLen = MAX(x->size, y->size);
  Nat *sum = newNat(sumLen + 2); 
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

//===================================================================
// Returns the difference x - y of two natural numbers x and y
// Requires x >= y
Nat *subNat(Nat *x, Nat *y) {
  if (x->size < y->size) {
    fprintf(stderr, "Subtraction not supported " 
                    "for negative numbers\n");
    exit(EXIT_FAILURE);
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

    // compute offset of first non-zero digit
  int offset = 0;
  while (offset < diff->size && diff->digits[offset] == '0') 
    offset++;

    // remove any leading zeros
  diff->size -= offset;
  memmove(diff->digits, diff->digits + offset, diff->size);

  return diff;
}

//===================================================================
// Splits n into two parts, x and y, such that n = x * 10^exp + y
void splitNat(Nat *n, size_t exp, Nat **x, Nat **y) {
  
  *x = newNat(n->size);
  *y = newNat(n->size);
  size_t nSize = n->size;

  while (nSize && exp--) 
    (*y)->digits[(*y)->size++] = n->digits[--nSize];
  while (nSize)
    (*x)->digits[(*x)->size++] = n->digits[--nSize];

  *x = reverseNat(*x);
  *y = reverseNat(*y);
}

//===================================================================
// Multiplies n by 10^exp
void mulByPow10 (Nat *n, size_t exp) {
  if (isZero(n)) 
    return;
  checkCapacity(n, n->size + exp);
  while (exp--) 
    n->digits[n->size++] = '0';
}

//===================================================================
// Checks if n is zero
bool isZero(Nat *n) {
  return n->size == 0;
}

//===================================================================
// Returns the natural number 0
Nat *zero() {
  return newNat(1);
}

//===================================================================
// Reverse the digits of n in place
Nat *reverseNat(Nat *n) {
  for (size_t i = 0; i < n->size / 2; ++i) {
    char temp = n->digits[i];
    n->digits[i] = n->digits[n->size - i - 1];
    n->digits[n->size - i - 1] = temp;
  }
  return n;
}

//===================================================================
// Converts an integer to a natural number
Nat *intToNat(int x) {
  if (x == 0) 
    return zero();

    // allocate memory for the number: 
    // 10 digits suffice for any int
  Nat *n = newNat(10);  

  while (x) {
    n->digits[n->size++] = x % 10 + '0';
    x /= 10;
  }
  return reverseNat(n);
}
