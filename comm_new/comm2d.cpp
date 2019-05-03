#include "comm.h"

namespace comm {

COMM2D::COMM2D (
      int                *argc,
      char               **argv[],
      const unsigned int *numStageProcs,
      const unsigned int numStages,
      const unsigned int thisStageNum_in,
      const unsigned int tiles[2])
   : COMM (argc, argv, numStageProcs, numStages, thisStageNum_in)
{
   numTilesDim0 = tiles[0];
   numTilesDim1 = tiles[1];

#if 0
   // initialize the communication handles to null
   dimensions0and1Comm = MPI_COMM_NULL;

   // initialize the number of communication handles to zero
   numCommSendDim0and1Handles = 0;

   // initialize status flags

   // get the rank of this process within this stage
   int localRank;
   MPI_Comm_rank (stageComms[thisStageNum], &localRank);

   int numTilesDim0and1 = tiles[0] * tiles[1];

   // establish the tiles 0 & 1 group and its corresponding intra-communicator from the group of all processes within this stage. Partitions within each dim2 will be able to communicate with each other with this communicator handle but not across dim2
   const int tiles0and1dim2 = localRank / numTilesDim0and1;
   const int tiles0a1Rankings = tiles0and1dim2 * numTilesDim0and1;
   int tiles0and1GroupRankings[numTilesDim0and1];
   for (int rank = tiles0a1Rankings, ind = 0; ind < numTilesDim0and1; ind++, rank++)
   {
      tiles0and1GroupRankings[ind] = rank;
   }

   MPI_Group tiles0and1Group;
   MPI_Group_incl (stageGroups[thisStageNum], numTilesDim0and1, tiles0and1GroupRankings, &tiles0and1Group);

   // create communication handle for dimensions 0 and 1
   MPI_Comm_create (stageComms[thisStageNum], tiles0and1Group, &dimensions0and1Comm);

   // get the rank for dimensions 0 and 1 communicator
   MPI_Comm_rank (dimensions0and1Comm, &dimensions0and1Rank);

   // free no longer needed group handles
   MPI_Group_free (&tiles0and1Group);
#endif

}

COMM2D::~COMM2D (void)
{
   // pass
}

template <typename type>
bool COMM2D::send_up (type *data, int dataSize, int tag) {

   bool stat = true;

   int receiveRank = (localRank + numTilesDim0) % (numTilesDim0 * numTilesDim1);

   stat |= send (data, dataSize, receiveRank, tag);

   return stat;
}

template bool COMM2D::send_up (float*  data, int dataSize, int tag);
template bool COMM2D::send_up (double* data, int dataSize, int tag);
template bool COMM2D::send_up (int*    data, int dataSize, int tag);
template bool COMM2D::send_up (char*   data, int dataSize, int tag);

template <typename type>
bool COMM2D::receive_down (type *data, int dataSize, int tag) {

   bool stat = true;

   int sendRank = (localRank - numTilesDim0);
   if (sendRank < 0) sendRank += numTilesDim0 * numTilesDim1;

   stat |= receive (data, dataSize, sendRank, tag);

   return stat;
}

template bool COMM2D::receive_down (float*  data, int dataSize, int tag);
template bool COMM2D::receive_down (double* data, int dataSize, int tag);
template bool COMM2D::receive_down (int*    data, int dataSize, int tag);
template bool COMM2D::receive_down (char*   data, int dataSize, int tag);

bool COMM2D::wait_for_send_up (int tag) {
   return wait_for_send ((localRank + numTilesDim0) % (numTilesDim0 * numTilesDim1), tag);
}

bool COMM2D::wait_for_receive_down (int tag)
{
   int targetRank = localRank - numTilesDim0;
   if (targetRank < 0) targetRank += numTilesDim0 * numTilesDim1;

   return wait_for_receive (targetRank, tag);
}

} // namespace comm
