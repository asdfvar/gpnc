#include "comm.h"
#include <iostream>
#include <cmath>

#define INTERCOMM_TAG 1

namespace comm {

/*
** constructor name: COMM
**
** parent communicator class. Establishes the MPI initialization, destruction,
** and configurations that all processes must call
*/
COMM::COMM (
      int                *argc,
      char               ***argv,
      const int numStages,
      const int thisStageNum_in)
{

   int requested = MPI_THREAD_MULTIPLE;
   int provided;

   MPI_Init_thread (argc, argv, requested, &provided);

   int worldRank;
   MPI_Comm_rank (MPI_COMM_WORLD, &worldRank);

   if (provided != requested && worldRank == 0)
   {
      if (provided == MPI_THREAD_SINGLE) {
         // only one thread will execute
         std::cout << "provided level of MPI support is MPI_THREAD_SINGLE" << std::endl;
      }
      else if (provided == MPI_THREAD_FUNNELED) {
         // only the main thread will make MPI calls
         std::cout << "provided level of MPI support is MPI_THREAD_FUNNELED" << std::endl;
      }
      else if (provided == MPI_THREAD_SERIALIZED) {
         // any thread can make MPI calls but only serialized
         std::cout << "provided level of MPI support is MPI_THREAD_SERIALIZED" << std::endl;
      }
      else if (provided == MPI_THREAD_MULTIPLE) {
         // any thread can make MPI calls at any time
         std::cout << "provided level of MPI support is MPI_THREAD_MULTIPLE" << std::endl;
      }
   }

   thisStageNum  = thisStageNum_in;

   numStageProcs = new int[numStages];

   // determine the hash mapping from the world rank to its associated stage number
   int worldSize;
   MPI_Comm_size (MPI_COMM_WORLD, &worldSize);
   worldStages = new int[worldSize];

   // set this stage number
   worldStages[worldRank] = thisStageNum;

   // send this stage number to all other ranks and receive their stage numbers as well
   for (int rank = 0; rank < worldSize; rank++)
   {
      if (rank == worldRank) continue;

      MPI_Request sendRequest, recvRequest;

      MPI_Issend (&worldStages[worldRank], 1, MPI_INT, rank, 1, MPI_COMM_WORLD, &sendRequest);
      MPI_Irecv  (&worldStages[rank],      1, MPI_INT, rank, 1, MPI_COMM_WORLD, &recvRequest);
      MPI_Wait   (&sendRequest, MPI_STATUS_IGNORE);
      MPI_Wait   (&recvRequest, MPI_STATUS_IGNORE);
   }

   // setup the number of stage processes hashing
   for (int stage = 0; stage < numStages; stage++)
   {
      numStageProcs[stage] = 0;
      for (int rank = 0; rank < worldSize; rank++)
      {
         if (worldStages[rank] == stage) numStageProcs[stage]++;
      }
   }

   for (int stage = 0; stage < numStages; stage++) {
      numSendToStageHandles.push_back (0);
   }

   for (int stage = 0; stage < numStages; stage++) {
      numReceiveFromStageHandles.push_back (0);
   }

   for (int stage = 0; stage < numStages; stage++) {
      sendToStageRequests.push_back (std::vector< std::vector<MPI_Request*> >());
      receiveFromStageRequests.push_back (std::vector< std::vector<MPI_Request*> >());
   }

   tagsFromStage.reserve (numStages);
   tagsToStage.reserve (numStages);

   // set initial identifying tag numbers to invalid
   for (int stageIndex = 0; stageIndex < numStages; stageIndex++) {
      tagsToStage.push_back (std::vector<int>());
      tagsFromStage.push_back (std::vector<int>());
   }

   stageGroups.reserve (numStages);
   for (int stage = 0; stage < numStages; stage++) {
      MPI_Group *group = new MPI_Group;
      *group = MPI_GROUP_NULL;
      stageGroups.push_back (group);
   }

   stageComms.reserve (numStages);
   for (int stage = 0; stage < numStages; stage++) {
      MPI_Comm *comm = new MPI_Comm;
      *comm = MPI_COMM_NULL;
      stageComms.push_back (comm);
   }

   // get the world group from the world communicator
   MPI_Group worldGroup = MPI_GROUP_NULL;
   MPI_Comm_group (MPI_COMM_WORLD, &worldGroup);

   int maxRanks = 0;
   for (int stage = 0; stage < numStages; stage++) if (numStageProcs[stage] > maxRanks) maxRanks = numStageProcs[stage];

   int *worldStageRanks = new int[maxRanks];

   /*
   ** setup intra-communication configurations for all stages
   */
   int worldStageStartRank = 0;
   for (int stage = 0; stage < numStages; stage++)
   {
      for (int rank = worldStageStartRank, ind = 0; ind < numStageProcs[stage]; ind++, rank++)
      {
         worldStageRanks[ind] = rank;
      }

      // create the group for the associated stage
      MPI_Group_incl (worldGroup, numStageProcs[stage], worldStageRanks, stageGroups[stage]);

      // create the intra communicator for the associated stage
      MPI_Comm_create (MPI_COMM_WORLD, *stageGroups[stage], stageComms[stage]);

      worldStageStartRank += numStageProcs[stage];
   }

   delete[] worldStageRanks;

   // define the local rank within this stage
   MPI_Comm_rank (*stageComms[thisStageNum], &localRank);

   /*
   ** setup inter-communication configurations for all stages
   */
   interComms.reserve (numStages);
   for (int stage = 0; stage < numStages; stage++) {
      MPI_Comm *comm = new MPI_Comm;
      *comm = MPI_COMM_NULL;
      interComms.push_back (comm);
   }

   MPI_Comm unionStagesComms[200][200];

   for (int fromStage = 0; fromStage < numStages; fromStage++)
   {
      for (int toStage = fromStage + 1; toStage < numStages; toStage++)
      {
         MPI_Group unionStagesGroup;

         // union groups between associated stages in ascending order
         MPI_Group_union (*stageGroups[fromStage], *stageGroups[toStage], &unionStagesGroup);

         // create the intra communicator between the associated stages
         MPI_Comm_create (MPI_COMM_WORLD, unionStagesGroup, &unionStagesComms[fromStage][toStage]);
      }
   }

   // setup intercommunicators between stages
   for (int toStage = 0; toStage < numStages; toStage++)
   {
      if (thisStageNum == toStage) continue;

      // create inter communicator to the associated stage. the communicator is created from the unioned group
      // between this stage and the associated stage in ascending numerical order hence the starting rank
      // must be relative to where the assocated stage starting rank will be from this stage
      int startRank;
      int lowStage, highStage;
      if (thisStageNum < toStage) {
         startRank = numStageProcs[thisStageNum];
         lowStage  = thisStageNum;
         highStage = toStage;
      } else {
         startRank = 0;
         lowStage  = toStage;
         highStage = thisStageNum;
      }

      MPI_Intercomm_create (*stageComms[thisStageNum], 0, unionStagesComms[lowStage][highStage], startRank, INTERCOMM_TAG, interComms[toStage]);
   }

   // free no longer needed group handles
   if (worldGroup != MPI_GROUP_NULL) MPI_Group_free (&worldGroup);

   // ensure all processes get here before proceeding
   MPI_Barrier (MPI_COMM_WORLD);

} // COMM

/*
** destructor name: COMM
*/
COMM::~COMM (void)
{
   // sync all communicators before cleanup
   MPI_Barrier (MPI_COMM_WORLD);

   // delete the sending request handles
   while (!sendToStageRequests.empty()) {
      while (!sendToStageRequests.back().empty()) {
         while (!sendToStageRequests.back().back().empty()) {
            delete sendToStageRequests.back().back().back();
            sendToStageRequests.back().back().pop_back();
         }
         sendToStageRequests.back().pop_back();
      }
      sendToStageRequests.pop_back();
   }

   // delete the receiving request handles
   while (!receiveFromStageRequests.empty()) {
      while (!receiveFromStageRequests.back().empty()) {
         while (!receiveFromStageRequests.back().back().empty()) {
            delete receiveFromStageRequests.back().back().back();
            receiveFromStageRequests.back().back().pop_back();
         }
         receiveFromStageRequests.back().pop_back();
      }
      receiveFromStageRequests.pop_back();
   }

   while (!tagsToStage.empty()) {
      while (!tagsToStage.back().empty()) {
         tagsToStage.back().pop_back();
      }
      tagsToStage.pop_back();
   }

   while (!tagsFromStage.empty()) {
      while (!tagsFromStage.back().empty()) {
         tagsFromStage.back().pop_back();
      }
      tagsFromStage.pop_back();
   }

   while (!stageGroups.empty()) {
      stageGroups.pop_back();
   }

   while (!interComms.empty()) {
      interComms.pop_back();
   }

   while (!stageComms.empty()) {
      stageComms.pop_back();
   }

   delete[] numStageProcs;

   while (!numSendToStageHandles.empty()) numSendToStageHandles.pop_back();

   while (!numReceiveFromStageHandles.empty()) numReceiveFromStageHandles.pop_back();

   delete[] worldStages;

   // finalize MPI
   MPI_Finalize ();

} // ~COMM

/*
** function name: rank from COMM
*/
int COMM::rank (void) {
   int rank;
   MPI_Group_rank (*stageGroups[thisStageNum], &rank);
   return rank;
}

/*
** function name: send_to_stage from COMM
**
** perform a non-blocking send to the receiving stage
*/
template <typename type>
bool COMM::send_to_stage (type *data, int dataSize, int recvStage, unsigned int recvStageRank, int tag)
{
   // exit if the tag used is invalid
   if (tag == INVALID_TAG) {
      std::cout << tag << " is designated as the invalid tag number" << std::endl;
      return false;
   }

   // exit if there is no stage process associated with this receive
   if (numStageProcs[recvStage] < 1) return false;

   // search for this tag to identify if the communication handle exists
   bool found = false;
   int tagIndex;
   for (tagIndex = 0; tagIndex < numSendToStageHandles[recvStage] && !found; tagIndex++)
   {
      if (tagsToStage[recvStage][tagIndex] == tag) {
         found = true;
         tagIndex--;
      }
   }

   // manage the list of requests and get the appropriate request handle
   MPI_Request* request;
   if (found)
   {
      // create the new request if it doesn't already exist for this rank
      while (recvStageRank >= sendToStageRequests[recvStage][tagIndex].size()) {
         sendToStageRequests[recvStage][tagIndex].push_back (new MPI_Request);
      }

      // retrieve the new, or already existing, request handle
      request = sendToStageRequests[recvStage][tagIndex][recvStageRank];
   }
   else
   {
      // add a new vector of ranks for this tag index
      sendToStageRequests[recvStage].push_back (std::vector <MPI_Request*>());

      // update the tags list
      tagsToStage[recvStage].push_back (tag);
      numSendToStageHandles[recvStage]++;

      // add a new request handle
      for (unsigned int rank = 0; rank <= recvStageRank; rank++)
      {
         sendToStageRequests[recvStage][tagIndex].push_back (new MPI_Request);
      }

      // get the new request handle
      request = sendToStageRequests[recvStage][tagIndex][recvStageRank];
   }

   MPI_Comm *commHandle;

   if (recvStage != thisStageNum) commHandle = interComms[recvStage];
   else                           commHandle = stageComms[thisStageNum];

   // perform the non-blocking send
   if (std::is_same <type, float>::value ) {
      MPI_Issend (data, dataSize, MPI_FLOAT, recvStageRank, tag, *commHandle, request);
   }
   else if (std::is_same <type, double>::value) {
      MPI_Issend (data, dataSize, MPI_DOUBLE, recvStageRank, tag, *commHandle, request);
   }
   else if (std::is_same <type, int>::value) {
      MPI_Issend (data, dataSize, MPI_INT, recvStageRank, tag, *commHandle, request);
   }
   else if (std::is_same <type, char>::value) {
      MPI_Issend (data, dataSize, MPI_CHAR, recvStageRank, tag, *commHandle, request);
   }
   else {
      MPI_Issend (data, dataSize * sizeof (*data), MPI_BYTE, recvStageRank, tag, *commHandle, request);
   }

   return true;
} // send_to_stage

// defined types for send
template bool COMM::send_to_stage (float*  data, int dataSize, int recvStage, unsigned int recvStageRank, int tag);
template bool COMM::send_to_stage (double* data, int dataSize, int recvStage, unsigned int recvStageRank, int tag);
template bool COMM::send_to_stage (int*    data, int dataSize, int recvStage, unsigned int recvStageRank, int tag);
template bool COMM::send_to_stage (char*   data, int dataSize, int recvStage, unsigned int recvStageRank, int tag);

/*
** function name: wait_for_send_to_stage from COMM
*/
bool COMM::wait_for_send_to_stage (int recvStage, unsigned int stageRank, int tag)
{
   // exit if the tag used is invalid
   if (tag == INVALID_TAG) {
      std::cout << tag << " is designated as the invalid tag number" << std::endl;
      return false;
   }

   // exit if there is no stage process
   if (numStageProcs[recvStage] < 1) return false;

   // search for this tag to identify the associated communication handle
   bool found = false;
   int tagIndex;
   for (tagIndex = 0; tagIndex < numSendToStageHandles[recvStage] && !found; tagIndex++)
   {
      if (tagsToStage[recvStage][tagIndex] == tag) {
         found = true;
         tagIndex--;
      }
   }

   // ensure that the associated send call has been made
   if (!found) {
      std::cout << "send to module " << recvStage << " from module " << thisStageNum << " request not found" << std::endl;
      return false;
    }

   // perform the blocking wait
   MPI_Wait (sendToStageRequests[recvStage][tagIndex][stageRank], MPI_STATUS_IGNORE);

   return true;
}  // wait_for_send_to_stage

/*
** function name: receive_from_stage from COMM
**
** receive data from another stage at its local rank
*/
template <typename type>
bool COMM::receive_from_stage (type* data, int dataSize, int sendStage, unsigned int sendStageRank, int tag)
{
   // exit if the tag used in invalid
   if (tag == INVALID_TAG) {
      std::cout << tag << " is designated as the invalid tag number" << std::endl;
      return false;
   }

   // exit if there is no previous stage process
   if (numStageProcs[sendStage] < 1) return false;

   // search for this tag to identify if the communication handle exists
   bool found = false;
   int tagIndex;
   for (tagIndex = 0; tagIndex < numReceiveFromStageHandles[sendStage] && !found; tagIndex++)
   {
      if (tagsFromStage[sendStage][tagIndex] == tag) {
         found = true;
         tagIndex--;
      }
   }

   // retrieve or get the new request handle
   MPI_Request *request;

   if (found)
   {
      // create the new request if it doesn't already exist for this rank
      while (sendStageRank >= receiveFromStageRequests[tagIndex].size()) {
         receiveFromStageRequests[sendStage][tagIndex].push_back (new MPI_Request);
      }

      // retrieve the new, or already existing, reqeust handle
      request = receiveFromStageRequests[sendStage][tagIndex][sendStageRank];
   }
   else
   {
      // add a new vector of ranks for this tag index
      receiveFromStageRequests[sendStage].push_back (std::vector <MPI_Request*>());

      // update the tags list
      tagsFromStage[sendStage].push_back (tag);
      numReceiveFromStageHandles[sendStage]++;

      // add a new request handle associated with this rank
      for (unsigned int rank = 0; rank <= sendStageRank; rank++)
      {
         receiveFromStageRequests[sendStage][tagIndex].push_back (new MPI_Request);
      }

      // get the new request handle
      request = receiveFromStageRequests[sendStage][tagIndex][sendStageRank];
   }

   MPI_Comm *commHandle;

   if (sendStage != thisStageNum) commHandle = interComms[sendStage];
   else                           commHandle = stageComms[thisStageNum];

   // receive from stage 1
   if (std::is_same <type, float>::value ) {
      MPI_Irecv (data, dataSize, MPI_FLOAT, sendStageRank, tag, *commHandle, request);
   }
   else if (std::is_same <type, double>:: value) {
      MPI_Irecv (data, dataSize, MPI_DOUBLE, sendStageRank, tag, *commHandle, request);
   }
   else if (std::is_same <type, int>:: value) {
      MPI_Irecv (data, dataSize, MPI_INT, sendStageRank, tag, *commHandle, request);
   }
   else if (std::is_same <type, char>:: value) {
      MPI_Irecv (data, dataSize, MPI_CHAR, sendStageRank, tag, *commHandle, request);
   }
   else {
      MPI_Irecv (data, dataSize * sizeof (*data), MPI_BYTE, sendStageRank, tag, *commHandle, request);
   }

   return true;

} // receive_from_stage

// defined types for receive
template bool COMM::receive_from_stage (float  *data, int dataSize, int sendStage, unsigned int sendStageRank, int tag);
template bool COMM::receive_from_stage (double *data, int dataSize, int sendStage, unsigned int sendStageRank, int tag);
template bool COMM::receive_from_stage (int    *data, int dataSize, int sendStage, unsigned int sendStageRank, int tag);
template bool COMM::receive_from_stage (char   *data, int dataSize, int sendStage, unsigned int sendStageRank, int tag);

/*
** function name: wait_for_receive_from_stage from COMM
*/
bool COMM::wait_for_receive_from_stage (int sendStage, int sendStageRank, int tag)
{
   // exit if the tag used is invalid
   if (tag == INVALID_TAG) {
      std::cout << tag << " is designated as the invalid tag number" << std::endl;
      return false;
   }

   // exit if there is no stage-1 process
   if (numStageProcs[sendStage] < 1) return false;

   // status variable
   int stat = 0;

   // search for this tag to identify the associated communication handle
   bool found = false;
   int tagIndex;
   for (tagIndex = 0; tagIndex < numReceiveFromStageHandles[sendStage] && !found; tagIndex++)
   {
      if (tagsFromStage[sendStage][tagIndex] == tag) {
         found = true;
         tagIndex--;
      }
   }

   if (!found) {
      std::cout << "receive from " << sendStage << " request not found" << std::endl;
      return false;
   }

   // perform the blocking wait
   stat = MPI_Wait (receiveFromStageRequests[sendStage][tagIndex][sendStageRank], MPI_STATUS_IGNORE);

   if (stat != MPI_SUCCESS) {
      std::cout << "error with waiting for receive request from " << sendStageRank << std::endl;
   }

   return true;
} // wait_for_receive_from_stage

} // namespace comm
