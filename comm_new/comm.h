#ifndef COMM_H
#define COMM_H

#include <mpi.h>
#include <vector>

#define MAX_TAGS       200
#define MAX_STAGES     20
#define MAX_INTERCOMMS MAX_STAGES
#define INVALID_TAG    -1

namespace comm {

/*
** class name: COMM
*/
class COMM {

   public:
      COMM (int                *argc,
            char               ***argv,
            const unsigned int *numStageProcs,
            const unsigned int numStages,
            const unsigned int thisStageNum);

     ~COMM (void);

     int rank (void);

     template<typename type>
        bool send_to_stage (
              type *data,
              int dataSize,
              unsigned int recvStage,
              unsigned int recvStageRank,
              int tag);

      bool wait_for_send_to_stage (
            unsigned int recvStage,
            unsigned int recvStageRank,
            int tag);

      template<typename type>
         bool send_to_stage (
               type* data,
               int size,
               int stage,
               int tag)
         {
            return send_to_stage (data, size, stage, 0, tag);
         }

      template <typename type>
         bool receive_from_stage (
               type* data,
               int dataSize,
               unsigned int sendStage,
               unsigned int stageRank,
               int tag);

      bool wait_for_receive_from_stage
         (unsigned int sendStage,
          unsigned int sendStageRank,
          int tag);

   protected:

      unsigned int numStageProcs[MAX_STAGES];

      MPI_Group stageGroups[MAX_STAGES];
      MPI_Comm  stageComms[MAX_STAGES];
      MPI_Comm  interComms[MAX_INTERCOMMS];

      // accounting
      int tagsToStage[MAX_STAGES][MAX_TAGS];
      int tagsFromStage[MAX_STAGES][MAX_TAGS];
      unsigned int thisStageNum;
      int sendToStageHandles[MAX_STAGES];
      int numSendToStageHandles[MAX_STAGES];
      int numReceiveFromStageHandles[MAX_STAGES];

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
            const unsigned int *numStageProcs,
            const unsigned int numStages,
            const unsigned int thisStageNum,
            const unsigned int tiles[2]);

      ~COMM2D (void);

   protected:

      int                        tagsDim0and1[MAX_TAGS];
      int                        numCommSendDim0and1Handles;
      int                        waitingStage[MAX_TAGS];
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
            const unsigned int *numStageProcs,
            const unsigned int numStages,
            const unsigned int thisStageNum,
            const unsigned int tiles[3]);

      ~COMM3D (void);

      template <typename type> bool send_to_next_dim2 (type* data, int src_size, int tag);
      template <typename type> bool receive_from_previous_dim2 (type* data, int src_size, int tag);
      bool wait_for_send_to_next_dim2 (int tag);

   protected:

      int                        tagsDim2[MAX_TAGS];
      int                        numCommSendDim2Handles;
      int                        numTilesDim2;
      std::vector <MPI_Request*> localSendRequestsDim2;
      std::vector <MPI_Request*> localReceiveRequestsDim2;
      bool                       sendingToDim2[MAX_TAGS];
      bool                       receivingFromDim2[MAX_TAGS];

   private:

      MPI_Comm dimension2Comm;
      int      dimension2Rank;
};

} // namespace comm
#endif
