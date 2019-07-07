#include "callpy.h"
#include <iostream>
#include <cmath>

int main ()
{

   int nx = 100;
   int ny = 100;

   int nxNew = 40;
   int nyNew = 40;

   int periods[4] = {4, 8, 16, 32};

   float *data = new float[nx * ny];

   for (int p = 0; p < 4; p++)
   {
      for (int iy = 0, ind = 0; iy < ny; iy++)
      {
         for (int ix = 0; ix < nx; ix++)
         {
            data[ind] = std::sin ((float)ix / (float)nx * 2.0f * M_PI * periods[p]);
         }
      }
   }

   // instantiate an interface to the "fourier" python function
   CallPy fourier (
         ".",           // python path
         "downsample",  // module name
         "fourier",     // function name
         5);            // number of arguments

   // set the arguments
   fourier.set_arg (data, nx * ny);
   fourier.set_arg (nx);
   fourier.set_arg (ny);
   fourier.set_arg (nxNew);
   fourier.set_arg (nyNew);

   // execute the python function
   fourier.execute();

   float *dataNew = new float[nxNew * nyNew + 100*100];

   fourier.read_result (dataNew);

#if 0
   // instantiate an interface to the "fourier" python function
   CallPy imshow (
         ".",           // python path
         "downsample",  // module name
         "imshow",      // function name
         1);            // number of arguments
#endif

   delete[] data;
   delete[] dataNew;

   return 0;
}
