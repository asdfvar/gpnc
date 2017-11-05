// slave_dex.cpp

#include "slave_dex.h"
#include "com.h"
#include <iostream>

void* slave_dex_task( void* task_args )
{
   // cast task arguments as Slave_dex_params type
   Slave_dex_params* slave_dex_params = (Slave_dex_params*)task_args;

   // announce ourselves
   std::cout << "slave DEX task processing start" << std::endl;

   // tell the main thread this task is complete
   com::tsk::barrier_wait( slave_dex_params->barrier );
}
