#include "ann.h"

namespace math {
   namespace ann {

      ANN::ANN (int in_inputSize, int in_outputSize)
      {
         inputSize  = in_inputSize;
         outputSize = in_outputSize;

         maxLayerSize = inputSize > outputSize ? inputSize : outputSize;

         outputs = new float[outputSize];
      }

      ANN::~ANN (void)
      {
         // deallocate the outputs
         delete[] outputs;

         // clear contents of the layers
         layers.clear();
      }

      // function: addLayer from ANN
      // add a layer to the end of the network
      void ANN::addLayer (int layerType, int layerInputSize, int layerOutputSize)
      {
         layers.push_back (Layer (layerType, layerInputSize, layerOutputSize));

         if (maxLayerSize < layerInputSize) maxLayerSize = layerInputSize;
         if (maxLayerSize < layerOutputSize) maxLayerSize = layerOutputSize;
      }

      // function: feedForward from ANN
      // perform forward propagation on the neural network
      void ANN::feedForward (float* inputs)
      {
         float* inbuffer = new float[maxLayerSize];
         float* outbuffer = new float[maxLayerSize];

         for (int k = 0; k < inputSize; k++) inbuffer[k] = inputs[k];

         for (std::list<Layer>::iterator layer = layers.begin();
               layer != layers.end();
               layer++)
         {
            layer->operate (inbuffer, outbuffer);
            for (int k = 0; k < layer->getOutputSize(); k++) inbuffer[k] = outbuffer[k];
         }

         for (int k = 0; k < outputSize; k++) outputs[k] = outbuffer[k];

         delete[] inbuffer;
         delete[] outbuffer;
      }

   }
}
