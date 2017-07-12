#include "com.h"
#include <iostream>

int main( int argc, char* argv[] )
{

   com::Com comObj( argc, argv );

   float buf[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
   MPI_Request request;

   std::cout << "about to send" << std::endl;

   comObj.send( (float*)buf,
                4,
                1,
               &request );

   std::cout << "sending" << std::endl;

   comObj.wait( &request );

   std::cout << "sent" << std::endl;

   return 0;
}
