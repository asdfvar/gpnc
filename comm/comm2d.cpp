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

   rankUp    = (localRank + numTilesDim0) % (numTilesDim0 * numTilesDim1);

   rankDown  = localRank - numTilesDim0;
   if (rankDown < 0) rankDown += numTilesDim0 * numTilesDim1;

   rankLeft  = localRank - 1;
   if (localRank % numTilesDim0 != 0) rankLeft = localRank - 1;
   else                               rankLeft = localRank + numTilesDim0 - 1;

   rankRight = localRank + 1;
   if (rankRight % numTilesDim0 == 0) rankRight -= numTilesDim0;
}

COMM2D::~COMM2D (void)
{
   // pass
}

template <typename type>
bool COMM2D::send_up (type *data, int dataSize, int tag)
{
   bool stat = true;

   int receiveRank = (localRank + numTilesDim0) % (numTilesDim0 * numTilesDim1);

   stat |= send (data, dataSize, receiveRank, tag);

   return stat;
}

template bool COMM2D::send_up (float*  data, int dataSize, int tag);
template bool COMM2D::send_up (double* data, int dataSize, int tag);
template bool COMM2D::send_up (int*    data, int dataSize, int tag);
template bool COMM2D::send_up (char*   data, int dataSize, int tag);

template <typename type>
bool COMM2D::send_right (type *data, int dataSize, int tag)
{
   bool stat = true;

   int targetRank = localRank + 1;
   if (targetRank % numTilesDim0 == 0) targetRank -= numTilesDim0;

   stat |= send (data, dataSize, targetRank, tag);

   return stat;
}

template bool COMM2D::send_right (float*  data, int dataSize, int tag);
template bool COMM2D::send_right (double* data, int dataSize, int tag);
template bool COMM2D::send_right (int*    data, int dataSize, int tag);
template bool COMM2D::send_right (char*   data, int dataSize, int tag);

template <typename type>
bool COMM2D::send_down (type *data, int dataSize, int tag)
{
   bool stat = true;

   int receiveRank = localRank - numTilesDim0;
   if (receiveRank < 0) receiveRank += numTilesDim0 * numTilesDim1;

   stat |= send (data, dataSize, receiveRank, tag);

   return stat;
}

template bool COMM2D::send_down (float*  data, int dataSize, int tag);
template bool COMM2D::send_down (double* data, int dataSize, int tag);
template bool COMM2D::send_down (int*    data, int dataSize, int tag);
template bool COMM2D::send_down (char*   data, int dataSize, int tag);

template <typename type>
bool COMM2D::send_left (type *data, int dataSize, int tag)
{
   bool stat = true;

   int receiveRank = localRank - 1;
   if (localRank % numTilesDim0 != 0) receiveRank = localRank - 1;
   else                               receiveRank = localRank + numTilesDim0 - 1;

   stat |= send (data, dataSize, receiveRank, tag);

   return stat;
}

template bool COMM2D::send_left (float*  data, int dataSize, int tag);
template bool COMM2D::send_left (double* data, int dataSize, int tag);
template bool COMM2D::send_left (int*    data, int dataSize, int tag);
template bool COMM2D::send_left (char*   data, int dataSize, int tag);

template <typename type>
bool COMM2D::receive_down (type *data, int dataSize, int tag) {

   bool stat = true;

   int sendRank = localRank - numTilesDim0;
   if (sendRank < 0) sendRank += numTilesDim0 * numTilesDim1;

   stat |= receive (data, dataSize, sendRank, tag);

   return stat;
}

template bool COMM2D::receive_down (float*  data, int dataSize, int tag);
template bool COMM2D::receive_down (double* data, int dataSize, int tag);
template bool COMM2D::receive_down (int*    data, int dataSize, int tag);
template bool COMM2D::receive_down (char*   data, int dataSize, int tag);

template <typename type>
bool COMM2D::receive_left (type *data, int dataSize, int tag)
{
   bool stat = true;

   int targetRank = localRank - 1;
   if (localRank % numTilesDim0 == 0) targetRank += numTilesDim0;

   stat |= receive (data, dataSize, targetRank, tag);

   return stat;
}

template bool COMM2D::receive_left (float*  data, int dataSize, int tag);
template bool COMM2D::receive_left (double* data, int dataSize, int tag);
template bool COMM2D::receive_left (int*    data, int dataSize, int tag);
template bool COMM2D::receive_left (char*   data, int dataSize, int tag);

template <typename type>
bool COMM2D::receive_up (type *data, int dataSize, int tag)
{
   bool stat = true;

   int receiveRank = localRank + numTilesDim0;
   if (receiveRank > numTilesDim0 * numTilesDim1) receiveRank = localRank % numTilesDim0;

   stat |= receive (data, dataSize, receiveRank, tag);

   return stat;
}

template bool COMM2D::receive_up (float*  data, int dataSize, int tag);
template bool COMM2D::receive_up (double* data, int dataSize, int tag);
template bool COMM2D::receive_up (int*    data, int dataSize, int tag);
template bool COMM2D::receive_up (char*   data, int dataSize, int tag);

template <typename type>
bool COMM2D::receive_right (type *data, int dataSize, int tag)
{
   bool stat = true;

   int targetRank = localRank + 1;
   if (targetRank % numTilesDim0 == 0) targetRank -= numTilesDim0;

   stat |= receive (data, dataSize, targetRank, tag);

   return stat;
}

template bool COMM2D::receive_right (float*  data, int dataSize, int tag);
template bool COMM2D::receive_right (double* data, int dataSize, int tag);
template bool COMM2D::receive_right (int*    data, int dataSize, int tag);
template bool COMM2D::receive_right (char*   data, int dataSize, int tag);

bool COMM2D::wait_for_send_up (int tag)
{
   int targetRank = (localRank + numTilesDim0) % (numTilesDim0 * numTilesDim1);

   return wait_for_send (targetRank, tag);
}

bool COMM2D::wait_for_send_right (int tag)
{
   int targetRank = localRank + 1;
   if (targetRank % numTilesDim0 == 0) targetRank -= numTilesDim0;

   return wait_for_send (targetRank, tag);
}

bool COMM2D::wait_for_receive_down (int tag)
{
   int targetRank = localRank - numTilesDim0;

   if (targetRank < 0) targetRank += numTilesDim0 * numTilesDim1;

   return wait_for_receive (targetRank, tag);
}

bool COMM2D::wait_for_receive_left (int tag)
{
   int targetRank = localRank - 1;
   if (localRank % numTilesDim0 == 0) targetRank += numTilesDim0;

   return wait_for_receive (targetRank, tag);
}

} // namespace comm
