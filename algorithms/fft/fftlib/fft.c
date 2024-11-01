/* 
  file: ftt.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: FFT implementation for integer inputs
*/

#include "../../../lib/clib.h"
#include <math.h>
#include <ctype.h>
#include "fft.h"

const double PI = 3.141592653589793238462643383279502884;

//===================================================================
// Returns the smallest power of 2 greater than n
static size_t getPow2(size_t n) {
  size_t pow2 = 1;
  while (pow2 < n) pow2 <<= 1;
  return pow2;
}

//===================================================================
// Returns the fast fourier transform of an array of complex numbers
// given the length of the array and the nth root of unity
static cdbl *_fft(cdbl *arr, size_t len, cdbl ωn) {
  
  if (len == 1) 
    return arr;

  cdbl ω = 1;
  cdbl *evenXs = safeCalloc(len/2, sizeof(cdbl)); 
  cdbl *oddXs = safeCalloc(len/2, sizeof(cdbl));

  for (size_t i = 0; i < len/2; ++i) {
    evenXs[i] = arr[2 * i];
    oddXs[i] = arr[2 * i + 1];
  }
  
  cdbl *evenYs = _fft(evenXs, len/2, ωn * ωn);
  cdbl *oddYs = _fft(oddXs, len/2, ωn * ωn);

  cdbl *y = safeCalloc(len, sizeof(cdbl));
  for (size_t i = 0; i < len/2; ++i) {
    y[i] = evenYs[i] + ω * oddYs[i];
    y[i + len/2] = evenYs[i] - ω * oddYs[i];
    ω *= ωn;
  }

  free(evenXs); free(oddXs); 
  if (len > 2) {
    free(evenYs); 
    free(oddYs);
  }
  return y;
}

//===================================================================
// Returns the (forward) fast fourier transform of an input array of 
// complex numbers
cdbl *fft(cdbl *arr, size_t arrLen) {
  
  cdbl ω = cexp(2 * PI * I / arrLen);
  cdbl *res = _fft(arr, arrLen, ω);
  return res;
}

//===================================================================
// Returns the inverse (backward) fast fourier transform of an input
// array of complex numbers 
cdbl *ifft(cdbl *arr, size_t arrLen) {
  
  cdbl ω = cexp(-2 * PI * I / arrLen);
  cdbl *inv = _fft(arr, arrLen, ω);
  for (size_t i = 0; i < arrLen; ++i) 
    inv[i] /= arrLen;
  return inv;
}

//===================================================================
// Returns the convolution of two input arrays representing complex
// coefficients of polynomials 
cdbl *convolve(cdbl *x, cdbl *y, size_t xLen, size_t yLen) {
  
  size_t len = getPow2(MAX(xLen, yLen) * 2);

  cdbl *x2 = safeCalloc(len, sizeof(cdbl));
  cdbl *y2 = safeCalloc(len, sizeof(cdbl));

    // copy x and y into x2 and y2 
  for (size_t i = 0; i < xLen; ++i) 
    x2[i] = x[i];

  for (size_t i = 0; i < yLen; ++i)
    y2[i] = y[i];

    // compute FFT of x2 and y2
  cdbl *fftX = fft(x2, len);
  cdbl *fftY = fft(y2, len);

    // compute product of fftX and fftY
  cdbl *fftXY = safeCalloc(len, sizeof(cdbl));
  for (size_t i = 0; i < len; ++i) 
    fftXY[i] = fftX[i] * fftY[i];

    // compute inverse FFT of fftXY
  cdbl *conv = ifft(fftXY, len);

  free(x2); free(y2);
  free(fftX); free(fftY); free(fftXY);

  return conv;
}