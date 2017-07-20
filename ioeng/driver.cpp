#include "com.h"
#include <iostream>

int main( int argc, char* argv[] )
{

   com::Com comObj( argc, argv );

   float buf[10];
   MPI_Request request;
   std::cout << "about to receive" << std::endl;

   comObj.recv( (float*)buf,
                4,            // count
                0,            // proc id
                1,            // tag
               &request );

   std::cout << "receiving" << std::endl;

   comObj.wait( &request );

   std::cout << "received" << std::endl;

   std::cout << "buf = " << buf[0] << buf[1] << buf[2] << buf[3] << std::endl;

   return 0;
}
