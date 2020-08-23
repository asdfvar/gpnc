#include "comm.h"
#include <iostream>
#include "configuration.h"

int main (int argc, char *argv[])
{

   const int tiles[3] = {3, 2, 4};

   comm::COMM3D Comm (&argc, &argv, NUM_STAGES, 5, tiles);

   int localRank = Comm.rank();
   int receivedRank;

   Comm.send_up      (&localRank,    1, 0);
   Comm.receive_down (&receivedRank, 1, 0);
   Comm.wait_for_send_up (0);
   Comm.wait_for_receive_down (0);

   std::cout << __FILE__ << __LINE__ <<
      ": rank " << localRank <<
      " received data from rank " <<
      receivedRank <<
      " from below" << std::endl;

   Comm.send_left      (&localRank,    1, 1);
   Comm.receive_right (&receivedRank, 1, 1);
   Comm.wait_for_send_left (1);
   Comm.wait_for_receive_right (1);

   std::cout << __FILE__ << __LINE__ <<
      ": rank " << localRank <<
      " received data from rank " <<
      receivedRank <<
      " from the right" << std::endl;

   std::cout << "module 5 is done" << std::endl;

   return 0;
}
