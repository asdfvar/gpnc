// master.cpp

#include "com.h"
#include "master.h"
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

   std::string str_mem_size = getenv( "GPNC_MASTER_MEM" );
   int mem_size = atoi( str_mem_size.c_str() );
   int mem_size_words = (mem_size + 4) / 4;

   // declare and define workspace
   mem::Memory workspace( mem_size_words, "master" );

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
   master_task_params.dex_comm    = master_comm.get_dex_comm();

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
