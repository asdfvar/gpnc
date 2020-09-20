#!/usr/bin/python

import numpy as np

class ANN:
   def __init__(self, layers, beta):
      self.weights = [None] * (len (layers) - 1)
      self.bias    = [None] * (len (layers) - 1)
      self.psi     = [None] * (len (layers))
      for ind in range (len (self.weights)):
         # weights
         self.weights[ind] = np.random.rand (layers[ind + 1], layers[ind])
         # bias weights
         self.bias[ind] = np.random.rand (layers[ind + 1])
      self.beta = beta

   def forward (self, Input):
      y = Input
      for ind in range (len (self.weights)):
         y  = np.matmul (self.weights[ind], y)
         y += self.bias[ind]
         self.psi[ind] = y
         y  = sigmoid (y, self.beta)
      return y

   def back (self, Input, Output):
      z = self.forward (Input)
      N = len (self.psi)
      gamma = (z - Output) * dsigmoid (self.psi[N - 2], self.beta)

def sigmoid (x, beta):
   return 1.0 / (1.0 + np.exp (-beta * x))

def dsigmoid (x, beta):
   sig = sigmoid (x, beta)
   return beta * (1 - sig) * sig

if __name__ == "__main__":
   NN = ANN ([4, 3, 2, 3], 0.4)
   Input = np.random.rand (4)
   print (NN.forward (Input))
   Output = np.random.rand (3)
   NN.back (Input, Output)
