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

         template<typename type>
            bool send_up (
                  type *data,
                  int dataSize,
                  int tag);

         template<typename type>
            bool send_down (
                  type *data,
                  int dataSize,
                  int tag);

         template <typename type>
            bool receive_up (
                  type *data,
                  int src_size,
                  int tag);

         template <typename type>
            bool receive_down (
                  type *data,
                  int src_size,
                  int tag);

         bool wait_for_send_up      (int tag);
         bool wait_for_send_down    (int tag);
         bool wait_for_receive_up   (int tag);
         bool wait_for_receive_down (int tag);

      protected:

         int rankUp;
         int rankDown;
   };

} // namespace comm
#endif
