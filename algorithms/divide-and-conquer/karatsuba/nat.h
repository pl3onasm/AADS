#ifndef NAT_H_INCLUDED
#define NAT_H_INCLUDED

#include <stddef.h>
#include <ctype.h>

typedef struct {
  char *digits;
  size_t size, capacity;
} Nat;

Nat *newNat(size_t capacity);
void freeNat(Nat *n);
void printNat(Nat *n);
Nat *readNat();
Nat *addNat(Nat *a, Nat *b);
Nat *subNat(Nat *a, Nat *b);
void mulByPow10(Nat *n, int exp);
void splitNat(Nat *n, int exp, Nat *a, Nat *b);
bool isZero(Nat *n);
Nat *zero();
Nat *intToNat(int x);
Nat *reverseNat(Nat *n);

#endif // NAT_H_INCLUDED
