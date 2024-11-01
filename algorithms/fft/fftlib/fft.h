#ifndef FFT_H_INCLUDED
#define FFT_H_INCLUDED

#include <stdlib.h>
#include <complex.h>
typedef complex double cdbl;


  // returns the fast fourier transform
  // of an array of complex numbers
cdbl *fft(cdbl *arr, size_t arrLen);

  // returns the inverse fast fourier transform
  // of an array of complex numbers
cdbl *ifft(cdbl *arr, size_t arrLen);

  // returns the convolution of two input arrays
  // representing complex coefficients of polynomials
cdbl *convolve(cdbl *x, cdbl *y, size_t xLen, size_t yLen);


#endif // FFT_H_INCLUDED
