#include "comm.h"
#include <iostream>
#include "configuration.h"

int main (int argc, char *argv[])
{

   #include "numStageProcs.i"
   const unsigned int tiles[2] = {2, 3};

   comm::COMM2D Comm (&argc, &argv, numStageProcs, NUM_STAGES, 3, tiles);

   float array0[4] = {3.14159f, 2.71828f, -7.4325f, 923.5234f};
   float array1[4];

   Comm.send_up (
         array0, // data
         4,      // data size
         0);     // tag

   Comm.receive_down (
         array1, // data
         4,      // data size
         0);     // tag

   Comm.wait_for_send_up (0);

   Comm.wait_for_receive_down (0);

   Comm.send_right (
         array1, // data
         4,      // data size
         1);     // tag

   Comm.receive_left (
         array0, // data
         4,      // data size
         1);     // tag

   Comm.wait_for_send_right (1);

   Comm.wait_for_receive_left (1);

   return 0;
}
