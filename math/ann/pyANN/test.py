#!/usr/bin/python3

import ann
import numpy as np
from matplotlib import pyplot as plt
import random

NN = ann.ANN ([2, 3, 2, 2], 0.99)

offset = np.array ([0.0, 4.0])

Input = []
Input.append (np.array ([1.0, 1.0]) + offset)
Input.append (np.array ([1.1, 0.9]) + offset)
Input.append (np.array ([0.9, 0.7]) + offset)
Input.append (np.array ([0.8, 0.9]) + offset)

Input.append (np.array ([-1.0, -1.0]) + offset)
Input.append (np.array ([-1.1, -0.9]) + offset)
Input.append (np.array ([-0.9, -0.7]) + offset)
Input.append (np.array ([-0.8, -0.9]) + offset)

Output = []
Output.append (np.array ([1.0, 0.0]))
Output.append (np.array ([1.0, 0.0]))
Output.append (np.array ([1.0, 0.0]))
Output.append (np.array ([1.0, 0.0]))

Output.append (np.array ([0.0, 1.0]))
Output.append (np.array ([0.0, 1.0]))
Output.append (np.array ([0.0, 1.0]))
Output.append (np.array ([0.0, 1.0]))

N = 500000
error_list = np.zeros (N)
for k in range (N):
   randind = random.randint (0, 7)
   Error = NN.back (Input[randind], Output[randind], 0.01)
   error_list[k] = Error

plt.plot (error_list)
plt.show ()

print (NN.forward (np.array ([2, 2])))
print (NN.forward (np.array ([-1, -1])))
