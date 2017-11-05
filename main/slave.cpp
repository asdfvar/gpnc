// slave.cpp

#include "com.h"
#include "fio.h"
#include "memory.h"
#include "worker_task.h"
#include "proc_maps.h"
#include <iostream>
#include "slave_comm.h"

int main( int argc, char* argv[] )
{

   Slave_comm slave_comm( argc, argv );

   fio::Text_file parameters( "../parameters/parameters.txt" );

   Worker_tsk_params worker_tsk_parameters;

   size_t mem_size           = parameters.get_int( "memory_size_slave" );

   mem::Memory workspace( mem_size );

   std::cout << "parameters = " << worker_tsk_parameters.par_int   << ", "
                                << worker_tsk_parameters.par_float << ", "
                                << worker_tsk_parameters.par_double << std::endl;

   com::tsk::handler worker_tsk_handle;
   com::tsk::barrier worker_barrier;
   com::tsk::barrier_init( &worker_barrier, 2 );

   worker_tsk_parameters.barrier = &worker_barrier;

   com::tsk::create( &worker_tsk_handle,
                     worker_task,
                     (void*)&worker_tsk_parameters );

   com::tsk::barrier_wait( &worker_barrier );
   std::cout << "AFTER barrier" << std::endl;

   /***************************************************************************
   * finish processing
   ***************************************************************************/

   // destroy thread barrier
   com::tsk::barrier_destroy( &worker_barrier );

   // suspend execution of the worker task
   com::tsk::join( worker_tsk_handle );

   slave_comm.finalize();

   // free workspace memory from heap
   workspace.finalize();

   return 0;
}
