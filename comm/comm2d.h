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
            bool send_forward (
                  type *data,
                  int dataSize,
                  int tag);

         template<typename type>
            bool send_right (
                  type *data,
                  int dataSize,
                  int tag);

         template<typename type>
            bool send_backward (
                  type *data,
                  int dataSize,
                  int tag);

         template<typename type>
            bool send_left (
                  type *data,
                  int dataSize,
                  int tag);

         template<typename type>
            bool receive_backward (
                  type *data,
                  int dataSize,
                  int tag);

         template<typename type>
            bool receive_left (
                  type *data,
                  int dataSize,
                  int tag);

         template<typename type>
            bool receive_forward (
                  type *data,
                  int dataSize,
                  int tag);

         template<typename type>
            bool receive_right (
                  type *data,
                  int dataSize,
                  int tag);

         bool wait_for_send_forward (int tag);

         bool wait_for_send_right (int tag);

         bool wait_for_send_backward (int tag);

         bool wait_for_send_left (int tag);

         bool wait_for_receive_backward (int tag);

         bool wait_for_receive_left (int tag);

         bool wait_for_receive_forward (int tag);

         bool wait_for_receive_right (int tag);

      protected:

         int  numTilesDim0;
         int  numTilesDim1;
         bool circular;

      private:

         int rankForward;
         int rankBackward;
         int rankLeft;
         int rankRight;

   };

} // namespace comm
#endif
