#!/usr/bin/python3

import random
from matplotlib import pyplot as plt

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
      for ind in range (self.N - 1):
         self.y[ind] = y;
         y  = np.matmul (self.weights[ind], y)
         y += self.bias[ind]
         self.psi[ind] = y
         y  = sigmoid (y, self.beta)
      return y

   def back (self, Input, Output, stepsize):
      z = self.forward (Input)
      diff = z - Output
      gamma = diff * dsigmoid (self.psi[self.N - 2], self.beta)

      for ind in np.arange (self.N - 2, -1, -1):
         if (ind == self.N - 2):
            dEdw = np.outer (gamma, self.y[ind])
            self.weights[ind] -= stepsize * dEdw
            dEdu = gamma
            self.bias[ind]    -= stepsize * dEdu

            # update delta for the next layer
            delta = np.matmul (gamma, self.weights[ind])
         else:

            # use the previously calculated delta to update the weights
            LHS  = np.tile (delta, (len (self.y[ind]), 1)).transpose ()
            gp   = dsigmoid (self.psi[ind], self.beta)
            RHS  = np.outer (gp, self.y[ind])
            dEdw = LHS * RHS

            self.weights[ind] -= stepsize * dEdw
            dEdu = delta * gp
            self.bias[ind]    -= stepsize * dEdu

            # update delta for the next layer
            Gp    = np.diag (gp)
            Gpw   = np.matmul (Gp, self.weights[ind])
            delta = np.matmul (delta, Gpw)

      return 0.5 * np.sum (diff * diff)

def sigmoid (x, beta):
   return 1.0 / (1.0 + np.exp (-beta * x))

def dsigmoid (x, beta):
   sig = sigmoid (x, beta)
   return beta * (1 - sig) * sig

if __name__ == "__main__":
   NN = ANN ([4, 3, 2, 2], 0.99)

   Input = []
   Input.append (np.array ([1.0, 1.0, 1.0, 1.0]))
   Input.append (np.array ([1.1, 0.9, 0.9, 1.1]))
   Input.append (np.array ([0.9, 0.7, 1.1, 1.0]))
   Input.append (np.array ([0.8, 0.9, 1.0, 0.8]))

   Input.append (np.array ([-1.0, -1.0, -1.0, -1.0]))
   Input.append (np.array ([-1.1, -0.9, -0.9, -1.1]))
   Input.append (np.array ([-0.9, -0.7, -1.1, -1.0]))
   Input.append (np.array ([-0.8, -0.9, -1.0, -0.8]))

   Output = []
   Output.append (np.array ([1.0, 0.0]))
   Output.append (np.array ([1.0, 0.0]))
   Output.append (np.array ([1.0, 0.0]))
   Output.append (np.array ([1.0, 0.0]))

   Output.append (np.array ([0.0, 1.0]))
   Output.append (np.array ([0.0, 1.0]))
   Output.append (np.array ([0.0, 1.0]))
   Output.append (np.array ([0.0, 1.0]))

   #print (NN.forward (Input))
   error_list = []
   for k in range (100000):
      
      randind = random.randint (0, 7)
      Error = NN.back (Input[randind], Output[randind], 0.01)
      error_list.append (Error)

   plt.plot (error_list)
   plt.show ()

   print (NN.forward (np.array ([2, 2, 2, 2])))
   print (NN.forward (np.array ([-1, -1, -1, -1])))
