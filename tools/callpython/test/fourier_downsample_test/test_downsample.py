#!/usr/bin/python

import downsample
import numpy as np

data = np.random.rand (100, 100)

dataNew = downsample.fourier (data, 100, 100, 40, 40)
