// master_dex.cpp

#include "master_dex.h"
#include "com.h"
#include "mem.h"
#include "proc_maps.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

void* master_dex_task( void* task_args )
{
   // cast task arguments as Master_dex_params type
   Master_dex_params* dex_params = (Master_dex_params*)task_args;

   // alias to workspace
   mem::Memory workspace = dex_params->workspace;

   // get the output storage location
   std::string output_dir = getenv( "GPNC_OUTPUT_DIR" );

   int* buffer = (int*)workspace.reserve( 400 );

   // persistent request handles for meta and data
   com::proc::Request request_meta;
   com::proc::Request request_data;

   // local object to hold meta data
   Meta meta;

   bool terminate = false;

   do {

     /****************************************
      ** receive meta data from master process
      ****************************************/

      com::proc::Irecv(
           &meta,
            1,            // count
            0,            // proc id
            MASTER_META,  // tag
            dex_params->master_comm,
            &request_meta );

      // wait for meta data to be received
      com::proc::wait( &request_meta );

      // check if this receive is a termination message
      terminate = meta.terminate;

      // break when termination message is received
      if ( terminate ) break;

     /************************************
      ** receive data from the master task
      ************************************/

      // get data size
      int count     = meta.count;
      int type_size = meta.type_size;

      char* dst = (char*)buffer;

      // receive data
      com::proc::Irecv(
            dst,                // data destination
            count * type_size,  // count
            0,                  // proc id
            MASTER_DATA,        // tag
            dex_params->master_comm,
            &request_data );

      // wait for data to be received
      com::proc::wait( &request_data );

     /*********************
      ** write data to disk
      *********************/

      std::string output_filename = output_dir + "/master_data";
      std::ofstream out_file;

      bool init = true;
      if (init) {
         out_file.open (output_filename.c_str());
         std::cout << "writing data to " << output_filename << std::endl;
      } else {
         out_file.open (output_filename.c_str(), std::ios::app);
      }
      out_file << buffer[0] << "\n";
      out_file.close();

   } while( !terminate );

   // tell the main thread this task is complete
   com::tsk::barrier_wait( dex_params->barrier );
}
