/* 
  file: magic.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: 
    this an example of binary search, where we are looking for 
    the n-th magical number. A magical number is a number that
    is divisible by either a or b, where a and b are two given
    positive integers.
  time complexity: 
    O(log(n * min(a, b)))
*/

#include "../../../../lib/clib/clib.h"
typedef unsigned long long ull;

//===================================================================
// Returns the greatest common divisor of a and b
ull gcd (ull a, ull b) {
  if (b == 0) return a;
  return gcd(b, a % b);
}

//===================================================================
// Returns the least common multiple of a and b
ull lcm (ull a, ull b) {
  return (a * b) / gcd(a, b);
}

//===================================================================
// Returns the n-th magical number
ull nthMagicalNumber (size_t n, ull a, ull b) {
  
    // make sure a <= b
  if (a > b) return nthMagicalNumber(n, b, a); 
  
  size_t left = 0, right = n * a, mid;
  ull lcmab = lcm(a, b);

  while (left < right) {
    mid = left + (right - left) / 2;
    if (mid/a + mid/b - mid/lcmab < n) 
      left = mid + 1;
    else right = mid;
  }
  return left;
}

//===================================================================

int main () {
  size_t n;
  ull a, b;

  assert(scanf("%zu %llu %llu", &n, &a, &b) == 3);

  printf("%llu\n", nthMagicalNumber(n, a, b));
  return 0;
}