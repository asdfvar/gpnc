#include "comm.h"

int main (int argc, char *argv[])
{

   comm::CONFIG configuration;

   configuration.numStages      = 2;
   configuration.numProcs[0]    = 1;
   configuration.numProcs[1]    = 1;
   configuration.assocStages[0] = 0;
   configuration.numAssocStages = 1;

   comm::COMM Comm (&argc, &argv, configuration, 2, 1);

   return 0;
}
