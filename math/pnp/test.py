#!/usr/bin/python3

import numpy as np
from numpy import linalg as la
from matplotlib import pyplot as plt
import random
import pose_estimation
import time

random.seed (0)
np.random.seed (0)

N = 20

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

# Project the points, Q, onto the CCD of the observer: Q / norm (zlook)
v = Q / np.matmul (zlook, Q)

# Print the orthogonal error for the exact solution
print ("Orthogonal error for the exact solution:")
print (pose_estimation.error (P, v, R, t))
print ()

# Perturb the rotation by a random amount within +/-90 degrees
theta_x = 90.0 * (random.random () * 2.0 - 1.0)
theta_y = 90.0 * (random.random () * 2.0 - 1.0)
theta_z = 90.0 * (random.random () * 2.0 - 1.0)
print ()
print (theta_x)
print (theta_y)
print (theta_z)
print ()
Roffset = pose_estimation.rotation (
      np.deg2rad (theta_x),
      np.deg2rad (theta_y),
      np.deg2rad (theta_z))
print ("Roffset = " + str (Roffset))
R0 = np.matmul (Roffset, R)

# Estimate the initial translation from the initial rotation estimate
_, t0 = pose_estimation.iterate (P, v, R0)

# Determine the points from the observer's perspective with the estimated parameters, Q:
# Qe = Re*P + te
Q0 = np.matmul (R0, P) + t0[:, np.newaxis]

# Determine the error in terms of |(Q0 - Q) / |Q|| of the initial estimate
print ("Error in the initial estimate in terms of |(Q0 - Q) / |Q||:")
print (la.norm ((Q0 - Q) / la.norm (Q, axis = 0)))

# Print the orthogonal error for the perturbed initial estimate
print ("Total orthogonal error for the initial estimate:")
print (pose_estimation.error (P, v, R0, t0))
print ()

Re = R0

# Iteratively solve for the rotation and translation
start = time.time ()
iterations = 10
errors = np.zeros (iterations)
angle_errors = np.zeros (iterations)
for iteration in range (iterations):
   b = np.matmul (Re, [0, 0, 1])
   Re, te = pose_estimation.iterate (P, v, Re)
   error = pose_estimation.error (P, v, Re, te)
   errors[iteration] = error
   print ("Orthogonal error after the " + str (iteration) + " iteration = " + str (error))
   a = np.matmul (R, [0, 0, 1])
   angle_error = np.arccos (np.dot (a,b))
   #print ("anglular error = " + str (np.rad2deg (angle_error)))
   angle_errors[iteration] = angle_error
end = time.time ()
print ("time = " + str (end - start) + " seconds")
print ()

print ("The true rotation:");      print (R)
print ("The estimated rotation:"); print (Re)
print ("The true translation             = " + str (t))
print ("The estimated translation        = " + str (te))
print ("Absolute error in translation    = " + str (la.norm (te - t)))
print ("Orthogonal error in the estimate = " + str (error) + " (" + str (error / N) + " normalized)")
print ("angular error (degrees)          = " + str (np.rad2deg (angle_error)))

# Determine the points from the observer's perspective with the estimated parameters, Q:
# Qe = Re*P + te
Qe = np.matmul (Re, P) + te[:, np.newaxis]

# Determine the relative error in terms of |(Qe - Q) / |Q||
print ("Relative error |(Qe - Q) / |Q||:")
print (la.norm ((Qe - Q) / la.norm (Q, axis = 0)))

plt.figure();
plt.semilogy (errors)
plt.title ("Orthogonal error")

plt.figure();
plt.semilogy (np.rad2deg (angle_errors))
plt.title ("Angular error (degrees)")

plt.show ()
