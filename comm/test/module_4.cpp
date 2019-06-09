#include "comm.h"
#include <iostream>
#include "configuration.h"

int main (int argc, char *argv[])
{

   comm::COMM Comm (&argc, &argv, NUM_STAGES, 4);

   std::cout << "module 4 is done" << std::endl;

   return 0;
}
