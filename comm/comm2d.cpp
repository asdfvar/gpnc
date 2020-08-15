#include "comm.h"
#include <iostream>

namespace comm {

   COMM2D::COMM2D (
         int                *argc,
         char               **argv[],
         const int numStages,
         const int thisStageNum_in,
         const int tiles[2])
      : COMM (argc, argv, numStages, thisStageNum_in)
   {
      numTilesDim0 = tiles[0];
      numTilesDim1 = tiles[1];

      rankForward = (localRank + numTilesDim0) % (numTilesDim0 * numTilesDim1);

      rankBackward  = localRank - numTilesDim0;
      if (rankBackward < 0) rankBackward += numTilesDim0 * numTilesDim1;

      rankLeft  = localRank - 1;
      if (localRank % numTilesDim0 != 0) rankLeft = localRank - 1;
      else                               rankLeft = localRank + numTilesDim0 - 1;

      rankRight = localRank + 1;
      if (rankRight % numTilesDim0 == 0) rankRight -= numTilesDim0;

      circular = true;
   }

   COMM2D::~COMM2D (void)
   {
      // pass
   }

   template <typename type>
      bool COMM2D::send_forward (type *data, int dataSize, int tag)
      {
         bool stat = true;

         stat |= send (data, dataSize, rankForward, tag);

         return stat;
      }

   template bool COMM2D::send_forward (float*  data, int dataSize, int tag);
   template bool COMM2D::send_forward (double* data, int dataSize, int tag);
   template bool COMM2D::send_forward (int*    data, int dataSize, int tag);
   template bool COMM2D::send_forward (char*   data, int dataSize, int tag);

   template <typename type>
      bool COMM2D::send_right (type *data, int dataSize, int tag)
      {
         bool stat = true;

         stat |= send (data, dataSize, rankRight, tag);

         return stat;
      }

   template bool COMM2D::send_right (float*  data, int dataSize, int tag);
   template bool COMM2D::send_right (double* data, int dataSize, int tag);
   template bool COMM2D::send_right (int*    data, int dataSize, int tag);
   template bool COMM2D::send_right (char*   data, int dataSize, int tag);

   template <typename type>
      bool COMM2D::send_backward (type *data, int dataSize, int tag)
      {
         bool stat = true;

         stat |= send (data, dataSize, rankBackward, tag);

         return stat;
      }

   template bool COMM2D::send_backward (float*  data, int dataSize, int tag);
   template bool COMM2D::send_backward (double* data, int dataSize, int tag);
   template bool COMM2D::send_backward (int*    data, int dataSize, int tag);
   template bool COMM2D::send_backward (char*   data, int dataSize, int tag);

   template <typename type>
      bool COMM2D::send_left (type *data, int dataSize, int tag)
      {
         bool stat = true;

         stat |= send (data, dataSize, rankLeft, tag);

         return stat;
      }

   template bool COMM2D::send_left (float*  data, int dataSize, int tag);
   template bool COMM2D::send_left (double* data, int dataSize, int tag);
   template bool COMM2D::send_left (int*    data, int dataSize, int tag);
   template bool COMM2D::send_left (char*   data, int dataSize, int tag);

   template <typename type>
      bool COMM2D::receive_backward (type *data, int dataSize, int tag) {

         bool stat = true;

         stat |= receive (data, dataSize, rankBackward, tag);

         return stat;
      }

   template bool COMM2D::receive_backward (float*  data, int dataSize, int tag);
   template bool COMM2D::receive_backward (double* data, int dataSize, int tag);
   template bool COMM2D::receive_backward (int*    data, int dataSize, int tag);
   template bool COMM2D::receive_backward (char*   data, int dataSize, int tag);

   template <typename type>
      bool COMM2D::receive_left (type *data, int dataSize, int tag)
      {
         bool stat = true;

         stat |= receive (data, dataSize, rankLeft, tag);

         return stat;
      }

   template bool COMM2D::receive_left (float*  data, int dataSize, int tag);
   template bool COMM2D::receive_left (double* data, int dataSize, int tag);
   template bool COMM2D::receive_left (int*    data, int dataSize, int tag);
   template bool COMM2D::receive_left (char*   data, int dataSize, int tag);

   template <typename type>
      bool COMM2D::receive_forward (type *data, int dataSize, int tag)
      {
         bool stat = true;

         stat |= receive (data, dataSize, rankForward, tag);

         return stat;
      }

   template bool COMM2D::receive_forward (float*  data, int dataSize, int tag);
   template bool COMM2D::receive_forward (double* data, int dataSize, int tag);
   template bool COMM2D::receive_forward (int*    data, int dataSize, int tag);
   template bool COMM2D::receive_forward (char*   data, int dataSize, int tag);

   template <typename type>
      bool COMM2D::receive_right (type *data, int dataSize, int tag)
      {
         bool stat = true;

         stat |= receive (data, dataSize, rankRight, tag);

         return stat;
      }

   template bool COMM2D::receive_right (float*  data, int dataSize, int tag);
   template bool COMM2D::receive_right (double* data, int dataSize, int tag);
   template bool COMM2D::receive_right (int*    data, int dataSize, int tag);
   template bool COMM2D::receive_right (char*   data, int dataSize, int tag);

   bool COMM2D::wait_for_send_forward (int tag)
   {
      return wait_for_send (rankForward, tag);
   }

   bool COMM2D::wait_for_send_right (int tag)
   {
      return wait_for_send (rankRight, tag);
   }

   bool COMM2D::wait_for_send_backward (int tag)
   {
      return wait_for_send (rankBackward, tag);
   }

   bool COMM2D::wait_for_send_left (int tag)
   {
      return wait_for_send (rankLeft, tag);
   }

   bool COMM2D::wait_for_receive_backward (int tag)
   {
      return wait_for_receive (rankBackward, tag);
   }

   bool COMM2D::wait_for_receive_left (int tag)
   {
      return wait_for_receive (rankLeft, tag);
   }

   bool COMM2D::wait_for_receive_forward (int tag)
   {
      return wait_for_receive (rankForward, tag);
   }

   bool COMM2D::wait_for_receive_right (int tag)
   {
      return wait_for_receive (rankRight, tag);
   }

} // namespace comm
