#!/usr/bin/python3

import random
import numpy as np
from numpy import linalg as la
import homography


# Generated the rotation matrix
R = homography.gen_rotation (0.0, 0.0, 0.0)

# Generate the normal vector
normal = np.array ([1.0, 0.0, 0.0])
normal /= la.norm (normal)

# Generate the translation vector
translation = np.array ([7.0, 0.0, 0.0])

distance = 100.0

# Generate the corresponding homography
H = homography.gen_homography (R, translation, distance, normal)

print ("H:")
print (H)

R_res, translation_res, normal_res, distance_res = homography.decompose_homography (H)

print ("Resulting Rotation:")
for el in R_res: print (el)

print ("Translation:")
for el in translation_res: print (el)
