#include "comm.h"
//#include "special_tags.h"
#include <iostream>

namespace comm {

   COMM3D::COMM3D (
         int                *argc,
         char               **argv[],
         const unsigned int *numStageProcs,
         const unsigned int numStages,
         const unsigned int thisStageNum_in,
         const unsigned int tiles[3])
      : COMM2D (argc, argv, numStageProcs, numStages, thisStageNum_in, tiles)
   {
      numTilesDim2 = tiles[2];

      // initialize the number of communication handles to zero
      numCommSendDim2Handles = 0;

      // initialize the communication handles to null
      dimension2Comm = MPI_COMM_NULL;

      // get the rank of this process within this stage
      int localRank;
      MPI_Comm_rank (stageComms[thisStageNum], &localRank);

      int numTilesDim0and1 = tiles[0] * tiles[1];

      // establish the intra communicator for dimension 2
      const int tiles2Rankings = localRank % numTilesDim0and1;
      int tiles2GroupRanks[tiles[2]];
      for (unsigned int rank = tiles2Rankings, ind = 0; ind < tiles[2]; ind++, rank += numTilesDim0and1) {
         tiles2GroupRanks[ind] = rank;
      }

      // produce new group for all processes that share the ssame coordinates in the first two dimensions
      // with rankings in ascending order
      MPI_Group tiles2Group;
      MPI_Group_incl (stageGroups[thisStageNum], tiles[2], tiles2GroupRanks, &tiles2Group);

      // create the third dimension communicatrion handle
      MPI_Comm_create (stageComms[thisStageNum], tiles2Group, &dimension2Comm);

      // get the rank for dimension 2 communicator
      MPI_Comm_rank (dimension2Comm, &dimension2Rank);

      // free no longer needed group handles
      MPI_Group_free (&tiles2Group);
   }

   COMM3D::~COMM3D (void)
   {
      // pass
   }

   /*
    ** function name: send_to_next_dim2 from COMM3D
    */
   template <typename type> bool COMM3D::send_to_next_dim2 (type *data, int src_size, int tag)
   {
#if 0
      // exit if the tag used is invalid
      if (tag == INVALID_TAG) {
         std::cout << tag << " is designated as the invalid tag number" << std::endl;
         return false;
      }

      // search for this tag to identify if the communication handle exists
      bool found = false;
      int index;
      for (index = 0; index < numCommSendDim2Handles && !found; index++)
      {
         if (tagsDim2[index] == tag) {
            found = true;
            index--;
         }
      }

      // set the status flag informing that data is in the process of being sent
      sendingToDim2[index] = true;

      MPI_Request *request;
      if (found) {
         request = localSendRequestsDim2[index];
      }
      else {
         request = new MPI_Request;
         localSendRequestsDim2.push_back (request);
         tagsDim2[numCommSendDim2Handles++] = tag;
      }

      // send to next stage 1 in dimension 2 direction
      int destinationRank = (dimension2Rank + 1) % numTilesDim2;

      if (std::is_same <type, float>::value) {
         MPI_Issend (data, src_size, MPI_FLOAT, destinationRank, tag, dimension2Comm, request);
      }
      else if (std::is_same <type, double>::value) {
         MPI_Issend (data, src_size, MPI_DOUBLE, destinationRank, tag, dimension2Comm, request);
      }
      else if (std::is_same <type, int>::value) {
         MPI_Issend (data, src_size, MPI_INT, destinationRank, tag, dimension2Comm, request);
      }
      else if (std::is_same <type, char>::value) {
         MPI_Issend (data, src_size, MPI_CHAR, destinationRank, tag, dimension2Comm, request);
      }
      else {
         MPI_Issend (data, src_size * sizeof (*data), MPI_BYTE, destinationRank, tag, dimension2Comm, request);
      }

#endif
      return true;
   } // send_to_next_dim2 

   // defined types
   template bool COMM3D::send_to_next_dim2 (float  *data, int src_size, int tag);
   template bool COMM3D::send_to_next_dim2 (double *data, int src_size, int tag);
   template bool COMM3D::send_to_next_dim2 (int    *data, int src_size, int tag);
   template bool COMM3D::send_to_next_dim2 (char   *data, int src_size, int tag);

   /*
   ** function name: wait_for_send_to_next_dim2 from COMM3D
   */
   bool COMM3D::wait_for_send_to_next_dim2 (int tag)
   {
#if 0
      // exit if the tag used is invalid
      if (tag == INVALID_TAG) {
         std::cout << tag << " is designated as the invalid tag number" << std::endl;
         return false;
      }
   
      // search for this tag to identify the associated communication handle
      bool found = false;
      int tagIndex;
      for (tagIndex = 0; tagIndex < numCommSendDim2Handles && !found; tagIndex++)
      {
         if (tagsDim2[tagIndex] == tag) {
            found = true;
            tagIndex--;
         }
      }
   
      // ensure that the associated send call has been made
      if (!found) {
         std::cout << "send to next request not found" << std::endl;
         return false;
       }
   
      // perform the blocking wait
      MPI_Wait (localSendRequestsDim2[tagIndex], MPI_STATUS_IGNORE);
   
#endif
      return true;
   }  // wait_for_send_to_stage

   /*
    ** function name: receive_from_previous_dim2 from COM3D
    */
   template <typename type> bool COMM3D::receive_from_previous_dim2 (type *data, int src_size, int tag)
   {
#if 0
      // exit if the tag used is invalid
      if (tag == INVALID_TAG) {
         std::cout << tag << " is designated as the invalid tag number" << std::endl;
         return false;
      }

      // search for this tag to identify if the communication handle exists
      bool found = false;
      int index;
      for (index = 0; index < numCommSendDim2Handles && !found; index++)
      {
         if (tagsDim2[index] == tag) {
            found = true;
            index--;
         }
      }

      // set the status flag informing that data is ikn the process of being sent
      receivingFromDim2[index] = true;

      MPI_Request *request;
      if (found) {
         request = localReceiveRequestsDim2[index];
      }
      else {
         request = new MPI_Request;
         localReceiveRequestsDim2.push_back (request);
         tagsDim2[numCommSendDim2Handles++] = tag;
      }

      int sourceRank = dimension2Rank - 1;
      if (sourceRank < 0) sourceRank += numTilesDim2;

      if (std::is_same <type, float>::value) {
         MPI_Irecv (data, src_size, MPI_FLOAT, sourceRank, tag, dimension2Comm, request);
      }
      else if (std::is_same <type, double>::value) {
         MPI_Irecv (data, src_size, MPI_DOUBLE, sourceRank, tag, dimension2Comm, request);
      }
      else if (std::is_same <type, int>::value) {
         MPI_Irecv (data, src_size, MPI_INT, sourceRank, tag, dimension2Comm, request);
      }
      else if (std::is_same <type, char>::value) {
         MPI_Irecv (data, src_size, MPI_CHAR, sourceRank, tag, dimension2Comm, request);
      }
      else {
         MPI_Irecv (data, src_size * sizeof (*data), MPI_CHAR, sourceRank, tag, dimension2Comm, request);
      }

#endif
      return true;
   }

// defined types
template bool COMM3D::receive_from_previous_dim2 (float  *data, int src_size, int tag);
template bool COMM3D::receive_from_previous_dim2 (double *data, int src_size, int tag);
template bool COMM3D::receive_from_previous_dim2 (int    *data, int src_size, int tag);
template bool COMM3D::receive_from_previous_dim2 (char   *data, int src_size, int tag);

} // namespace comm
