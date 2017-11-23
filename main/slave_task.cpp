// slave_task.cpp

#include "com.h"
#include "slave.h"
#include <iostream>
#include "extract_data.h"

// specify that the scope is of slave processing
using namespace slave;

void* slave_task( void* task_args )
{
   Slave_tsk_params* slave_tsk_parameters = (Slave_tsk_params*)task_args;

   std::cout << "slave task start" << std::endl;

// TODO:
//   int* data = (int*)slave_tsk_parameters->workspace.reserve( 10 );
   int* data = new int[10];

   data[0] = 12;
   data[1] = 17;
   data[2] = 11;
   data[3] = 18;

   int proc_id = slave_tsk_parameters->proc_id;
   int task_id = slave_tsk_parameters->task_id;


// TODO:
//      slave_tsk_parameters.task_id = task_num;
   // extract data
   extract_data(
         data,       // source
         "slave_filename", // filename
         4,          // count
         proc_id,
         task_id,
         slave_tsk_parameters->slave_comm->get_dex_comm() );

   /*
   ** End master-task processing
   */
   // TODO:
   delete[] data;

   // terminate slave data extraction task
//   finalize_extraction( slave_tsk_parameters->slave_comm->get_dex_comm() );
   finalize_extraction( com::proc::Comm_world, task_id );

   com::tsk::barrier_wait( slave_tsk_parameters->barrier );
}
