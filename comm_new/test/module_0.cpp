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
   configuration.assocStages[0] = 1;
   configuration.numAssocStages = 1;

   comm::COMM Comm (&argc, &argv, configuration, NUM_STAGES, 0);

   float *array = new float[4];

   array[0] = 3.1415926f;
   array[1] = 2.7182818f;
   array[2] = -78.23143f;
   array[3] = 7.1132894f;

   Comm.send_to_stage (
      array, // data
      4,     // data size
      1,     // receiving stage
      0,     // receiving stage rank
      0);    // tag

   Comm.wait_for_send_to_stage (
      1,     // receiving stage
      0,     // receiving stage rank
      0);    // tag

   std::cout << __FILE__ << ":sent data: ";
   for (int ind = 0; ind < 4; ind++)
   {
      std::cout << array[ind] << ", ";
   }
   std::cout << std::endl;

   Comm.receive_from_stage (
      array, // data
      4,     // data size
      1,     // sending stage
      0,     // sending stage rank
      1);    // tag

   Comm.wait_for_receive_from_stage (
      1,     // sending stage
      0,     // sending stage rank
      1);    // tag

   std::cout << __FILE__ << ":received data: ";
   for (int ind = 0; ind < 4; ind++)
   {
      std::cout << array[ind] << ", ";
   }
   std::cout << std::endl;

   delete[] array;

   return 0;
}
