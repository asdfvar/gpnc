#ifndef COMM_2D_H
#define COMM_2D_H

#include <mpi.h>
#include <vector>

namespace comm {

/*
** class name: COMM2D from COMM
*/
class COMM2D : public COMM
{
   public:

      COMM2D (
            int       *argc,
            char      **argv[],
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
         bool send_down (
               type *data,
               int dataSize,
               int tag);

      template<typename type>
         bool send_left (
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

      template<typename type>
         bool receive_up (
               type *data,
               int dataSize,
               int tag);

      template<typename type>
         bool receive_right (
               type *data,
               int dataSize,
               int tag);

      bool wait_for_send_up (int tag);

      bool wait_for_send_right (int tag);

      bool wait_for_send_down (int tag);

      bool wait_for_send_left (int tag);

      bool wait_for_receive_down (int tag);

      bool wait_for_receive_left (int tag);

      bool wait_for_receive_up (int tag);

      bool wait_for_receive_right (int tag);

   protected:

      int                        numCommSendDim0and1Handles;
      int                        numTilesDim0;
      int                        numTilesDim1;
      std::vector <MPI_Request*> localSendRequestsDim0and1;
      std::vector <MPI_Request*> localReceiveRequestsDim0and1;

   private:

      MPI_Comm dimensions0and1Comm;
      int      dimensions0and1Rank;
      int      rankUp;
      int      rankDown;
      int      rankLeft;
      int      rankRight;

};

} // namespace comm
#endif
