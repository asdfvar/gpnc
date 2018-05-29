#include "ann.h"
#include <cmath>

// function: sigmoid
// returns the sigmoid function:
//
//       x
//      e
//  ----------
//    x
//   e   +  1
//
static inline float sigmoid (float x)
{
   float val = exp (x);
   return val / (val + 1.0f);
}

namespace math {
   namespace ann {

      // constructor: Layer
      Layer::Layer (int layerType, int inputSize_in, int outputSize_in)
      {
         inputSize = inputSize_in;
         outputSize = outputSize_in;

         weights = new float [inputSize * outputSize];
      }

      // destructor: Layer
      Layer::~Layer (void)
      {
         delete[] weights;
      }

      // function: vectorMultiplyTranspose from Layer
      // multiplies a vector by the weights where the weights are
      // stored in their transposed form
      void Layer::vectorMultiplyTranspose (const float* input, float *output)
      {
         for (int i = 0; i < outputSize; i++)
         {
            output[i] = 0.0f;
            for (int j = 0; j < inputSize; j++)
            {
               output[i] += input[j] * weights[inputSize * i + j];
            }
         }
      }

      // function: operate from Layer
      // performs forward propagation on the inputs
      void Layer::operate (const float* input, float* output)
      {
         // the weights array is stored in its transposed form
         // so that matrix multiplication is done on contiguous memory
         // to better make use of SIMD instruction
         vectorMultiplyTranspose (input, output);

         // pass through the activation layer. in this case,
         // it is a sigmoid function
         for (int k = 0; k < outputSize; k++) output[k] = sigmoid (output[k]);
      }

   }
}
