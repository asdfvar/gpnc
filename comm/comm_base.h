#ifndef COMM_BASE_H
#define COMM_BASE_H

#include <mpi.h>
#include <vector>

namespace comm {

/*
** class name: COMM
*/
class COMM {

   public:
      COMM (int       *argc,
            char      ***argv,
            const int numStages,
            const int thisStageNum);

      COMM (int       *argc,
            char      ***argv,
            const int numStages,
            const int thisStageNum,
            int       requested);

     ~COMM (void);

     int rank (void);

     template<typename type>
        bool send_to_stage (
              type *data,
              int dataSize,
              int recvStage,
              unsigned int recvStageRank,
              int tag);

      bool wait_for_send_to_stage (
            int recvStage,
            unsigned int recvStageRank,
            int tag);

      template <typename type>
         bool receive_from_stage (
               type* data,
               int dataSize,
               int sendStage,
               unsigned int stageRank,
               int tag);

      bool wait_for_receive_from_stage (
          int sendStage,
          int sendStageRank,
          int tag);

      template<typename type>
         bool send (
               type *data,
               int size,
               int recvStageRank,
               int tag)
         {
            return send_to_stage (data, size, thisStageNum, recvStageRank, tag);
         }

      bool wait_for_send (
            int recvStageRank,
            int tag)
      {
         return wait_for_send_to_stage (thisStageNum, recvStageRank, tag);
      }

      template <typename type>
         bool receive (
               type *data,
               int dataSize,
               int stageRank,
               int tag)
      {
         return receive_from_stage (data, dataSize, thisStageNum, stageRank, tag);
      }

      bool wait_for_receive (
          int sendStageRank,
          int tag)
      {
         return wait_for_receive_from_stage (thisStageNum, sendStageRank, tag);
      }

      template<typename type>
         bool broadcast (
               type *data,
               int  dataSize,
               int  root);

   protected:

      int localRank;
      int *worldStages;
      int *numStageProcs;

      std::vector<MPI_Group*> stageGroups;  // size is number of stages
      std::vector<MPI_Comm*>  stageComms;   // size is number of stages
      std::vector<MPI_Comm*>  interComms;   // size is number of stages

      // accounting
      std::vector< std::vector<int> > tagsToStage; // [stage][tag]
      std::vector< std::vector<int> > tagsFromStage; // [stage][tag]
      int thisStageNum;
      std::vector<int> numSendToStageHandles;
      std::vector<int> numReceiveFromStageHandles;

      // request handles of dimension [stage][tag_index][rank]
      std::vector< std::vector< std::vector<MPI_Request*> > > sendToStageRequests;
      std::vector< std::vector< std::vector<MPI_Request*> > > receiveFromStageRequests;

   private:

      void init (
            int       *argc,
            char      ***argv,
            const int numStages,
            const int thisStageNum,
            int       requested);

};

} // namespace comm
#endif
