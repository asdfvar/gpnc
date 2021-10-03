#!/usr/bin/python3

import numpy as np
from numpy import linalg as la

# H = d R + t * n'

def gen_rotation (thx, thy, thz):
   R = np.identity (3)

   Rx = [[1.0,  0.0,           0.0         ],
         [0.0,  np.cos (thx), -np.sin (thx)],
         [0.0,  np.sin (thx),  np.cos (thx)]]
   R = np.matmul (R, np.array (Rx))

   Ry = [[np.cos (thy),  0.0,         -np.sin (thy)],
         [0.0,           1.0,          0.0         ],
         [np.sin (thy),  0.0,          np.cos (thy)]]
   R = np.matmul (R, np.array (Ry))

   Rz = [[np.cos (thz), -np.sin (thz), 0.0],
         [np.sin (thz),  np.cos (thz), 0.0],
         [0.0,           0.0         , 1.0]]
   R = np.matmul (R, np.array (Rz))

   return R

def gen_homography (R, t, d, n):
   return d * R - np.outer (t, n)

def decompose_homography (H):
   U, lamb, Vh = la.svd (H)
   # H = np.matmul (U * lamb, Vh)

   # R = s * U * R' * Vh
   # t = U * t'
   # n = Vh^T * n'
   # d = V * n'
   # s = det (U) * det (V)

   s = la.det (U) * la.det (Vh)

   # Determine the set of possible normal vectors
   x1 = np.sqrt ((lamb[0]**2 - lamb[1]**2) / (lamb[0]**2 - lamb[2]**2))
   x2 = 0.0
   x3 = np.sqrt ((lamb[1]**2 - lamb[2]**2) / (lamb[0]**2 - lamb[2]**2))

   normalp = []
   Rp      = []
   tp      = []

   normalp.append (np.array ([ x1,  x2,  x3]))
   normalp.append (np.array ([-x1,  x2,  x3]))
   normalp.append (np.array ([ x1,  x2, -x3]))
   normalp.append (np.array ([-x1,  x2, -x3]))

   if s > 0:
      # Determine the set of possible rotation matrices
      sinth = (lamb[0] - lamb[2]) * x1 * x3 / lamb[1]
      costh = (lamb[0] * x3**2 + lamb[2] * x1**2) / lamb[1]
      Rp.append (np.array (
               [[costh,   0.0, -sinth],
                [  0.0,   1.0,  0.0  ],
                [sinth,   0.0,  costh]]))
      Rp.append (np.array (
               [[costh,   0.0,  sinth],
                [  0.0,   1.0,  0.0  ],
                [-sinth,   0.0,  costh]]))
      Rp.append (np.array (
               [[costh,   0.0,  sinth],
                [  0.0,   1.0,  0.0  ],
                [-sinth,   0.0,  costh]]))
      Rp.append (np.array (
               [[costh,   0.0, -sinth],
                [  0.0,   1.0,  0.0  ],
                [sinth,   0.0,  costh]]))

      # Determine the set of possible translation vectors
      tp.append ((lamb[0] - lamb[2]) * np.array ([ x1,  0.0, -x3]))
      tp.append ((lamb[0] - lamb[2]) * np.array ([-x1,  0.0, -x3]))
      tp.append ((lamb[0] - lamb[2]) * np.array ([ x1,  0.0,  x3]))
      tp.append ((lamb[0] - lamb[2]) * np.array ([-x1,  0.0,  x3]))

   else:
      # Determine the set of possible rotation matrices
      sinth = (lamb[0] + lamb[2]) * x1 * x3 / lamb[1]
      costh = (lamb[0] * x3**2 - lamb[2] * x1**2) / lamb[1]
      Rp.append (np.array (
               [[costh,   0.0,  sinth],
                [  0.0,  -1.0,  0.0  ],
                [sinth,   0.0, -costh]]))
      Rp.append (np.array (
               [[ costh,   0.0, -sinth],
                [   0.0,  -1.0,  0.0  ],
                [-sinth,   0.0, -costh]]))
      Rp.append (np.array (
               [[ costh,   0.0, -sinth],
                [   0.0,  -1.0,  0.0  ],
                [-sinth,   0.0, -costh]]))
      Rp.append (np.array (
               [[costh,   0.0,  sinth],
                [  0.0,  -1.0,  0.0  ],
                [sinth,   0.0, -costh]]))

      # Determine the set of possible translation vectors
      tp.append ((lamb[0] - lamb[2]) * np.array ([ x1,  0.0,  x3]))
      tp.append ((lamb[0] - lamb[2]) * np.array ([-x1,  0.0,  x3]))
      tp.append ((lamb[0] - lamb[2]) * np.array ([ x1,  0.0, -x3]))
      tp.append ((lamb[0] - lamb[2]) * np.array ([-x1,  0.0, -x3]))


   normal = []
   for el in normalp: normal.append (np.matmul (Vh.transpose (), el))

   R = []
   for el in Rp: R.append (s * np.matmul (np.matmul (U, el), Vh))

   t = []
   for el in tp: t.append (np.matmul (U, el))

   d = []
   for el in range (4):
      dpRp = np.eye (3) * lamb - np.outer (tp[el], normalp[el])
      dpdiag = np.matmul (dpRp, Rp[el].transpose ())
      d.append (s * np.mean (np.diag (dpdiag)))

   return R, t, normal, d
