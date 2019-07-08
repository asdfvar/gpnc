#!/usr/bin/python

import downsample
import numpy as np

data = np.random.rand (100, 100)

x = np.sin (np.linspace (0, 2.0 * np.pi, 100))
for iy in range(0,100):
   for ix in range(0,100):
      data[iy, ix] = x[ix]
print (type(data))

dataNew = downsample.fourier (data, 100, 100, 40, 40)

downsample.savefig (dataNew, 40, 40);
