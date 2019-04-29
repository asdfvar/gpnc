#include "comm.h"
#include <iostream>
#include "configuration.h"

int main (int argc, char *argv[])
{

   comm::CONFIG configuration;

   configuration.numStages      = NUM_STAGES;
   configuration.numProcs[0]    = 1;
   configuration.numProcs[1]    = 1;
   configuration.numProcs[2]    = 6;
   configuration.assocStages[0] = 1;
   configuration.numAssocStages = 1;

   const unsigned int tiles[2] = {2, 3};

   comm::COMM2D Comm (&argc, &argv, configuration, NUM_STAGES, 3, tiles);

   return 0;
}
