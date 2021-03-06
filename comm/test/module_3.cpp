#include "comm.h"
#include <iostream>
#include "configuration.h"

int main (int argc, char *argv[])
{

   const int tiles[2] = {3, 4};

   comm::COMM2D Comm (&argc, &argv, NUM_STAGES, 3, tiles);

   float array0[4] = {3.14159f, 2.71828f, -7.4325f, 923.5234f};
   float array1[4];

   Comm.send_forward (
         array0, // data
         4,      // data size
         0);     // tag

   Comm.receive_backward (
         array1, // data
         4,      // data size
         0);     // tag

   Comm.wait_for_send_forward (0);

   Comm.wait_for_receive_backward (0);

   Comm.send_right (
         array1, // data
         4,      // data size
         1);     // tag

   Comm.receive_left (
         array0, // data
         4,      // data size
         1);     // tag

   Comm.wait_for_send_right (1);

   Comm.wait_for_receive_left (1);

   int myRank = Comm.rank();

   for (int rank = 0; rank < 6; rank++)
   {
      int rankValue = myRank;

      Comm.broadcast (&rankValue, 1, rank);

      std::cout << __FILE__ << __LINE__ <<
         ": rank " << Comm.rank() << " broadcast from rank "
         << rank << " and received " << rankValue << std::endl;
   }

   int localRank = Comm.rank();
   int receivedRank;

   Comm.send_left (&localRank, 1, 2);
   Comm.receive_right (&receivedRank, 1, 2);
   Comm.wait_for_send_left (2);
   Comm.wait_for_receive_right (2);
   std::cout << __FILE__ << __LINE__ <<
      ": rank " << Comm.rank() << " received rank " <<
      receivedRank << " from the right" << std::endl;

   Comm.send_forward (&localRank, 1, 3);
   Comm.receive_backward (&receivedRank, 1, 3);
   Comm.wait_for_send_forward (3);
   Comm.wait_for_receive_backward (3);
   std::cout << __FILE__ << __LINE__ <<
      ": rank " << Comm.rank() << " received rank " <<
      receivedRank << " from backward" << std::endl;

   std::cout << "module 3 is done" << std::endl;

   return 0;
}
