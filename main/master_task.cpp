// master_task.cpp

#include "master.h"
#include "data_exchange.h"
#include "fio.h"
#include "com.h"
#include <iostream>

// specify the scope used here
using namespace master;

// master_alg function
static void master_alg( fio::Parameter&  parameters,
                        mem::Memory      workspace,
                        com::proc::Comm  communicator )
{
#if 1
   /*
   ** Begin primary master-task processing
   */
   int    par_int    = parameters.get_int(  "parameter_int"    );
   float  par_float  = parameters.get_real( "parameter_float"  );
   double par_double = parameters.get_real( "parameter_double" );

   int* data = (int*)workspace.reserve( 10 );

   data[0] = 22222;
   data[1] = 7;
   data[2] = 1;
   data[3] = 8;

   Message extracting;

   // extract data
   extracting.extract_data(
         data,       // source
         "filename", // filename
         4,          // count
         communicator );
#endif
}

// master_task function
void* master_task( void* task_args )
{
   // cast task arguments as Master_task_params type
   Master_task_params* master_task_params = (Master_task_params*)task_args;

   // declare and define parameters
   fio::Parameter* parameters = master_task_params->parameters;

   // declare and define workspace
   mem::Memory workspace = master_task_params->workspace;

   master_alg(
        *parameters,
         workspace,
         master_task_params->master_comm->get_dex_comm() );

   /*************************************
   ** End master-task processing
   **************************************/

   // terminate master data extraction task
   finalize_extraction( master_task_params->master_comm->get_dex_comm() );

   // tell the main master thread this task is complete
   com::tsk::barrier_wait( master_task_params->barrier );
}
