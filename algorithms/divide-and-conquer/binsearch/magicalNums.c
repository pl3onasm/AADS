/* file: magicalNums.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: 
     this an example of binary search, where we are looking for 
     the n-th magical number. A magical number is a number that
     is divisible by either a or b, where a and b are two given
     co-prime integers.
   time complexity: 
     O(log(n * min(a, b)))
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned long long ull;

int gcd (int a, int b) {
  /* returns the greatest common divisor of a and b */
  if (b == 0) return a;
  return gcd(b, a % b);
}

int lcm (int a, int b) {
  /* returns the least common multiple of a and b */
  return (a * b) / gcd(a, b);
}

ull nthMagicalNumber (int n, int a, int b) {
  /* returns the n-th magical number */
  if (a > b) return nthMagicalNumber(n, b, a); // make sure a <= b
  ull l = 0, r = n * a, m;
  int lcmab = lcm(a, b);
  while (l < r) {
    m = l + (r - l) / 2;
    if (m/a + m/b - m/lcmab < n) l = m + 1;
    else r = m;
  }
  return l;
}

int main () {
  int n, a, b;
  scanf("%d %d %d", &n, &a, &b);
  printf("%llu\n", nthMagicalNumber(n, a, b));
  return 0;
}