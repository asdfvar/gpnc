#!/usr/bin/python3

import numpy as np

class ANN:
   def __init__(self, layers, actFunc, beta = 0.9):
      np.random.seed (0)
      self.N = len (layers)
      self.weights = [None] * (self.N - 1)
      self.bias    = [None] * (self.N - 1)
      self.psi     = [None] * (self.N)
      self.y       = [None] * (self.N)

      actFuncs = {"sigmoid": self.sigmoid (beta),
                  "unit":    self.unit () }

      self.g       = actFuncs[actFunc]

      for ind in range (len (self.weights)):
         # weights
         self.weights[ind] = np.random.rand (layers[ind + 1], layers[ind])
         # bias weights
         self.bias[ind] = np.random.rand (layers[ind + 1])

   def forward (self, Input):
      y = Input
      for ind in range (self.N - 1):
         self.y[ind] = y;
         y  = np.matmul (self.weights[ind], y)
         y += self.bias[ind]
         self.psi[ind] = y
         y  = self.g.activate (y)
      return y

   def back (self, Input, Output, stepsize):
      z = self.forward (Input)
      diff = z - Output
      gamma = diff * self.g.dactivate (self.psi[self.N - 2])

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
            gp   = self.g.dactivate (self.psi[ind])
            RHS  = np.outer (gp, self.y[ind])
            dEdw = LHS * RHS

            self.weights[ind] -= stepsize * dEdw
            dEdu = delta * gp
            self.bias[ind]    -= stepsize * dEdu

            if ind > 0:
               # update delta for the next layer
               Gp    = np.diag (gp)
               Gpw   = np.matmul (Gp, self.weights[ind])
               delta = np.matmul (delta, Gpw)

      z = self.forward (Input)
      diff = z - Output
      return 0.5 * np.sum (diff * diff)

   class activation:
      def activate  (self, x): pass
      def dactivate (self, x): pass

   class unit:
      def activate  (self, x): return x
      def dactivate (self, x): return np.ones (len (x))

   class sigmoid (activation):
      def __init__ (self, beta):
         self.beta = beta
      def activate (self, x):
         return 1.0 / (1.0 + np.exp (-self.beta * x))
      def dactivate (self, x):
         sig = self.activate (x)
         return self.beta * (1 - sig) * sig
