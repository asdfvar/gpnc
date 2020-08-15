#ifndef COMM_3D_H
#define COMM_3D_H

#include <mpi.h>
#include <vector>

namespace comm {

   /*
    ** class name: COMM3D from COMM
    */
   class COMM3D : public COMM2D {

      public:

         COMM3D (
               int       *argc,
               char      **argv[],
               const int numStages,
               const int thisStageNum,
               const int tiles[3]);

         ~COMM3D (void);

         template <typename type> bool send_to_next_dim2 (
               type* data,
               int src_size,
               int tag);

         template <typename type> bool receive_from_previous_dim2 (
               type* data,
               int src_size,
               int tag);

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
