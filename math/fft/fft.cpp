#include "complex.h"
#include "fft.h"
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
            y[i] += w[((i*j)/stride) % (N*stride)] * x[j];
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
      if (N % 2 == 0 && N > 2)
      {

         // FFT the evens
         fftc2c (x, y, w, 2*stride, N/2);

         // FFT the odds
         fftc2c (x + stride, y + stride, w, 2*stride, N/2);

         // Copy the result back into x
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
               ind += stride, eind += 2*stride, oind += 2*stride, count++)
         {
            y[ind             ] = x[eind] + x[oind];
            y[ind + N/2*stride] = x[eind] - x[oind];
         }

      }
      else {
         // Compute the DFT
         dftc2c (x, y, w, stride, N);
      }
   }
}
