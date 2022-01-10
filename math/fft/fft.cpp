#include "complex.h"
#include <iostream>
#include <cmath>

#define PI 3.1415926535f

namespace fft {
   void twiddle (complex<float>* w, int N) {
      for (int ind = 0; ind < N; ind++) {
         w[ind].real = cosf (-2.0f * PI * (float)ind/ (float)N);
         w[ind].imag = sinf (-2.0f * PI * (float)ind/ (float)N);
      }
   }

   void dftc2c (
         complex<float>* x,
         complex<float>* y,
         complex<float>* w,
         const int       stride,
         const int       N)
   {
      for (int i = 0, icount = 0; icount < N; i += stride, icount++) {
         y[i] = complex<float> (0.0f, 0.0f);
         for (int j = 0, jcount = 0; jcount < N; j += stride, jcount++) {
            y[i] += w[i*j % N] * x[j];
         }
      }
   }

   void fftc2c (
         complex<float>* x,
         complex<float>* y,
         complex<float>* w,
         const int       stride,
         const int       N)
   {
      if (N % 2 == 0) {
         // FFT the evens
         fftc2c (x, y, w, 2*stride, N/2);

         // FFT the odds
         fftc2c (x + stride, y + stride, w, 2*stride, N/2);

         for (int ind = 0, count = 0; count < N; ind += stride, count++)
            x[ind] = y[ind];

         // Apply the twiddle factors to the odd terms
         for (int ind = stride, wind = 0, count = 0;
               count < N/2;
               ind += 2*stride, wind += stride, count++)
         {
            x[ind] *= w[wind];
         }

         for (int ind = 0, eind = 0, oind = stride, count = 0;
               count < N/2;
               ind += stride, eind += stride, oind += stride, count++)
         {
            y[ind      ] = x[eind] + x[oind];
            y[ind + N/2] = x[eind] - x[oind];
         }
      }
      else {
         // Compute the DFT
         dftc2c (x, y, w, stride, N);
      }
   }
}

int main () {

   const int N = 4;
   complex<float>* w = new complex<float>[N];
   fft::twiddle (w, N);

   complex<float>* x = new complex<float>[N];
   x[0] = complex<float> (0.5488135, 0.71518937);
   x[1] = complex<float> (0.60276338, 0.54488318);
   x[2] = complex<float> (0.4236548, 0.64589411);
   x[3] = complex<float> (0.43758721, 0.891773);
   //x[4] = complex<float> (0.96366276, 0.38344152);
   //x[5] = complex<float> (0.79172504, 0.52889492);

   complex<float>* y = new complex<float>[N];

   std::cout << "x = ";
   for (int ind = 0; ind < N; ind++) {
      std::cout << x[ind] << ", ";
   }
   std::cout << std::endl;

   fft::fftc2c (x, y, w, 1, N);

   std::cout << "y = ";
   for (int ind = 0; ind < N; ind++) {
      std::cout << y[ind] << ", ";
   }
   std::cout << std::endl;

   return 0;
}
