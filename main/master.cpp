#include "com.h"
#include "data_extraction.h"
#include "fio.h"
#include "parameters.h"
#include "memory.h"
#include "proc_maps.h"
#include <iostream>
#include <stdlib.h>

typedef struct {
   mem::Memory       workspace;
   com::tsk::barrier* barrier;
} Master_tsk_params;

static void* master_tsk( void* task_args );

int main( int argc, char* argv[] )
{

   // get global rank and number of procs
   int numprocs;
   int myid;
   com::proc::start( argc, argv, &numprocs, &myid );

   // create output group
   com::proc::Comm my_comm;
   com::proc::split( MASTER_GROUP, myid, &my_comm );

   // get local rank
   int local_rank;
   com::proc::rank( my_comm, &local_rank );

   // inter-communicator to data extraction
   com::proc::Comm IO_comm;
   com::proc::intercomm_create( my_comm, DATA_EXTRACTION_GROUP, DATA_EXT, &IO_comm );

   // read parameter file
   fio::Text_file parameters( getenv( "GPNC_PARAMS" ) );

   std::cout << std::endl;
   std::cout << "reading in parameter-file contents:" << std::endl;
   parameters.print_all();
   std::cout << std::endl;

   Inp_params inp_parameters;

   inp_parameters.par_int    = parameters.get_int( "parameter_int" );
   inp_parameters.par_float  = parameters.get_real( "parameter_float" );
   inp_parameters.par_double = parameters.get_real( "parameter_double" );
   size_t mem_size           = parameters.get_int( "memory_size_master" );

   // declare and define workspace
   mem::Memory workspace( mem_size );

   Master_tsk_params master_tsk_params;
   master_tsk_params.workspace = workspace;

   com::tsk::handler master_tsk_handle;
   com::tsk::barrier master_barrier;

   master_tsk_params.barrier = &master_barrier;
   com::tsk::barrier_init( &master_barrier, 2 );

   /***************************************************************************
   * start the master task
   ***************************************************************************/

   com::tsk::create( &master_tsk_handle,
                     master_tsk,
                     (void*)&master_tsk_params );

   // wait for the master task to finish
   com::tsk::barrier_wait( &master_barrier );
   std::cout << "master task processing complete" << std::endl;

   float buf[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
   com::proc::Request request;

   std::cout << "about to send" << std::endl;

   com::proc::Isend(
         (float*)buf,
         4,            // count
         0,            // proc id
         1,            // tag
         IO_comm,
         &request );

   std::cout << "sending" << std::endl;

   com::proc::wait( &request );

   std::cout << "sent" << std::endl;

   /***************************************************************************
   * finish processing
   ***************************************************************************/

   // free IO-group comm handle
   com::proc::free( &IO_comm );

   // destroy thread barrier
   com::tsk::barrier_destroy( &master_barrier );

   // suspend execution of the master task
   com::tsk::join( master_tsk_handle );

   // finalize process communication
   com::proc::finalize();

   // free workspace memory from heap
   workspace.finalize();

   return 0;
}

static void* master_tsk( void* task_args )
{
   Master_tsk_params* master_tsk_params = (Master_tsk_params*)task_args;

   std::cout << "master task processing" << std::endl;

   com::tsk::barrier_wait( master_tsk_params->barrier );
}
