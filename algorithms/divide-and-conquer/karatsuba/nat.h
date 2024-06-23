#ifndef NAT_H_INCLUDED
#define NAT_H_INCLUDED

#include <stddef.h>
#include <ctype.h>

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

  // Returns the sum of two natural numbers a and b
Nat *addNat(Nat *a, Nat *b);

  // Returns the difference of two natural numbers a and b
Nat *subNat(Nat *a, Nat *b);
void mulByPow10(Nat *n, int exp);
void splitNat(Nat *n, int exp, Nat *a, Nat *b);
bool isZero(Nat *n);
Nat *zero();
Nat *intToNat(int x);
Nat *reverseNat(Nat *n);

#endif // NAT_H_INCLUDED
