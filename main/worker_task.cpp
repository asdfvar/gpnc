// worker_task.cpp

#include "com.h"
#include "worker_task.h"
#include "parameters.h"
#include <iostream>

void* worker_task( void* task_args )
{
   Worker_tsk_params* worker_tsk_parameters = (Worker_tsk_params*)task_args;

   com::tsk::barrier_wait( worker_tsk_parameters->barrier );
}
