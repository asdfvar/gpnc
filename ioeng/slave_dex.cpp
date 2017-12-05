// slave_dex.cpp

#include "mem.h"
#include "proc_maps.h"
#include "slave_dex.h"
#include "com.h"
#include <iostream>
#include <stdlib.h>

void* slave_dex_task( void* task_args )
{
   // cast task arguments as Slave_dex_params type
   Slave_dex_params* slave_dex_params = (Slave_dex_params*)task_args;

   int proc_id = slave_dex_params->proc_id;
   int task_id = slave_dex_params->task_id;

   int* data = (int*)slave_dex_params->workspace.reserve(100);

   Meta slave_meta;
   com::proc::Request request_meta;
   com::proc::Request request_data;

   int meta_tag = SLAVE_META + task_id;
   int data_tag = SLAVE_DATA + task_id;

   bool terminate = false;

   do {

      // receive meta data from slave process
      com::proc::Irecv(
            &slave_meta,
            1,           // count
            proc_id,     // proc id
            meta_tag,    // tag
            com::proc::Comm_world,
            &request_meta );

      // wait for meta data to be received
      com::proc::wait( &request_meta );

      terminate = slave_meta.terminate;

      if ( !terminate )
      {

         char* dst       = (char*)data;
         int   count     = slave_meta.count;
         int   type_size = slave_meta.type_size;

         // receive data
         com::proc::Irecv(
               dst,                // data destination
               count * type_size,  // count
               proc_id,            // proc id
               data_tag,           // tag
               com::proc::Comm_world,
               &request_data );

         // wait for data to be received
         com::proc::wait( &request_data );

         int* data_int = static_cast<int*>(data);

         std::cout << "data = ";
         std::cout << data_int[0] << ", ";
         std::cout << data_int[1] << ", ";
         std::cout << data_int[2] << ", ";
         std::cout << data_int[3] << std::endl;

      }

   } while( !terminate );

   // tell the main thread this task is complete
   com::tsk::barrier_wait( slave_dex_params->barrier );
}
