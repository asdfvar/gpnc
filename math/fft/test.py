#!/usr/bin/python3

import numpy as np

N = 4
np.random.seed (0)
x = np.random.rand (2*N); x = x[0::2] + 1j*x[1::2]

print (x)

y = np.fft.fft (x)

print ("fft (x) = " + str (y))
