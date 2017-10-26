// slave.cpp

#include "com.h"
#include "fio.h"
#include "parameters.h"
#include "memory.h"
#include "worker_task.h"
#include "proc_maps.h"
#include <iostream>

int main( int argc, char* argv[] )
{

   int numprocs;
   int myid;

   com::proc::start( argc, argv, &numprocs, &myid );

   com::proc::Comm my_comm;

//   com::proc::split( SLAVE_GROUP, myid, &my_comm );
int group_number = 2;
   MPI_Comm_split( MPI_COMM_WORLD, group_number, myid, &my_comm );

   fio::Text_file parameters( "../parameters/parameters.txt" );

   Worker_tsk_params worker_tsk_parameters;
   Inp_params        inp_parameters;

   inp_parameters.par_int    = parameters.get_int( "parameter_int" );
   inp_parameters.par_float  = parameters.get_real( "parameter_float" );
   inp_parameters.par_double = parameters.get_real( "parameter_double" );
   size_t mem_size           = parameters.get_int( "memory_size_slave" );

   worker_tsk_parameters.par_int    = inp_parameters.par_int;
   worker_tsk_parameters.par_float  = inp_parameters.par_float;
   worker_tsk_parameters.par_double = inp_parameters.par_double;

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
std::cout << __FILE__ << ":" << __LINE__ << ":got here" << std::endl;

   // destroy thread barrier
   com::tsk::barrier_destroy( &worker_barrier );

   // suspend execution of the worker task
   com::tsk::join( worker_tsk_handle );

   // finalize process communication
   com::proc::finalize();

   // free workspace memory from heap
   workspace.finalize();

   return 0;
}
