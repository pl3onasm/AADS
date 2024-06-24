#ifndef NAT_H_INCLUDED
#define NAT_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  char *digits;
  size_t size, capacity;
} Nat;

  // Creates a new natural number with given capacity
Nat *newNat(size_t capacity);
  
  // Deallocates the memory used by a natural number
void freeNat(Nat *n);

  // Shows a natural number on stdout
void showNat(Nat *n);

  // Reads a natural number from stdin
Nat *readNat();

  // Returns the sum of two natural numbers x and y
Nat *addNat(Nat *x, Nat *y);

  // Returns the difference of two natural numbers x and y
  // Precondition: x >= y
Nat *subNat(Nat *x, Nat *y);
  
  // Multiplies a natural number x by 10^exp
void mulByPow10(Nat *n, size_t exp);

  // Split a natural number n into two parts x and y
  // such that n = x * 10^exp + y
void splitNat(Nat *n, size_t exp, Nat **x, Nat **y);

  // Returns true if n is zero
bool isZero(Nat *n);

  // Returns the natural number zero
Nat *zero();

  // Converts an integer to a natural number
Nat *intToNat(int x);

#endif // NAT_H_INCLUDED
