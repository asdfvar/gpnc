#include "com.h"
#include "io.h"
#include "groups.h"
#include <iostream>

int main( int argc, char* argv[] )
{

   // get global rank and number of procs
   int global_rank;
   int numprocs;
   com::proc::init(      argc,       argv    );
   com::proc::size( com::proc::Comm_world, &numprocs    );
   com::proc::rank( com::proc::Comm_world, &global_rank );

   // create output group
   com::proc::Comm my_comm;
   com::proc::split( DATA_EXTRACTION_GROUP, global_rank, &my_comm );

   // get local rank
   int local_rank;
   com::proc::rank( my_comm, &local_rank );

   // inter-communicator to master
   com::proc::Comm master_comm;
   com::proc::intercomm_create( my_comm, MASTER_GROUP, 19, &master_comm );

//   io::Write writeObj( global_rank, MASTER_GROUP, my_comm, 7 );

   float buf[10];
   com::proc::Request request;
   std::cout << "about to receive" << std::endl;

   com::proc::Irecv(
         (float*)buf,
         4,            // count
         0,            // proc id
         1,            // tag
         master_comm,
         &request );

   std::cout << "receiving" << std::endl;

   com::proc::wait( &request );

   std::cout << "received" << std::endl;

   std::cout << "buf = " << buf[0] << buf[1] << buf[2] << buf[3] << std::endl;

   /***************************************************************************
    * finish processing
    ***************************************************************************/

   // free master-group comm handle
   com::proc::free( &master_comm );

   // finalize process communication
   com::proc::finalize();

   return 0;
}
