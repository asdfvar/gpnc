#include "comm.h"
#include <iostream>
#include "configuration.h"

int main (int argc, char *argv[])
{

   #include "numStageProcs.i"

   comm::COMM Comm (&argc, &argv, numStageProcs, NUM_STAGES, 2);

   float *array = new float[4];

   array[0] = 0.0f;
   array[1] = 0.0f;
   array[2] = 0.0f;
   array[3] = 0.0f;

   if (Comm.rank() == 1) {
      Comm.receive_from_stage (
            array, // data
            4,     // data size
            0,     // sending stage
            0,     // sending stage rank
            2);    // tag

      Comm.wait_for_receive_from_stage (
            0,     // sending stage
            0,     // sending stage rank
            2);    // tag

      std::cout << __FILE__ << ":received data from stage 0: ";
      for (int ind = 0; ind < 4; ind++)
      {
         std::cout << array[ind] << ", ";
      }
      std::cout << std::endl;
   }

   delete[] array;

   return 0;
}
