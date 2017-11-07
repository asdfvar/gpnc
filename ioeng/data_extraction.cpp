// data_extraction.cpp

#include "com.h"
#include "proc_maps.h"
#include <iostream>
#include "master_dex.h"
#include "slave_dex.h"

int main( int argc, char* argv[] )
{

   // get global rank and number of procs
   int global_rank;
   int numprocs;
   com::proc::init( argc, argv );
   com::proc::size( com::proc::Comm_world, &numprocs    );
   com::proc::rank( com::proc::Comm_world, &global_rank );

   // create output group
   com::proc::Comm my_comm;
   com::proc::split(
         DATA_EXTRACTION_GROUP,
         global_rank,
         &my_comm );

   // get local rank
   int local_rank;
   com::proc::rank( my_comm, &local_rank );

   // inter-communicator to master
   com::proc::Comm master_comm;
   com::proc::intercomm_create(
         my_comm,
         MASTER_GROUP,
         MASTER_DATA_EXT,
         &master_comm );

   // declare the master DEX task handle
   com::tsk::handler master_dex_handle;

   // declare the master DEX barrier
   com::tsk::barrier master_dex_barrier;

   // declare the master DEX parameters
   Master_dex_params master_dex_params;

   master_dex_params.master_comm = master_comm;

   // initialize the master DEX barrier
   com::tsk::barrier_init( &master_dex_barrier, 2 );

   master_dex_params.barrier = &master_dex_barrier;

   // start master data extraction task
   com::tsk::create(
         &master_dex_handle,
         master_dex_task,
         (void*)&master_dex_params );

   // inter-communicator to slave
   com::proc::Comm slave_comm;
   com::proc::intercomm_create(
         my_comm,
         SLAVE_GROUP,
         SLAVE_DATA_EXT,
         &slave_comm );

   // declare the slave DEX task handle
   com::tsk::handler slave_dex_handle;

   // declare the slave DEX barrier
   com::tsk::barrier slave_dex_barrier;

   // declare the slave DEX parameters
   Slave_dex_params slave_dex_params;

   // initialize the slave DEX barrier
   com::tsk::barrier_init( &slave_dex_barrier, 2 );

   slave_dex_params.barrier = &slave_dex_barrier;

   // start slave data extraction task
   com::tsk::create(
         &slave_dex_handle,
         slave_dex_task,
         (void*)&slave_dex_params );

   // wait for the master DEX task to finish
   com::tsk::barrier_wait( &master_dex_barrier );
   std::cout << "master DEX task processing complete" << std::endl;

   // wait for the slave DEX task to finish
   com::tsk::barrier_wait( &slave_dex_barrier );
   std::cout << "slave DEX task processing complete" << std::endl;

   /********************************************
    ** close down and finalize DEX processing **
    *******************************************/

   // destroy master DEX barrier
   com::tsk::barrier_destroy( &master_dex_barrier );

   // destroy slave DEX barrier
   com::tsk::barrier_destroy( &slave_dex_barrier );

   // free master-group comm handle
   com::proc::free( &master_comm );

   // free slave-group comm handle
   com::proc::free( &slave_comm );

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
