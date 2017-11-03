// master_dex.cpp

#include "master_dex.h"
#include "com.h"
#include "proc_maps.h"
#include <iostream>

void* master_dex_task( void* task_args )
{
   // cast task arguments as Master_dex_params type
   Master_dex_params* master_dex_params = (Master_dex_params*)task_args;

   // announce ourselves
   std::cout << "master DEX task processing start" << std::endl;

   bool finished = false;

   do {

      // receive meta data
#if 1
      com::proc::Request request;
      Meta master_meta;

float data;
std::cout << "ABOUT TO RECEIVE DATA" << std::endl;
      com::proc::Irecv(
#if 0
           &master_meta,
#else
           &data,
#endif
            1,            // count
            0,            // proc id
            MASTER_META,  // tag
            master_dex_params->master_comm,
            &request );

      std::cout << "receiving from master" << std::endl;

      com::proc::wait( &request );
std::cout << "DATA RECEIVED: " << data << std::endl;
#endif
      // check if this data extraction exists

      // write it to file

      finished = true;

   } while( !finished );

   // tell the main thread this task is complete
   com::tsk::barrier_wait( master_dex_params->barrier );
}
