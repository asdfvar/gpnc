#include "comm.h"
#include <iostream>
#include "configuration.h"

int main (int argc, char *argv[])
{

   comm::CONFIG configuration;

   configuration.numStages      = NUM_STAGES;
   configuration.numProcs[0]    = 1;
   configuration.numProcs[1]    = 1;
   configuration.numProcs[2]    = 2;
   configuration.assocStages[0] = 0;
   configuration.numAssocStages = 1;

   comm::COMM Comm (&argc, &argv, configuration, NUM_STAGES, 1);

   float *array = new float[4];

   Comm.receive_from_stage (
      array, // data
      4,     // data size
      0,     // sending stage
      0,     // sending stage rank
      0);    // tag

   Comm.wait_for_receive_from_stage (
      0,     // sending stage
      0,     // sending stage rank
      0);    // tag

   std::cout << __FILE__ << ":received data: ";
   for (int ind = 0; ind < 4; ind++)
   {
      std::cout << array[ind] << ", ";
   }
   std::cout << std::endl;

   for (int ind = 0; ind < 4; ind++) array[ind] += 1.0f;

   std::cout << __FILE__ << ":incremented the data then sending it back " << std::endl;

   Comm.send_to_stage (
      array, // data
      4,     // data size
      0,     // receive stage
      0,     // receive stage rank
      1);    // tag

   Comm.wait_for_send_to_stage (
      0,     // receive stage
      0,     // receive stage rank
      1);    // tag

   delete[] array;

   return 0;
}
