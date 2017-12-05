// master_dex.cpp

#include "master_dex.h"
#include "com.h"
#include "mem.h"
#include "proc_maps.h"
#include <iostream>

void* master_dex_task( void* task_args )
{
   // cast task arguments as Master_dex_params type
   Master_dex_params* master_dex_params = (Master_dex_params*)task_args;

   mem::Memory workspace = master_dex_params->workspace;

   int* data = (int*)workspace.reserve( 20 );

   // receive meta data
   com::proc::Request request_meta;
   com::proc::Request request_data;
   Meta master_meta;

   bool terminate = false;

   do {

      // receive meta data from master process
      com::proc::Irecv(
           &master_meta,
            1,            // count
            0,            // proc id
            MASTER_META,  // tag
            master_dex_params->master_comm,
            &request_meta );

      // wait for meta data to be received
      com::proc::wait( &request_meta );

      // check if this receive is a termination message
      terminate = master_meta.terminate;

      if ( !terminate )
      {

         // get data size
         int count     = master_meta.count;
         int type_size = master_meta.type_size;

         char* dst = (char*)data;

         // receive data
         com::proc::Irecv(
               dst,                // data destination
               count * type_size,  // count
               0,                  // proc id
               MASTER_DATA,        // tag
               master_dex_params->master_comm,
               &request_data );

         // wait for data to be received
         com::proc::wait( &request_data );

         int* data_int = static_cast<int*>(data);

         std::cout << "extraction data = ";
         std::cout << data_int[0] << ", ";
         std::cout << data_int[1] << ", ";
         std::cout << data_int[2] << ", ";
         std::cout << data_int[3] << std::endl;

         // check if this data extraction exists

         // write it to file
      }

   } while( !terminate );

   // tell the main thread this task is complete
   com::tsk::barrier_wait( master_dex_params->barrier );
}
