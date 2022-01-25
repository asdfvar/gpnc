#!/usr/bin/python3

import numpy as np
from numpy import linalg as la
from matplotlib import pyplot as plt
import random
import pose_estimation

random.seed (0)
np.random.seed (0)

N = 12

# Define arbitrary points uniformly distributed in a [-1, 1) x [-1, 1) x [-1, 1) box
P = np.random.rand (3, N) * 2.0 - 1.0

# Define the observer location at -t from the origin where t is chosen s.t |t| = [10, 20)
t = np.random.rand (3); t /= la.norm (t); t *= random.random () * 10.0 + 10.0

# Define the observer's orientation with the look in it's z-direction and perturbed by +/- 90
# degrees from the direction of the origin so that all points, P, are observable
zlook = t; zlook /= la.norm (zlook)
Rpurt = pose_estimation.rotation (
      np.deg2rad (random.random () * 180.0 - 90.0),
      np.deg2rad (random.random () * 180.0 - 90.0),
      np.deg2rad (random.random () * 180.0 - 90.0))
zlook = np.matmul (Rpurt, zlook)

# If R rotates a onto b, then
# R = I + [v]_x + [v]_x^2 * (1 - a \dot b) / |v|^2
# where v = a \cross b
def rotation_of_vectors (a, b):
   v = np.cross (a, b)
   vcross = -np.cross (v, np.eye (len (v)))
   R = np.eye (len (v)) + vcross + \
       np.matmul (vcross, vcross) * (1.0 - np.dot (a, b)) / la.norm (v)**2
   return R

# Determine the rotation matrix, R, from the observer's look direction to (0, 0, 1)
zaxis = np.array ([0.0, 0.0, 1.0])
R = rotation_of_vectors (zlook, zaxis)

# Determine the points from the observer's perspective, Q: Q = R*P + t
Q = np.matmul (R, P) + t[:, np.newaxis]

# Project the points, P, onto the CCD of the observer: Q / norm (zlook)
v = Q / np.matmul (zlook, Q)

# Print the orthogonal error for the exact solution
print ("Error for the exact solution:")
print (pose_estimation.error (P, v, R, t))
print ()

# Perturb the rotation by +/-10 degrees and translation by +/-5 for the initial estimate
R0 = np.matmul (pose_estimation.rotation (
      np.deg2rad (90.0 * (random.random () - 0.5)),
      np.deg2rad (90.0 * (random.random () - 0.5)),
      np.deg2rad (90.0 * (random.random () - 0.5))), R)

tpurt = np.random.rand (3); tpurt /= la.norm (tpurt); tpurt *= random.random () * 10.0 - 5.0
t0 = t + tpurt

# Print the orthogonal error for the perturbed initial estimate
print ("Error for the initial solution:")
print (pose_estimation.error (P, v, R0, t0))
print ()

Re = R0
te = t0
for iteration in range (10):
   Re, te = pose_estimation.iterate (P, v, Re)
   error = pose_estimation.error (P, v, Re, te)
   print ("Error after the " + str (iteration) + " iteration = " + str (error))
print ()

print ("The truth rotation:")
print (R)
print ("The estimated rotation:")
print (Re)
