#!/usr/bin/python3

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

   def back (self, Input, Output):
      z = self.forward (Input)
      gamma = (z - Output)

      #delta = gamma * dsigmoid (self.psi[self.N - 2], self.beta)
      delta = gamma

      # layer N - 2
      dEw = np.outer (gamma, self.y[self.N - 2])
      # modify N - 2 weights appropriately

      #delta = np.matmul (gamma, self.weights[self.N - 2])

      # TODO: work this out by hand then do the same (generally) for the notes (paper)
      print ("gamma:")
      print (gamma)
      print (str (self.N) + " layers (zero indexed):")
      for ind in np.arange (self.N - 2, 0, -1):
         print ("layer " + str (ind) + ":")
         print ("weights are:")
         print (self.weights[ind])
         print ("y:")
         print (self.y[ind])
         print ("--> psi:")
         print (self.psi[ind])
         gp    = dsigmoid (self.psi[ind], self.beta)
         Gp    = np.diag (gp)
         print ("--> G':")
         print (Gp)
         aux   = np.matmul (Gp, self.weights[ind])
         delta = np.matmul (delta, aux)
         print ("--> delta:")
         print (delta)
         repDelta = np.tile (delta, (len (self.psi[ind]), 1)).transpose ()
         RHS = np.outer (sigmoid (self.psi[ind], self.beta), self.y[ind])
         print ("--> RHS:")
         print (RHS)
         print ("=======")

'''
      delta = gamma
      print (gamma)
      print (self.psi[self.N - 2])
      # layer N - 3, ..., 0
#      for ind in range (self.N - 3):
      # delta = g' (psi^q) * w^q
      delta = np.matmul (delta, dsigmoid (self.psi[self.N - 2], self.beta))
      print (delta)
      delta = np.matmul (delta, self.weights[self.N - 2])
      rht = np.outer (dsigmoid (self.psi[self.N - 3], self.beta), self.y[self.N-3])
      dEw = np.matmul (delta.transpose (), rht)
'''

def sigmoid (x, beta):
   return 1.0 / (1.0 + np.exp (-beta * x))

def dsigmoid (x, beta):
   sig = sigmoid (x, beta)
   return beta * (1 - sig) * sig

if __name__ == "__main__":
   NN = ANN ([4, 3, 2, 3], 0.4)
   Input = np.random.rand (4)
   #print (NN.forward (Input))
   Output = np.random.rand (3)
   NN.back (Input, Output)

'''
   vector  = np.array ([1, 2, 3, 4])
   vector2 = np.array ([4, 3, 2, 1])
   diagv  = np.diag (vector)
   print (len (vector))
   print (diagv)
   print ()
   print (np.tile (vector, (3, 1)))
   print ()
   print (np.outer (vector, vector2))
'''
