#include "ann.h"

namespace math {
   namespace ann {

      ANN::ANN (int in_inputSize, int in_outputSize)
      {
         inputSize  = in_inputSize;
         outputSize = in_outputSize;

         outputs = new float[outputSize];
      }

      ANN::~ANN (void)
      {
         // deallocate the outputs
         delete[] outputs;

         // clear contents of the layers
         layers.clear();
      }

      void ANN::addLayer (int layerType, int layerInputSize, int layerOutputSize)
      {
         layers.push_back (Layer (layerType, layerInputSize, layerOutputSize));
      }

      void ANN::feedForward (float* inputs)
      {

      }

   }
}
