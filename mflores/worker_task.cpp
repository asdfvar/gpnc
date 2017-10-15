#include "com.h"
#include "worker_task.h"
#include "parameters.h"
#include <iostream>

void* worker_task( void* task_args )
{
   Worker_tsk_params* worker_tsk_parameters = (Worker_tsk_params*)task_args;
   
   std::cout << __FILE__ << ":" << __LINE__ << ":execution from slave task" << std::endl;

   std::cout << "BEFORE barrier" << std::endl;
   com::tsk::barrier_wait( worker_tsk_parameters->barrier );
}
