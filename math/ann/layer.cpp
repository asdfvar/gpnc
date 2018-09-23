#include "ann.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

// function: sigmoid
// returns the sigmoid function:
//
//               1
// g(x;B) =  ----------
//             -Bx
//            e   +  1
//
// then it follows that g'(x;B) = B * (1 - g(x;B)) * g(x;B)
//
static inline float sigmoid (float x, float B)
{
   float val = exp (-B*x);
   return 1.0f / (val + 1.0f);
}

static inline float sigmoid_prime (float x, float B)
{
   float sig = sigmoid (x, B);
   return B * (1.0f - sig) * sig;
}

// Compute the result of multiplication of two matrices
// C = A * B (dimension ai x bj)
static inline bool matrix_mult (const float* A,
                                int          ai,
                                int          aj,
                                const float* B,
                                int          bi,
                                int          bj,
                                float*       C)
{

   // ai must equal bj
   if (aj != bi) {
      std::cout << "A * B error, matrix dimensions don't agree" << std::endl;
      return false;
   }

   const int m = ai = bj;
   float temp;

   for (int i = 0; i < ai; i++) {
      for (int j = 0; j < bj; j++) {
         temp = 0.0f;
         for (int k = 0; k < m; k++) {
            // temp += A[i,k] * B[k,j];
            temp += A[i * aj + k] * B[k * bj + j];
         }
         // C[i,j] = temp;
         C[i * bj + j] = temp;
      }
   }

   return true;
}

// Compute the result of the transpose of two matrices
// C = A' * B' (dimension aj x bi)
static inline bool transpose_transpose_matrix_mult (const float* A,
                                                    int          ai,
                                                    int          aj,
                                                    const float* B,
                                                    int          bi,
                                                    int          bj,
                                                    float*       C)
{

   // ai must equal bj
   if (ai != bj) {
      std::cout << "A' * B' error, matrix dimensions don't agree" << std::endl;
      return false;
   }

   const int m = ai = bj;
   float temp;

   for (int i = 0; i < aj; i++) {
      for (int j = 0; j < bi; j++) {
         temp = 0.0f;
         for (int k = 0; k < m; k++) {
            // temp += A[k,i] * B[j,k];
            temp += A[k * aj + i] * B[j * bj + k];
         }
         // C[i,j] = temp;
         C[i * bi + j] = temp;
      }
   }

   return true;
}

// Compute the results of a vector by the transpose of another vector
// C = A * B'
static inline bool vector_transpose_mult (const float* A, int an,
                                          const float* B, int bn,
                                          float* C)
{
   for (int i = 0; i < an; i++) {
      for (int j = 0; j < bn; j++) {
         // C[i,j] = A[i] * B[j];
         C[i * bn + j] = A[i] * B[j];
      }
   }

   return true;
}

// TODO: compute A' * B where A' is (a, a, a, ...)'

namespace math {
   namespace ann {

      // constructor: Layer
      Layer::Layer (int layerType, int inputSize_in, int outputSize_in)
      {
         inputSize = inputSize_in;
         outputSize = outputSize_in;

         weights = new float [inputSize * outputSize];

         // randomize the weights to values within [0, 1)
         for (int k = 0; k < inputSize * outputSize; k++)
         {
            weights[k] = static_cast<float> (rand() % 1000) / 1000.0f;
         }
      }

      // destructor: Layer
      Layer::~Layer (void)
      {
         delete[] weights;
      }

      // function: operate from Layer
      // performs forward propagation on the inputs
      void Layer::operate (const float* input, float* output)
      {
         bool stat = true;

         // output = weights * input
         stat = matrix_mult (weights, outputSize, inputSize,
                             input,   inputSize,  1,
                             output);

         // pass through the activation layer. In this case,
         // it is a sigmoid function
         for (int k = 0; k < outputSize; k++) output[k] = sigmoid (output[k], 1.0f);
      }

      // function: getOutputSize from Layer
      int Layer::getOutputSize (void)
      {
         return outputSize;
      }

   }
}
