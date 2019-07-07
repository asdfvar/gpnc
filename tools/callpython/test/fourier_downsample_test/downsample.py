import numpy as np
from matplotlib import pyplot as pl

def fourier (data, nx, ny, newNx, newNy):
   print ("hello")
   data     = np.array (list (data))
   data     = np.reshape (data, (nx, ny))
   print ("hello")
   print ("size = ", np.shape (data))
   Fdata    = np.fft.fft2 (data);
   print ("hello")
   Fdata    = np.fft.fftshift (Fdata);
   x0       = np.shape (Fdata)[1] / 2 - newNx / 2
   y0       = np.shape (Fdata)[0] / 2 - newNy / 2
   FdataNew = Fdata[x0:x0+newNx, y0:y0+newNy];
   dataNew  = np.fft.ifft2 (FdataNew)
   return dataNew

def imshow (data):
   pl.imshow (data)
   pl.show()
