#!/usr/bin/python3

import ann
import numpy as np
from matplotlib import pyplot as plt
import random
import time
import progressbar

NN = ann.ANN ([2, 3, 3, 1], 0.9)

Input = []
Output = []

for ind in range (6):
   Input.append (np.random.normal ([-1.0, -1.0], 0.2))
   Output.append (np.array ([1.0]))

for ind in range (6):
   Input.append (np.random.normal ([1.0, -1.0], 0.2))
   Output.append (np.array ([1.0]))

for ind in range (6):
   Input.append (np.random.normal ([0.0, 1.0], 0.2))
   Output.append (np.array ([1.0]))

for ind in range (6):
   Input.append (np.random.normal ([-1.0, 1.0], 0.2))
   Output.append (np.array ([0.0]))

for ind in range (6):
   Input.append (np.random.normal ([1.0, 1.0], 0.2))
   Output.append (np.array ([0.0]))

for ind in range (6):
   Input.append (np.random.normal ([0.0, -1.0], 0.2))
   Output.append (np.array ([0.0]))

# Train
N = 10000000
error_list = np.zeros (N)
start = time.time ()
for k in progressbar.progressbar (range (N)):
   randind = random.randint (0, len (Input) - 1)
   Error = NN.back (Input[randind], Output[randind], 0.01)
   error_list[k] = Error
end = time.time ()
print ("training time: " + str (end - start))

print ("weights:")
for weights in NN.weights:
   print (weights)
   print ()

fig1 = plt.figure (1)
plt.plot (error_list)
plt.title ("Error")

xgrid = np.linspace (-2.0, 2.0, 100)
ygrid = np.linspace (-2.0, 2.0, 100)

fig2 = plt.figure (2)
start = time.time ()
for y in progressbar.progressbar (ygrid):
   for x in xgrid:
      value = NN.forward (np.array ([x, y]))[0]
      if value >= 0.5: Color = 'darksalmon'
      if value <  0.5: Color = 'aqua'
      plt.plot (x, y, '.', color = Color)

for ind in range (len (Input)):
   if Output[ind][0] >= 0.5: Color = 'r'
   if Output[ind][0] <  0.5: Color = 'b'
   plt.plot (Input[ind][0], Input[ind][1], '.', color = Color)
end = time.time ()
print ("plotting time: " + str (end - start))

plt.show ()
