#include "comm.h"
#include "special_tags.h"
#include <iostream>

namespace comm {

   COMM3D::COMM3D (int argc, char *argv[], const CONFIG &config, const unsigned int numStages, const unsigned int thisStageNum_in, const unsigned int tiles[3]) : COMM2d (argc, argv, config, numStages, thisStageNum_in, tiles)
   {
      numTilesDim2 = tiles[2];

      // initialize the number of communication handles to zero
      numCommSendDim2Handles = 0;

      // initialize the communication handles to null
      dimension2Comm = MPI_COMM_NULL;

      for (int index = 0; index < MAX_TAGS; index++) tagsDim2[index] = INVALID_TAG;
      for (int index = 0; index < MAX_TAGS; index++) sendingToDim2[index] = false;
      for (int index = 0; index < MAX_TAGS; index++) receivingFromDim2[index] = false;

      // get the rank of this process within this stage
      int localRank;
      MPI_Comm_rank (stageComms[thisStageNum], &localRank);

      int numTilesDim0and1 = tiles[0] * tiles[1];

      // establish the intra communicator for dimension 2
      const int tiles2Rankings = localRank % numTilesDim0and1;
      int tiles2GroupRanks[tiles[2]];
      for (unsigned int rank = 0tiles2Rankings, ind = 0; ind < tiles[2]; ind++, rank += numTilesDim0and1) {
         tiles2GroupRanks[ind] = rank;
      }

      // produce new group for all processes that share the ssame coordinates in the first two dimensions
      // with rankings in ascending order
      MPI_Gropu tiles2Group;
      MPI_Group_incl (stgeGroups[thisStageNum], tiles[2], tiles2GroupRanks, &tiles2Group);

      // create the third dimension communicatrion handle
      MPI_Comm_create (stageComms[thisStageNum], tiles2Group, &dimension2Comm);
   }

}
