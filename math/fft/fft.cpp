#include "complex.h"
#include <iostream>
#include <cmath>
#include <stdlib.h>

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
std::cout << "i" << i << ",j" << j << ": w" << w[((i*j)/stride) % 4] << " x" << x[j] << " y" << y[i] << std::endl;
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
std::cout << "start x (N = " << N << ") = ";
for (int ind = 0, count = 0; count < N; ind += stride, count++) {
   std::cout << x[ind] << ", ";
}
std::cout << std::endl;

std::cout << "FFT the evens (N = " << N << "): ";
for (int ind = 0; ind < N/2; ind++) {
   std::cout << x[2*stride*ind] << ", ";
}
std::cout << std::endl;

         // FFT the evens
         fftc2c (x, y, w, 2*stride, N/2);
std::cout << "FFT (evens) (N = " << N << ") = ";
for (int ind = 0; ind < N/2; ind++) {
   std::cout << y[2*stride*ind] << ", ";
}
std::cout << std::endl;

         // Copy the result back into x for the evens
         for (int ind = 0, count = 0; count < N/2; ind += 2*stride, count++)
            x[ind] = y[ind];

std::cout << "FFT the odds (N = " << N << "): ";
for (int ind = 0; ind < N/2; ind++) {
   std::cout << x[stride + 2*stride*ind] << ", ";
}
std::cout << std::endl;
         // FFT the odds
         fftc2c (x + stride, y + stride, w, 2*stride, N/2);
std::cout << "FFT (odds) (N = " << N << ") = ";
for (int ind = 0; ind < N/2; ind++) {
   std::cout << y[stride + 2*stride*ind] << ", ";
}
std::cout << std::endl;

//         // Copy the result back into x
//         for (int ind = 0, count = 0; count < N; ind += stride, count++)
//            x[ind] = y[ind];
         // Copy the result back into x for the odds
         for (int ind = stride, count = 0; count < N/2; ind += 2*stride, count++)
            x[ind] = y[ind];

std::cout << "after x (N = " << N << ") = ";
for (int ind = 0, count = 0; count < N; ind += stride, count++) {
   std::cout << x[ind] << ", ";
}
std::cout << std::endl;

         // Apply the twiddle factors to the odd terms
         for (int ind = stride, wind = 0, count = 0;
               count < N/2;
               ind += 2*stride, wind += stride, count++)
         {
            x[ind] *= w[wind];
         }

std::cout << "twiddle factors applied to x (N = " << N << ") = ";
for (int ind = 0, count = 0; count < N; ind += stride, count++) {
   std::cout << x[ind] << ", ";
}
std::cout << std::endl;

         for (int ind = 0, eind = 0, oind = stride, count = 0;
               count < N/2;
               ind += stride, eind += 2*stride, oind += 2*stride, count++)
         {
            y[ind             ] = x[eind] + x[oind];
            y[ind + N/2*stride] = x[eind] - x[oind];
std::cout << "first half of y at N = 4: " << y[ind] << std::endl;
std::cout << "last half of y at N = 4: " << y[ind + N/2] << std::endl;
         }

std::cout << "fft (x) (N = " << N << ") = ";
for (int ind = 0, count = 0; count < N; ind += stride, count++) {
   std::cout << y[ind] << ", ";
}
std::cout << std::endl;
      }
      else {
std::cout << "DFT of: " << x[0] << ", " << x[stride] << std::endl;
         // Compute the DFT
         dftc2c (x, y, w, stride, N);
std::cout << "DFT = " << y[0] << ", " << y[stride] << std::endl;
      }
   }
}

int main () {

   const int N = 32;
   complex<float>* w = new complex<float>[N];
   fft::twiddle (w, N);

   std::cout << "w = ";
   for (int ind = 0; ind < N; ind++) {
      std::cout << w[ind] << ", ";
   }
   std::cout << std::endl;


   complex<float>* x = new complex<float>[N];

   for (int ind = 0; ind < N; ind++) {
      x[ind] = complex<float> (
            (float)(rand () % 10000000) / 10000000,
            (float)(rand () % 10000000) / 10000000);

   }

   complex<float>* xdft = new complex<float>[N];
   for (int ind = 0; ind < N; ind++) xdft[ind] = x[ind];

   complex<float>* y    = new complex<float>[N];
   complex<float>* ydft = new complex<float>[N];

   std::cout << "x = ";
   for (int ind = 0; ind < N; ind++) {
      std::cout << x[ind] << ", ";
   }
   std::cout << std::endl;

   fft::fftc2c (x, y, w, 1, N);

   std::cout << "FFT (x) = ";
   for (int ind = 0; ind < N; ind++) {
      std::cout << y[ind] << ", ";
   }
   std::cout << std::endl;

   fft::dftc2c (xdft, ydft, w, 1, N);

   std::cout << "DFT (x) = ";
   for (int ind = 0; ind < N; ind++) {
      std::cout << ydft[ind] << ", ";
   }
   std::cout << std::endl;

   std::cout << "FFT (x) compares to DFT (x) = ";
   for (int ind = 0; ind < N; ind++) {
      complex<float> diff = y[ind] - ydft[ind];
      if (sqrtf (diff.real * diff.real + diff.imag * diff.imag) < 0.00001f) std::cout << ".";
      else                                                                  std::cout << "x";
   }
   std::cout << std::endl;

   return 0;
}
