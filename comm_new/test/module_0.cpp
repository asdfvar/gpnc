#include "comm.h"
#include <iostream>

int main (int argc, char *argv[])
{

   comm::CONFIG configuration;

   configuration.numStages      = 2;
   configuration.numProcs[0]    = 1;
   configuration.numProcs[1]    = 1;
   configuration.assocStages[0] = 1;
   configuration.numAssocStages = 1;

   comm::COMM Comm (&argc, &argv, configuration, 2, 0);

   float *array = new float[4];

   array[0] = 3.1415926f;
   array[1] = 2.7182818f;
   array[2] = -78.23143f;
   array[3] = 7.1132894f;

   Comm.send_to_stage (array, 4, 1, 0, 0);

   Comm.wait_for_send_to_stage (1, 0, 0);

   delete[] array;

   return 0;
}
