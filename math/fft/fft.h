#include "complex.h"

namespace fft
{
   void twiddle (complex<float>* w, int N);

   void dftc2c (
         complex<float>* x,
         complex<float>* y,
         complex<float>* w,
         const int       stride,
         const int       N);

   void dftc2c (
         complex<float>* x,
         complex<float>* y,
         const int       N);

   void fftc2c (
         complex<float>* x,
         complex<float>* y,
         complex<float>* w,
         const int       stride,
         const int       N);

   void fftc2c (
         complex<float>* x,
         complex<float>* y,
         const int       N);
}
