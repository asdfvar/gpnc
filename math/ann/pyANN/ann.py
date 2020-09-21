#!/usr/bin/python

import numpy as np

class ANN:
   def __init__(self, layers, beta):
      self.N = len (layers)
      self.weights = [None] * (self.N - 1)
      self.bias    = [None] * (self.N - 1)
      self.psi     = [None] * (self.N)
      self.y       = [None] * (self.N)
      for ind in range (len (self.weights)):
         # weights
         self.weights[ind] = np.random.rand (layers[ind + 1], layers[ind])
         # bias weights
         self.bias[ind] = np.random.rand (layers[ind + 1])
      self.beta = beta

   def forward (self, Input):
      y = Input
      for ind in range (len (self.weights)):
         self.y[ind] = y;
         y  = np.matmul (self.weights[ind], y)
         y += self.bias[ind]
         self.psi[ind] = y
         y  = sigmoid (y, self.beta)
      return y

   def back (self, Input, Output):
      z = self.forward (Input)
      gamma = (z - Output) * dsigmoid (self.psi[self.N - 2], self.beta)

      # layer N - 2
      dEw = np.outer (gamma, self.y[self.N - 2])

      # layer N - 3
      lht = np.matmul (gamma, self.weights[self.N - 2]).transpose ()
      rht = np.outer (dsigmoid (self.psi[self.N - 3], self.beta), self.y[self.N-3])
      dEw = np.matmul (lht, rht)

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
