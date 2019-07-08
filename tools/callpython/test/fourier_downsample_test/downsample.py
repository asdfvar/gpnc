import numpy as np
from matplotlib import pyplot as pl

def fourier (data, nx, ny, newNx, newNy):
   data     = np.array (list (data))
   data     = np.reshape (data, (nx, ny))
   #Fdata    = np.fft.fft2 (data);
   #Fdata    = np.fft.fftshift (Fdata);
   #x0       = np.shape (Fdata)[1] / 2 - newNx / 2
   #y0       = np.shape (Fdata)[0] / 2 - newNy / 2
   #FdataNew = Fdata[x0:x0+newNx, y0:y0+newNy];
   #FdataNew = np.fft.fftshift (FdataNew);
   #dataNew  = np.fft.ifft2 (FdataNew).real
   ##savefig (data, nx, ny)
   dataNew = data
   dataNew = tuple(dataNew.flatten())
   return dataNew

def savefig (data, nx, ny):
   data  = np.array (list (data))
   print (type(data))
   data -= data.min()
   data  = np.reshape (data, (nx, ny))
   data /= data.max() - data.min()
   data *= 255.0
   data = data.astype(int)
   print (type(data))

   pl.imshow (data)
   pl.savefig("image.png")
