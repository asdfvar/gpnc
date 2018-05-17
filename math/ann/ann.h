#ifndef ANN_H
#define ANN_H

namespace math {
   namespace ann {

      enum layerType {TRADITIONAL, SIGMOID};

      class Layer
      {
         public:
            Layer (enum layerType, int inputSize);
            ~Layer (void);

         private:
            float* weights;
      };

      class ANN
      {
         public:
            ANN (int inputSize, int outputSize);
            ~ANN (void);

            void addLayer (enum layerType, int layerSize);

            void feedForward (float* inputs);

         private:
            Layer*  layers;
            float*  outputs;

            int inputSize;
            int outputSize;
            int numLayers;
      };

   }
}

#endif
