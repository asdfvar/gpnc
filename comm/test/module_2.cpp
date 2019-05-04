#include "comm.h"
#include <iostream>
#include "configuration.h"

int main (int argc, char *argv[])
{

   comm::COMM Comm (&argc, &argv, NUM_STAGES, 2);

   float *array0 = new float[4];
   float *array1 = new float[4];

   array0[0] = 0.0f;
   array0[1] = 0.0f;
   array0[2] = 0.0f;
   array0[3] = 0.0f;

   if (Comm.rank() == 1) {
      Comm.receive_from_stage (
            array0, // data
            4,     // data size
            0,     // sending stage
            0,     // sending stage rank
            2);    // tag

      Comm.wait_for_receive_from_stage (
            0,     // sending stage
            0,     // sending stage rank
            2);    // tag

      std::cout << __FILE__ << ": local rank " << Comm.rank() << " received data from stage 0: ";
      for (int ind = 0; ind < 4; ind++)
      {
         std::cout << array0[ind] << ", ";
      }
      std::cout << std::endl;
   }

   for (int ind = 0; ind < 4; ind++) array0[ind] += 1.0f;
   std::cout << __FILE__ << ": incremented the data by 1" << std::endl;

   Comm.send (
         array0,          // data
         4,               // data size
         1 - Comm.rank(), // receiving stage rank
         3);              // tag

   Comm.receive (
         array1,          // data
         4,               // data size
         1 - Comm.rank(), // sending stage rank
         3);              // tag

   Comm.wait_for_receive (
         1 - Comm.rank(), // sending stage rank
         3);              // tag

   Comm.wait_for_send (
         1 - Comm.rank(), // receiving stage rank
         3);              // tag

   std::cout << __FILE__ << ": local rank " << Comm.rank() << " received data from this stage: ";
   for (int ind = 0; ind < 4; ind++)
   {
      std::cout << array1[ind] << ", ";
   }
   std::cout << std::endl;

   delete[] array0;
   delete[] array1;

   return 0;
}
