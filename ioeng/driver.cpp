#include "com.h"
#include "proc_maps.h"
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
   com::proc::intercomm_create( my_comm, MASTER_GROUP, DATA_EXT, &master_comm );

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

#if 0
  /*
   * Function NAME: write_val
   */
  void write_val(float val, std::string out_file_path, bool init)
  {

    std::ofstream out_file;
    if (init) {
       out_file.open (out_file_path.c_str());
       std::cout << "Writing data to " << out_file_path << std::endl;
    } else {
       out_file.open (out_file_path.c_str(), std::ios::app);
    }
    out_file << val << "\n";
    out_file.close();

  }
#endif
