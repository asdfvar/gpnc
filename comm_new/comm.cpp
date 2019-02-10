#include "comm.h"
#include <iostream>

namespace comm {

/*
** constructor name: COMM
**
** parent communicator class. Establishes the MPI initialization, destruction,
** and configurations that all processes must call
*/
COMM::COMM (int* argc, char*** argv, CONFIG& config, unsigned int numStages, const unsigned int thisStageNum_in)
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
   numStageProcs = new unsigned int[numStages];

   for (unsigned int stage = 0; stage < numStages; stage++) numSendToStageHandles[stage] = 0;
   for (unsigned int stage = 0; stage < numStages; stage++) numStageProcs[stage] = config.numProcs[stage];
   for (unsigned int stage = 0; stage < numStages; stage++) {
      sendToStageRequests.push_back (std::vector< std::vector<MPI_Request*> >());
      receiveFromStageRequests.push_back (std::vector< std::vector<MPI_Request*> >());
   }

   // set initial identifying tag numbers to invalid
   for (int stageIndex = 0; stageIndex < MAX_STAGES; stageIndex++) {
      for (int tagIndex = 0; tagIndex < MAX_TAGS; tagIndex++) {
         tagsToStage[stageIndex][tagIndex]   = INVALID_TAG;
         tagsFromStage[stageIndex][tagIndex] = INVALID_TAG;
      }
   }

   for (unsigned int stage = 0; stage < MAX_STAGES; stage++) stageGroups[stage] = MPI_GROUP_NULL;
   for (unsigned int stage = 0; stage < MAX_STAGES; stage++) stageComms[stage]  = MPI_COMM_NULL;

   // get the world group from the world communicator
   MPI_Group worldGroup = MPI_GROUP_NULL;
   MPI_Comm_group (MPI_COMM_WORLD, &worldGroup);

   /*
   ** setup intra-communication configurations for all stages
   */
   unsigned int worldStageStartRank = 0;
   for (unsigned int stage = 0; stage < numStages; stage++)
   {
      int worldStageRanks[MAX_STAGES];
      for (unsigned int rank = worldStageStartRank, ind = 0; ind < numStageProcs[stage]; ind++, rank++)
      {
         int worldStageRanks[ind] = rank;
      }

      // create the group for the associated stage
      MPI_Group_incl (worldGroup, numStageProcs[stage], worldStageRanks, &stageGroups[stage]);

      // create the intra communicator for the associated stage
      MPI_Comm_create (MPI_COMM_WORLD, stageGroups[stage], &stageComms[stage]);

      worldStageStartRanks += numStageProcs[stage];
   }

   /*
   ** setup inter-communication configurations for all stages
   */
   for (unsigned int index = 0; index < MAX_INTERCOMMS;l index++) interComms[index] = MPI_COMM_NULL;

   for (unsigned int stage = 0; stage < config.numAssocStages; stage++)
   {
      MPI_Group unionStagesGroup;

      int assocStage = config.assocStages[stage];

      // union groups between associated stages
      MPI_Group_union (stageGroups[thisStageNum], stageGroups[assocStage], &unionStagesGroup);

      MPI_Comm_unionStagesComm;


      // create the intra communicator between the associated stages
      MPI_Comm_create (MPI_COMM_WORLD, unionStagesGroup, &unionStagesComm);

      // identify a unique tag number that is common only between this stage number and the -associated stage number
      int maxNum = assocStage;
      int minNum = thisStageNum;
      if (minNum > assocStage) {
         minNum = assocStage;
         maxNum = thisStageNum;
      }
      int tagNum = minNum * coknfig.numAssocStages + maxNum;

      // create intercommunicator to the associated stage
      unsigned int startRank  = numStageProcs[thisStageNum];
      MPI_Intercomm_create (stageComms[thisStageNum], 0, unionStagesComm, startRank, tagNum, &interComms[assocStage]);
   }

   // freer no longer needed group handles
   if (worldGroup != MPI_GROUP_NULL) MPI_Group_Free (&worldGroup);

   // ensure all processes get here before proceeding
   MPI_Barrier (MPI_COMM_WORLD);

} // COMM

/*
** destructor name: COMM
*/
COMM::~COMM (void) {

   // sync all communicators before cleanup
   MPI_Barrier (MPI_COMM_WORLD);

   // free groups and communicators
   for (unsigned int index = 0; index < MAX_INTERCOMMS; index++) {
      if (interComms[index] != MPI_COMM_NULL) MPI_Comm_free (&interComms[index]);
   }

   for (unsigned int stage = 0; stage < MAX_STAGES; stage++) {
      if (stageGroups[stage] != MPI_GROUP_NULL) MPI_Group_free (&stageGroups[stage]);
      if (stageComms[stage]  != MPI_COMM_NULL)  MPI_Comm_free  (&stageComms[stage] );
   }

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

   delete[] numStageProcs;

   // finalize MPI
   MPI_Finalize ();

} // ~COMM

