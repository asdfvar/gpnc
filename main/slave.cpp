// slave.cpp

#include "com.h"
#include "fio.h"
#include "memory.h"
#include "slave.h"
#include "proc_maps.h"
#include <iostream>

int main( int argc, char* argv[] )
{

   Slave_comm slave_comm( argc, argv );

   Slave_tsk_params slave_tsk_parameters;

   fio::Text_file parameters( getenv( "GPNC_PARAMS" ) );
   slave_tsk_parameters.parameters = &parameters;

   size_t mem_size = parameters.get_int( "memory_size_slave" );

   mem::Memory workspace( mem_size );

   com::tsk::handler slave_tsk_handle;
   com::tsk::barrier slave_barrier;
   com::tsk::barrier_init( &slave_barrier, 2 );

   slave_tsk_parameters.barrier = &slave_barrier;

   com::tsk::create( &slave_tsk_handle,
                     slave_task,
                     (void*)&slave_tsk_parameters );

   // wait for slave task to finish
   com::tsk::barrier_wait( &slave_barrier );
   std::cout << "AFTER barrier" << std::endl;

   /***************************************************************************
   * finish processing
   ***************************************************************************/

   // destroy thread barrier
   com::tsk::barrier_destroy( &slave_barrier );

   // suspend execution of the slave task
   com::tsk::join( slave_tsk_handle );

   slave_comm.finalize();

   // free workspace memory from heap
   workspace.finalize();

   return 0;
}
