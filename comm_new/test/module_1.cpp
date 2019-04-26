#include "comm.h"
#include <iostream>

int main (int argc, char *argv[])
{

   comm::CONFIG configuration;

   configuration.numStages      = 2;
   configuration.numProcs[0]    = 1;
   configuration.numProcs[1]    = 1;
   configuration.assocStages[0] = 0;
   configuration.numAssocStages = 1;

   comm::COMM Comm (&argc, &argv, configuration, 2, 1);

   float *array = new float[4];

   Comm.receive_from_stage (array, 4, 0, 0, 0);

   Comm.wait_for_receive_from_stage (0, 0, 0);

   delete[] array;

   return 0;
}
