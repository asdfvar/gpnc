// master.cpp

#include "com.h"
#include "master.h"
#include "extract_data.h"
#include "fio.h"
#include "memory.h"
#include "proc_maps.h"
#include <iostream>
#include <stdlib.h>

int main( int argc, char* argv[] )
{

   // setup process communications
   Master_comm master_comm( argc, argv );

   // read parameter file
   fio::Parameter parameters( getenv( "GPNC_PARAMS" ) );

#if 0
   std::cout << std::endl;
   std::cout << "reading in parameter-file contents:" << std::endl;
   std::cout << "###################################" << std::endl;
   parameters.print_all();
   std::cout << "###################################" << std::endl;
   std::cout << std::endl;
#endif

   size_t mem_size = parameters.get_int( "memory_size_master" );

   // declare and define workspace
   mem::Memory workspace( mem_size );

   // declare the master-task handle
   com::tsk::handler master_tsk_handle;

   // declare the master-task barrier
   com::tsk::barrier master_barrier;

   // initialize the master-task barrier
   com::tsk::barrier_init( &master_barrier, 2 );

   // populate the master-task parameters
   Master_task_params master_task_params;
   master_task_params.parameters  = &parameters;
   master_task_params.workspace   = workspace;
   master_task_params.barrier     = &master_barrier;
   master_task_params.master_comm = &master_comm;

   // start the master task
   com::tsk::create( &master_tsk_handle,
                     master_task,
                     (void*)&master_task_params );

   // wait for the master task to finish
   com::tsk::barrier_wait( &master_barrier );

   /**************************************************************************
   * finish processing
   ***************************************************************************/

   // wait for all processes to sync before closing down
   com::proc::Barrier( com::proc::Comm_world );

   // destroy thread barrier
   com::tsk::barrier_destroy( &master_barrier );

   // suspend execution of the master task
   com::tsk::join( master_tsk_handle );

   // finalize process communication
   master_comm.finalize();

   // free workspace memory from heap
   workspace.finalize();

   return 0;
}
