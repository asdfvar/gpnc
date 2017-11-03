// master_dex.cpp

#include "master_dex.h"
#include "com.h"
#include "proc_maps.h"
#include <iostream>

void* master_dex_task( void* task_args )
{
   // cast task arguments as Master_dex_params type
   Master_dex_params* master_dex_params = (Master_dex_params*)task_args;

   // receive meta data
   com::proc::Request request;
   Meta master_meta;

   // announce ourselves
   std::cout << "master DEX task processing start" << std::endl;

   bool finished = false;

   do {

std::cout << "ABOUT TO RECEIVE META DATA" << std::endl;
      // receive meta data from master process
      com::proc::Irecv(
           &master_meta,
            1,            // count
            0,            // proc id
            MASTER_META,  // tag
            master_dex_params->master_comm,
            &request );

      // wait for meta data to be received
      com::proc::wait( &request );
std::cout << "RECEIVED META DATA" << std::endl;

      // check if this receive is a termination message
      finished = master_meta.finished;

      if ( !finished )
      {
         // receive data

         // check if this data extraction exists

         // write it to file
      }

   } while( !finished );

   // tell the main thread this task is complete
   com::tsk::barrier_wait( master_dex_params->barrier );
}
