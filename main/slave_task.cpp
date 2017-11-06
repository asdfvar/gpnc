// slave_task.cpp

#include "com.h"
#include "slave.h"
#include <iostream>

void* slave_task( void* task_args )
{
   Slave_tsk_params* slave_tsk_parameters = (Slave_tsk_params*)task_args;

   std::cout << "slave task start" << std::endl;

   com::tsk::barrier_wait( slave_tsk_parameters->barrier );
}
