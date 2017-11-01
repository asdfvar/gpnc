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

   bool done = false;

   do {

      // receive meta data
#if 0
      com::proc::Irecv(
            (float*)buf,
            4,            // count
            0,            // proc id
            1,            // tag
            master_comm,
            &request );

      std::cout << "receiving" << std::endl;

      com::proc::wait( &request );
#endif
      // check if this data extraction exists

      // write it to file

      done = true;

   } while( !done );

   // tell the main thread this task is complete
   com::tsk::barrier_wait( master_dex_params->barrier );
}
