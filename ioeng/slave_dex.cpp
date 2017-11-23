// slave_dex.cpp

#include "proc_maps.h"
#include "slave_dex.h"
#include "com.h"
#include <iostream>
#include <stdlib.h>

void* slave_dex_task( void* task_args )
{
   // cast task arguments as Slave_dex_params type
   Slave_dex_params* slave_dex_params = (Slave_dex_params*)task_args;

   // announce ourselves
   std::cout << "slave DEX task processing start" << std::endl;

   int proc_id = slave_dex_params->proc_id;
   int task_id = slave_dex_params->task_id;

   // TODO: use workbuffer
   int* data = new int[1024];

   Meta slave_meta;
   com::proc::Request request_meta;
   com::proc::Request request_data;

   bool terminate = false;
std::cout << __FILE__ << ":" << __LINE__ << ":got here task " << task_id << std::endl;

   do {

      // receive meta data from slave process
      com::proc::Irecv(
            &slave_meta,
            1,           // count
            proc_id,     // proc id
            SLAVE_META + 1000 + task_id,  // tag
//            slave_dex_params->slave_comm,
            com::proc::Comm_world,
            &request_meta );

      // wait for meta data to be received
      com::proc::wait( &request_meta );

      terminate = slave_meta.terminate;
std::cout << __FILE__ << ":" << __LINE__ << ":got here task " << task_id << std::endl;

#if 0
      if ( !terminate )
      {

         char* dst     = (char*)data;
         int count     = slave_meta.count;
         int type_size = slave_meta.type_size;

         // receive data
         com::proc::Irecv(
               dst,                // data destination
               count * type_size,  // count
               proc_id,                  // proc id
               SLAVE_DATA + 2000 + task_id,        // tag
               slave_dex_params->slave_comm,
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
#endif

   } while( !terminate );

   delete[] data;

std::cout << __FILE__ << ":" << __LINE__ << ":got here task " << task_id << std::endl;
   // tell the main thread this task is complete
   com::tsk::barrier_wait( slave_dex_params->barrier );
}
