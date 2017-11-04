// master.cpp

#include "com.h"
#include "master_comm.h"
#include "extract_data.h"
#include "fio.h"
#include "parameters.h"
#include "memory.h"
#include "proc_maps.h"
#include <iostream>
#include <stdlib.h>

typedef struct {
   mem::Memory        workspace;
   fio::Text_file*    parameters;
   com::tsk::barrier* barrier;
   Master_comm*       master_comm;
} Master_task_params;

static void* master_task( void* task_args );

int main( int argc, char* argv[] )
{

   // setup processor communications
   Master_comm master_comm( argc, argv );

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

   /*
   ** start the master task
   */
   com::tsk::create( &master_tsk_handle,
                     master_task,
                     (void*)&master_task_params );

   // wait for the master task to finish
   com::tsk::barrier_wait( &master_barrier );
   std::cout << "master task processing complete" << std::endl;

   /*
   ** close down and finalize master processing
   */

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

static void* master_task( void* task_args )
{
   // cast task arguments as Master_task_params type
   Master_task_params* master_task_params = (Master_task_params*)task_args;

   // announce ourselves
   std::cout << "start master task processing" << std::endl;

   // TODO: use workbuffer
   int* data = new int[1024];

   data[0] = 2;
   data[1] = 7;
   data[2] = 1;
   data[3] = 8;

   // extract data
   extract_data(
         data,    // source
         4,       // count
         master_task_params->master_comm->get_dex_comm() );

   // terminate master data extraction task
   finalize_extraction( master_task_params->master_comm->get_dex_comm() );

   // TODO: use workbuffer
   delete[] data;

   // tell the main thread this task is complete
   com::tsk::barrier_wait( master_task_params->barrier );
}
