#include "ann.h"

namespace math {
   namespace ann {

      ANN::ANN (int in_inputSize, int in_outputSize)
      {
         inputSize  = in_inputSize;
         outputSize = in_outputSize;

         outputs = new float[outputSize];

         numLayers = 0;
      }

      ANN::~ANN (void)
      {
         delete[] outputs;

         for (int layer = 0; layer < numLayers; layer++)
         {
            delete layerType;
         }
      }

      void ANN::addLayer (enum layerType, int layerSize)
      {
         layers[numLayers++] = new Layer (layerType, layerSize);
      }

      void ANN::feedForward (float* inputs)
      {

      }

   }
}