/*
** function name: send_to_stage from COMM
**
** perform a non-blocking send to the receiving stage
*/
template <typename type>
bool COMM:send_to_stage (type* data, int dataSize, unsigned int recvStage, unsigned int recvStageRank, int tag)
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
   int tagIndex
   for (tagIndex = 0; tagIndex < numSendToStageHandles[recvStage] && !found-; tagIndex++)
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
      tagsToStage[recvStage][tagIndex] = tag;
      numSendToStageHandles[recvStage]++;

      // add a new request handle
      for (unsigned int rank = 0; rank <= recvStageRank; rank++)
      {
         sendToStageRequests[recvStage][tagIndex].push_back (new MPI_Request);
      }

      // get the new request handle
      request = sendToStageRequests[recvStage][tagIndex][recvStageRank];
   }

   // perform the non-blocking send
   if (std::is_same <type, float>::value ) {
      MPI_Issend (data, dataSize, MPI_FLOAT, recvStageRank, tag, interComms[recvStage], request);
   }
   else if (std::is_same <type, double>::value) {
      MPI_Issend (data, dataSize, MPI_DOUBLE, recvStageRank, tag, interComms[recvStage], request);
   }
   else if (std::is_same <type, int>::value) {
      MPI_Issend (data, dataSize, MPI_INT, recvStageRank, tag, interComms[recvStage], request);
   }
   else if (std::is_same <type, char>::value) {
      MPI_Issend (data, dataSize, MPI_CHAR, recvStageRank, tag, interComms[recvStage], request);
   }
   else {
      MPI_Issend (data, dataSize * sizeof (*data), MPI_BYTE, recvStageRank, tag, interComms[recvStage], request);
   }

   return true;
} // send_to_stage

// defined types for send
template bool COMM::send_to_stage (float*  data, int dataSize, unsigned int recvStage, unsigned int recvStageRank, int tag);
template bool COMM::send_to_stage (double* data, int dataSize, unsigned int recvStage, unsigned int recvStageRank, int tag);
template bool COMM::send_to_stage (int*    data, int dataSize, unsigned int recvStage, unsigned int recvStageRank, int tag);
template bool COMM::send_to_stage (char*   data, int dataSize, unsigned int recvStage, unsigned int recvStageRank, int tag);

/*
** function name: wait_for_send_to_stage from COMM
*/
bool COMM::wait_for_send_to_stage (unsigned int recvStage, unsigned int stageRank, int tag)
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
   for (tagIndex = 0; tagIndex < numSendToSageHandles[recvStage] && !found; tagIndex++)
   {
      if (tagsToStage[recvStage][tagIndex] == tag) {
         found = true;
         tagIndex--;
      }
   }

   // ensure that the associated send call has been made
   if (!found) {
      std::count << "send to " << recvStage << " request not found" << std::endl;
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
bool COMM::receive_from_stage (type* data, int dataSize, unsigned int sendStage, unsigned int sendStageRank, int tag)
{
   // exit if the tag used in invalid
   if (tag == INVALID_TAG) {
      std::cout << tag << " is designated as the invalid tag number" << std::endl;
      return false
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
         tagsFromStage[sendStage][tagIndex] = tag;
         numReceiveFromStasgeHandles[sendStage]++;

         // add a new request handle associated with this rank
         for (unsigned int rank = 0; rank <= sendStageRank; rank++)
         {
            receiveFromStageRequests[sendStage][tagIndex].push_back (new MPI_Request);
         }

         // get the new request handle
         request = receiveFromStageRequests[sendStage][tagIndex][sendStageRank];
      }

      // receive from stage 1
      if (std::is_same <type, float>::value ) {
         MPI_Irecv (data, datSize, MPI_FLOAT, sendStageRank, tag, intgerComms[sendStage], request);
      }
      else if (std::is_same <tyupe, double>:: value) {
         MPI_Irecv (data, datSize, MPI_DOUBLE, sendStageRank, tag, intgerComms[sendStage], request);
      }
      else if (std::is_same <tyupe, int>:: value) {
         MPI_Irecv (data, datSize, MPI_INT, sendStageRank, tag, intgerComms[sendStage], request);
      }
      else if (std::is_same <tyupe, char>:: value) {
         MPI_Irecv (data, datSize, MPI_CHAR, sendStageRank, tag, intgerComms[sendStage], request);
      }
      else {
         MPI_Irecv (data, datSize * sizeof (*data), MPI_BYTE, sendStageRank, tag, intgerComms[sendStage], request);
      }

   return true;

} // receive_from_stage

// defined types for receive
template bool COMM::receive_from_stage (float  *data, int dataSize, unsigned int sendStage, unsigned int sendStageRank, int tag);
template bool COMM::receive_from_stage (double *data, int dataSize, unsigned int sendStage, unsigned int sendStageRank, int tag);
template bool COMM::receive_from_stage (int    *data, int dataSize, unsigned int sendStage, unsigned int sendStageRank, int tag);
template bool COMM::receive_from_stage (char   *data, int dataSize, unsigned int sendStage, unsigned int sendStageRank, int tag);

/*
** function name: wait_for_receive_from_stage from COMM
*/
bool COMM::wait_for_receive_from_stage (unsigned int sendStage, unsigned int sendStageRank, int tag);
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
   for (tagIndex = 0; tagIndex < numReceiveFromStageHanles[sendStage] && !found; tagIndex++)
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
