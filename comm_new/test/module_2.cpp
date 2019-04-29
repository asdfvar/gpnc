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
   configuration.numAssocStages = 0;

   comm::COMM Comm (&argc, &argv, configuration, NUM_STAGES, 2);

   return 0;
}
