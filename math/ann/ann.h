#ifndef ANN_H
#define ANN_H

#include <list>

namespace math {
   namespace ann {

      enum layerType {SIGMOID};


      // class: Layer
      class Layer
      {
         public:
            Layer (int layerType, int inputSize, int outputSize);
            ~Layer (void);

            // perform forward propagation on the inputs
            void operate (const float* input, float* output);

            int getOutputSize (void);

         private:
            float* weights;
            int    inputSize;
            int    outputSize;

            void vectorMultiplyTranspose (const float* inputs, float *outputs);
      };


      // class: ANN
      class ANN
      {
         public:
            ANN (int inputSize, int outputSize);
            ~ANN (void);

            void addLayer (int layerType, int layerInputSize, int layerOutputSize);

            void feedForward (float* inputs);

         private:
            std::list<Layer> layers;
            float* outputs;

            int inputSize;
            int outputSize;
            int numLayers;
            int maxLayerSize;
      };

   }
}

#endif
