/* file: magic.c
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

size_t gcd (ull a, ull b) {
  /* returns the greatest common divisor of a and b */
  if (b == 0) return a;
  return gcd(b, a % b);
}

ull lcm (ull a, ull b) {
  /* returns the least common multiple of a and b */
  return (a * b) / gcd(a, b);
}

ull nthMagicalNumber (size_t n, ull a, ull b) {
  /* returns the n-th magical number */
  if (a > b) 
    return nthMagicalNumber(n, b, a); // make sure a <= b
  
  size_t left = 0, right = n * a, mid;
  ull lcmab = lcm(a, b);

  while (left < right) {
    mid = left + (right - left) / 2;
    if (mid/a + mid/b - mid/lcmab < n) left = mid + 1;
    else right = mid;
  }
  return left;
}

int main () {
  size_t n;
  ull a, b;
  (void)! scanf("%lu %llu %llu", &n, &a, &b);
  printf("%llu\n", nthMagicalNumber(n, a, b));
  return 0;
}