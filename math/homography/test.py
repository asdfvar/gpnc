#!/usr/bin/python3

import random
import numpy as np
from numpy import linalg as la
import homography

#random.seed (a = 0)
#np.random.seed (0)

# Define a random rotation matrix
thx = 2.0 * (random.random () - 0.5) * np.pi
thy = 2.0 * (random.random () - 0.5) * np.pi
thz = 2.0 * (random.random () - 0.5) * np.pi
R_orig = homography.gen_rotation (thx, thy, thz)

print ()
print ("\033[94mOrignal rotation matrix:\033[0m")
print (R_orig)

# Define the plane by a random normal vector
normal_orig = np.random.rand (3)
normal_orig /= la.norm (normal_orig)

print ()
print ("\033[94mOrignal normal vector of the plain:\033[0m")
print (normal_orig)

# Define the distance to the plane
distance_orig = random.random ()

print ()
print ("\033[94mOrignal distance from the plane:\033[0m")
print (distance_orig)

translation_orig = np.random.rand (3)
translation_orig /= la.norm (translation_orig)
translation_orig *= 0.5 * distance_orig

print ()
print ("\033[94mOrignal translation:\033[0m")
print (translation_orig)

# Generate the corresponding homography
H = homography.gen_homography (R_orig, translation_orig, distance_orig, normal_orig)

print ()
print ("\033[94mCorresponding homography H:\033[0m")
print (H)

R, translation, normal, distance = homography.decompose_homography (H)

print ()
print ("After decomposing the homography, the set of possible components are:")

print ()
print ("\033[94mPossible normal vectors:\033[0m")
for el in normal:
   difference = el - normal_orig
   error = la.norm (difference) / la.norm (el)
   if error < 0.01:
      print ("\033[92mCLOSEST MATCH\033[0m with " + str (error) + " error:")
      print (el)
   else:
      print ("with " + str (error) + " error:")
      print (el)
   print ()

print ()
print ("\033[94mPossible rotation matrices:\033[0m")
for el in R:
   difference = el - R_orig
   error = la.norm (difference.flatten ()) / la.norm (el.flatten ())
   if error < 0.01:
      print ("\033[92mCLOSEST MATCH\033[0m with " + str (error) + " error:")
      print (el)
   else:
      print ("with " + str (error) + " error:")
      print (el)
   print ()

print ()
print ("\033[94mPossible translation vectors:\033[0m")
for el in translation:
   difference = el - translation_orig
   error = la.norm (difference) / la.norm (el)
   if error < 0.01:
      print ("\033[92mCLOSEST MATCH\033[0m with " + str (error) + " error:")
      print (el)
   else:
      print ("with " + str (error) + " error:")
      print (el)
   print ()

print ()
print ("\033[94mPossible distances:\033[0m")
for el in distance:
   print (el)
   print ()
