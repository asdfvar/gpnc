#include "fft.h"
#include "complex.h"
#include <iostream>
#include <stdlib.h>
#include <cmath>

int main () {

   const int N = 2*2*2*3;
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
