// master_dex.cpp

#include "master_dex.h"
#include "com.h"
#include <iostream>

void* master_dex_task( void* task_args )
{
   // cast task arguments as Master_dex_params type
   Master_dex_params* master_dex_params = (Master_dex_params*)task_args;

   // announce ourselves
   std::cout << "master DEX task processing start" << std::endl;

   // tell the main thread this task is complete
   com::tsk::barrier_wait( master_dex_params->barrier );
}
