#include "comm.h"
#include <iostream>
#include "configuration.h"

int main (int argc, char *argv[])
{

   comm::COMM Comm (&argc, &argv, NUM_STAGES, 0);

   float *array = new float[4];

   int tag = 0;

   array[0] = 3.1415926f;
   array[1] = 2.7182818f;
   array[2] = -78.23143f;
   array[3] = 7.1132894f;

   for (int itt = 0; itt < 20; itt++)
   {
      Comm.send_to_stage (
            array, // data
            4,     // data size
            1,     // receiving stage
            0,     // receiving stage rank
            tag);  // tag

      Comm.wait_for_send_to_stage (
            1,     // receiving stage
            0,     // receiving stage rank
            tag);  // tag
   }

   tag++;

   std::cout << __FILE__ << ": sent data: ";
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
      tag);  // tag

   Comm.wait_for_receive_from_stage (
      1,     // sending stage
      0,     // sending stage rank
      tag);  // tag

   tag++;

   std::cout << __FILE__ << ": received data: ";
   for (int ind = 0; ind < 4; ind++)
   {
      std::cout << array[ind] << ", ";
   }
   std::cout << std::endl;

   Comm.send_to_stage (
      array, // data
      4,     // data size
      2,     // receiving stage
      1,     // receiving stage rank
      tag);  // tag

   Comm.wait_for_send_to_stage (
      2,     // receiving stage
      1,     // receiving stage rank
      tag);  // tag

   std::cout << __FILE__ << ": sent data to stage 2: ";
   for (int ind = 0; ind < 4; ind++)
   {
      std::cout << array[ind] << ", ";
   }
   std::cout << std::endl;

   delete[] array;

   return 0;
}
