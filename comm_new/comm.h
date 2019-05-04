#ifndef COMM_H
#define COMM_H

#include <mpi.h>
#include <vector>

#define INVALID_TAG -1

namespace comm {

/*
** class name: COMM
*/
class COMM {

   public:
      COMM (int                *argc,
            char               ***argv,
            const int numStages,
            const int thisStageNum);

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
               type* data,
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
               type* data,
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



   protected:

      int localRank;
      int *worldStages;
      int *numStageProcs;

      MPI_Group stageGroups[200];  // size is number of stages
      std::vector<MPI_Comm*>  stageComms;   // size is number of stages
      MPI_Comm  interComms[200];   // size is number of stages

      std::vector<int> toStageInterCommHash;
      std::vector<MPI_Comm*> interComms_new;   // size is number of stages

      // accounting
      std::vector< std::vector<int> > tagsToStage; // [stage][tag]
      std::vector< std::vector<int> > tagsFromStage; // [stage][tag]
      int thisStageNum;
      std::vector<int> numSendToStageHandles;
      std::vector<int> numReceiveFromStageHandles;

      // request handles of dimension [stage][tag_index][rank]
      std::vector< std::vector< std::vector<MPI_Request*> > > sendToStageRequests;
      std::vector< std::vector< std::vector<MPI_Request*> > > receiveFromStageRequests;

};

/*
** class name: COMM2D from COMM
*/
class COMM2D : public COMM {

   public:

      COMM2D (
            int                *argc,
            char               **argv[],
            const int numStages,
            const int thisStageNum,
            const int tiles[2]);

      ~COMM2D (void);

      template<typename type>
         bool send_up (
               type *data,
               int dataSize,
               int tag);

      template<typename type>
         bool send_right (
               type *data,
               int dataSize,
               int tag);

      template<typename type>
         bool receive_down (
               type *data,
               int dataSize,
               int tag);

      template<typename type>
         bool receive_left (
               type *data,
               int dataSize,
               int tag);

      bool wait_for_send_up (int tag);

      bool wait_for_send_right (int tag);

      bool wait_for_receive_down (int tag);

      bool wait_for_receive_left (int tag);

   protected:

      int                        numCommSendDim0and1Handles;
      int                        numTilesDim0;
      int                        numTilesDim1;
      std::vector <MPI_Request*> localSendRequestsDim0and1;
      std::vector <MPI_Request*> localReceiveRequestsDim0and1;

   private:

      MPI_Comm dimensions0and1Comm;
      int      dimensions0and1Rank;

};

/*
** class name: COMM3D from COMM
*/
class COMM3D : public COMM2D {

   public:

      COMM3D (
            int                *argc,
            char               **argv[],
            const int numStages,
            const int thisStageNum,
            const int tiles[3]);

      ~COMM3D (void);

      template <typename type> bool send_to_next_dim2 (type* data, int src_size, int tag);
      template <typename type> bool receive_from_previous_dim2 (type* data, int src_size, int tag);
      bool wait_for_send_to_next_dim2 (int tag);

   protected:

      int                        numCommSendDim2Handles;
      int                        numTilesDim2;
      std::vector <MPI_Request*> localSendRequestsDim2;
      std::vector <MPI_Request*> localReceiveRequestsDim2;

   private:

      MPI_Comm dimension2Comm;
      int      dimension2Rank;
};

} // namespace comm
#endif
