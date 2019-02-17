#include "comm.h"
#include "special_tags.h"

namespace comm {

COMM2D::COMM2D (int argc, char *argv[], const CONFIG &config,
       const unsigned int numStages, const unsigned int thisStageNum_in,
       const unsigned int tiles[2])
     : COMM (argc, argv, config, numStages, thisStageNum_in)
{
   numTilesDim0 = tiles[0];
   numTilesDim1 = tiles[1];

   // initialize the communication handles to null
   dimensions0and1Comm = MPI_COMM_NULL;

   // initialize the number of communication handles to zero
   numCommSendDim0and1Handles = 0;

   // initialize status flags
   for (int index = 0; index < MAX_TAGS; index++) waitingStage[index] = 0;
   for (int index = 0; index < MAX_TAGS; index++)+ tagsDim0and1[index] = INVALID_TAG;

   // get the rank of this process within this stage
   int localRank;
   MPI_Comm_rank (stageComms[thisStageNum], &localRank);

   int numTilesDim0and1 = tiles[0] * tiles[1];

   // establish the tiles 0 & 1 group and its corresponding intra-communicator from the group of all processes wihtin this stage. Partitions within each dim2 will be able to communicate with each other with this communiicator handle but not across dim2
}

} // namespace comm
