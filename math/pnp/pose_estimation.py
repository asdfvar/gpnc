#!/usr/bin/python3

import numpy as np
from numpy import linalg as la

def rotation (theta_x, theta_y, theta_z):
   Rx = np.array ([
         [ 1.0,                            0.0,               0.0],
         [ 0.0,               np.cos (theta_x), -np.sin (theta_x)],
         [ 0.0,               np.sin (theta_x),  np.cos (theta_x)]])
   Ry = np.array ([
         [ np.cos (theta_x),               0.0,  np.sin (theta_x)],
         [              0.0,               1.0,               0.0],
         [-np.sin (theta_x),               0.0,  np.cos (theta_x)]])
   Rz = np.array ([
         [ np.cos (theta_x), -np.sin (theta_x),               0.0],
         [ np.sin (theta_x),  np.cos (theta_x),               0.0],
         [             0.0,                0.0,               1.0]])
   return (np.matmul (Rz, np.matmul (Ry, Rx)))

# Error E (R, t) = sum ((I - V_i)*(R*p_i + t), i)
# where V_i is the LOS projection matrix: V_i = (v_i * v_i^t)/(v_i^t * v_i)
def error (P, v, R, t):
   Error = 0.0
   for ind in range (v.shape[1]):
      V = np.outer (v[:,ind], v[:,ind]) / np.dot (v[:,ind], v[:,ind])
      term_1 = np.eye (3) - V
      term_2 = np.matmul (R, P[:,ind]) + t
      Error += la.norm (np.matmul (term_1, term_2))**2
   return Error

# Determine the initial translation estimate, ti, given the rotation
# estimate, Ri, from the objective function which has a closed form
# solution:
# ti = 1/n * (I - 1/n * sum (Vi, i))^-1 * sum ((Vi - I) * Ri * pi, i)
# where Vi is the projection matrix: Vi = (vi*vi^t)/(vi^t*vi)
def translation (P, v, R):

   N = v.shape[1]

   term1 = np.zeros ((3, 3))
   term2 = np.zeros (3)
   for ind in range (N):
      V = np.outer (v[:,ind], v[:,ind]) / np.dot (v[:,ind], v[:,ind])
      term1 += V
      term2 += np.matmul (V - np.eye (3), np.matmul (R, P[:, ind]))
   term1 = np.eye (3) - 1.0 / N * term1
   ti = 1.0 / N * np.matmul (la.inv (term1), term2)

   return ti

# Perform an iteration to improve the estimate of the rotation, R, and
# translation, t, from an initial rotation estimate, Ri. Q = R*P + t defines
# the points from the camera's point of view
def iterate (P, v, Ri):

   ti = translation (P, v, Ri)

   Q = np.matmul (Ri, P) + ti[:, np.newaxis]

   Pbar = np.mean (P, axis = 1)
   Qbar = np.mean (Q, axis = 1)

   Pp = P - Pbar[:, np.newaxis]
   Qp = Q - Qbar[:, np.newaxis]

   # Determine the cross-covariance matrix
   M = np.zeros ((3, 3), dtype = float)
   for ind in range (len (v)):
      M += np.outer (Qp[:, ind], Pp[:, ind])

   # Perform SVD of M = U * S * Vh
   U, S, Vh = la.svd (M)
   R = np.matmul (U, Vh).transpose ()

   t = translation (P, v, R)

   return R, t
