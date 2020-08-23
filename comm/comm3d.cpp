#include "comm.h"
#include <iostream>

namespace comm {

   COMM3D::COMM3D (
         int                *argc,
         char               **argv[],
         const int numStages,
         const int thisStageNum_in,
         const int tiles[3])
      : COMM2D (argc, argv, numStages, thisStageNum_in, tiles)
   {
      rankUp   = (localRank + tiles[0] * tiles[1]) % (tiles[0] * tiles[1] * tiles[2]);
      rankDown = (localRank - tiles[0] * tiles[1]);
      if (rankDown < 0) rankDown += tiles[0] * tiles[1] * tiles[2];
   }

   COMM3D::~COMM3D (void)
   {
      // pass
   }

   /*
    ** function name: send_up from COMM3D
    */
   template <typename type> bool COMM3D::send_up (type *data, int src_size, int tag)
   {
      return send (data, src_size, rankUp, tag);
   }

   template bool COMM3D::send_up (float  *data, int src_size, int tag);
   template bool COMM3D::send_up (double *data, int src_size, int tag);
   template bool COMM3D::send_up (int    *data, int src_size, int tag);
   template bool COMM3D::send_up (char   *data, int src_size, int tag);

   /*
    ** function name: send_down from COMM3D
    */
   template <typename type> bool COMM3D::send_down (type *data, int src_size, int tag)
   {
      return send (data, src_size, rankDown, tag);
   }

   template bool COMM3D::send_down (float  *data, int src_size, int tag);
   template bool COMM3D::send_down (double *data, int src_size, int tag);
   template bool COMM3D::send_down (int    *data, int src_size, int tag);
   template bool COMM3D::send_down (char   *data, int src_size, int tag);

   /*
    ** function name: receive_up from COMM3D
    */
   template <typename type> bool COMM3D::receive_up (type *data, int src_size, int tag)
   {
      return receive (data, src_size, rankUp, tag);
   }

   template bool COMM3D::receive_up (float  *data, int src_size, int tag);
   template bool COMM3D::receive_up (double *data, int src_size, int tag);
   template bool COMM3D::receive_up (int    *data, int src_size, int tag);
   template bool COMM3D::receive_up (char   *data, int src_size, int tag);

   /*
    ** function name: receive_down from COMM3D
    */
   template <typename type> bool COMM3D::receive_down (type *data, int src_size, int tag)
   {
      return receive (data, src_size, rankDown, tag);
   }

   template bool COMM3D::receive_down (float  *data, int src_size, int tag);
   template bool COMM3D::receive_down (double *data, int src_size, int tag);
   template bool COMM3D::receive_down (int    *data, int src_size, int tag);
   template bool COMM3D::receive_down (char   *data, int src_size, int tag);

   bool COMM3D::wait_for_send_up (int tag)
   {
      return wait_for_send (rankUp, tag);
   }

   bool COMM3D::wait_for_send_down (int tag)
   {
      return wait_for_send (rankDown, tag);
   }

   bool COMM3D::wait_for_receive_up (int tag)
   {
      return wait_for_receive (rankUp, tag);
   }

   bool COMM3D::wait_for_receive_down (int tag)
   {
      return wait_for_receive (rankDown, tag);
   }

} // namespace comm
