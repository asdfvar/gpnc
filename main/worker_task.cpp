// worker_task.cpp

#include "com.h"
#include "worker_task.h"
#include <iostream>

void* worker_task( void* task_args )
{
   Worker_tsk_params* worker_tsk_parameters = (Worker_tsk_params*)task_args;

   std::cout << "slave task start" << std::endl;

   com::tsk::barrier_wait( worker_tsk_parameters->barrier );
}
