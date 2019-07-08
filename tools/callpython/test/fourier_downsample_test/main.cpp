#include "callpy.h"
#include <iostream>
#include <cmath>

int main ()
{

   int nx = 100;
   int ny = 100;

   int nxNew = 40;
   int nyNew = 40;

   float periods[4] = {4.0f, 8.0f, 16.0f, 32.0f};

   float *data = new float[nx * ny];

   for (int ind = 0; ind < nx * ny; ind++) data[ind] = 0.0f;

   for (int p = 0; p < 1; p++)
   {
      for (int iy = 0, ind = 0; iy < ny; iy++)
      {
         for (int ix = 0; ix < nx; ix++, ind++)
         {
            data[ind] += std::sin ((float)ix / (float)nx * 2.0f * M_PI * periods[p]/periods[p]);
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

   float *dataNew = new float[nxNew * nyNew];

   fourier.read_result (dataNew);

#if 1
   // instantiate an interface to the "fourier" python function
   CallPy savefig (
         ".",           // python path
         "downsample",  // module name
         "savefig",     // function name
         3);            // number of arguments

#if 0
std::cout << "dataNew = " << std::endl;
for (int indy = 0, ind = 0; indy < nyNew; indy++)
for (int indx = 0; indx < nyNew; indx++, ind++)
std::cout << dataNew[ind] << ", ";
std::cout << std::endl;
#endif

   savefig.set_arg (dataNew, nxNew * nyNew);
   savefig.set_arg (nxNew);
   savefig.set_arg (nyNew);

   savefig.execute();
#endif

   delete[] data;
   delete[] dataNew;

   return 0;
}
