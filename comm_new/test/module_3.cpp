#include "comm.h"
#include <iostream>
#include "configuration.h"

int main (int argc, char *argv[])
{

   #include "numStageProcs.i"
   const unsigned int tiles[2] = {2, 3};

//   comm::COMM2D Comm (&argc, &argv, numStageProcs, NUM_STAGES, 3, tiles);
   comm::COMM Comm (&argc, &argv, numStageProcs, NUM_STAGES, 3);

   return 0;
}
