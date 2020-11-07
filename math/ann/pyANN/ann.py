#!/usr/bin/python3

# inputs are tensors which are defined as Python lists with dimensions like so:
# [batch, channel, 2-D numpy array]
#
# in some cases, they are expressed as [layer, batch, channel, 2-D numpy array]

import numpy as np

class ANN:
   def __init__(self, layers, numChannels, actFunc, beta = 0.9):
      np.random.seed (0)
      self.N = len (layers)
      self.numChannels = numChannels

      actFuncs = {"sigmoid": self.sigmoid (beta),
                  "unit":    self.unit () }

      self.g = actFuncs[actFunc]

      # the following are expressed as [layer][channel][numpy array]
      self.weights = []
      self.bias    = []
      for layer in range (len (layers)):
         self.weights.append ([None] * numChannels[layer])
         self.bias.append    ([None] * numChannels[layer])

      for layer in range (self.N - 1):
         for channel in range (len (self.weights[layer])):
            # weights
            self.weights[layer][channel] = np.random.rand (layers[layer + 1], layers[layer])
            # bias weights
            self.bias[layer][channel]    = np.random.rand (layers[layer + 1])

   # "Input" is in the form of [batch][channel][numpy array]
   def forward (self, Input):

      # the following are expressed as [layer][batch][channel][numpy array]
      self.psi   = []
      self.y     = []
      self.delta = []
      for layer in range (self.N):
         self.psi.append   ([])
         self.y.append     ([])
         self.delta.append ([])
         for batch in range (len (Input)):
            self.psi[layer].append   ([])
            self.y[layer].append     ([])
            self.delta[layer].append ([])
            for channel in range (self.numChannels[layer]):
               self.psi[layer][channel].append   ([])
               self.y[layer][channel].append     ([])
               self.delta[layer][channel].append ([])

      self.y[0] = Input
      for layer in range (self.N - 1):
         for batch in range (len (self.y[layer])):
            for channel in range (len (self.y[layer][batch])):
               self.psi[layer][batch][channel] = np.matmul (self.weights[layer][channel], self.y[layer][batch][channel]) + self.bias[layer][channel]
               self.y[layer + 1][batch][channel] = self.g.activate (self.psi[layer][batch][channel])
      return self.y[self.N - 1]

   # "Input" is in the form of [batch, channel, 2-D numpy array]
   # "Output" is in the form of [batch, channel, 2-D numpy array]
   # "stepsize" is a positive real number specifying the step size used in gradient descent
   def back (self, Input, Output, stepsize):
      z = self.forward (Input)
      # the following are expressed as [batch][channel][numpy array]
      diff  = []
      gamma = []
      for batch in range (len (z)):
         diff.append  ([None])
         gamma.append ([None])

      for batch in range (len (self.y[self.N - 2])):
         for channel in range (len (self.y[self.N - 2][batch])):
            diff[batch][channel] = z[batch][channel] - Output[batch][channel]
            gamma[batch][channel] = diff[batch][channel] * self.g.dactivate (self.psi[self.N - 2][batch][channel])

      for layer in np.arange (self.N - 2, -1, -1):
         for batch in range (len (self.y[layer])):
            for channel in range (len (self.y[layer][batch])):
               lweights = self.weights[layer][channel]
               ldelta = self.delta[batch][channel]
               # last layer
               if (layer == self.N - 2):
                  dEdw = np.outer (gamma[batch][channel], self.y[layer][batch][channel])
                  lweights -= stepsize * dEdw
                  dEdu = gamma[batch][channel]
                  self.bias[layer][channel] -= stepsize * dEdu

                  # update delta for the next layer
                  self.delta[batch][channel] = np.matmul (gamma[batch][channel], lweights)

               # all other interior layers
               else:
                  # use the previously calculated delta to update the weights
                  LHS  = np.tile (ldelta, (len (self.y[layer]), 1)).transpose ()
                  gp   = self.g.dactivate (self.psi[layer][batch][channel])
                  RHS  = np.outer (gp, self.y[layer])
                  dEdw = LHS * RHS

                  lweights -= stepsize * dEdw
                  dEdu          = ldelta * gp
                  self.bias[layer]             -= stepsize * dEdu

                  if layer > 0:
                     # update delta for the next layer
                     Gp    = np.diag (gp)
                     Gpw   = np.matmul (Gp, lweights)
                     self.delta[batch][channel] = np.matmul (ldelta, Gpw)

      Error = 0.0
      z = self.forward (Input)
      for batch in range (len (self.y[self.N - 2])):
         for channel in range (len (self.y[self.N - 2][batch])):
            diff[batch][channel] = z[batch][channel] - Output[batch][channel]
            Error += 0.5 * np.sum (diff[batch][channel] * diff[batch][channel])

      return Error

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
