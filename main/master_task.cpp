// master_task.cpp

#include "master.h"
#include "extract_data.h"
#include "fio.h"
#include <iostream>

void* master_task( void* task_args )
{
   // cast task arguments as Master_task_params type
   Master_task_params* master_task_params = (Master_task_params*)task_args;

   // declare and define alias to parameters object
   fio::Parameter* parameters = master_task_params->parameters;

   // announce ourselves
   std::cout << "start master-task processing" << std::endl;

   /*
   ** Begin primary master-task processing
   */

   int    par_int    = parameters->get_int(  "parameter_int"    );
   float  par_float  = parameters->get_real( "parameter_float"  );
   double par_double = parameters->get_real( "parameter_double" );

   std::cout << "par_int    = " << par_int    << std::endl;
   std::cout << "par_float  = " << par_float  << std::endl;
   std::cout << "par_double = " << par_double << std::endl;

   int* data = (int*)master_task_params->workspace.reserve( 10 );

   data[0] = 2;
   data[1] = 7;
   data[2] = 1;
   data[3] = 8;

   // extract data
   extract_data(
         data,       // source
         "filename", // filename
         4,          // count
         master_task_params->master_comm->get_dex_comm() );

   /*
   ** End master-task processing
   */

   // terminate master data extraction task
   finalize_extraction( master_task_params->master_comm->get_dex_comm() );

   // tell the main thread this task is complete
   com::tsk::barrier_wait( master_task_params->barrier );
}
