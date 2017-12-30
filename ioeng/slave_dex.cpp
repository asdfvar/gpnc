// slave_dex.cpp

#include "mem.h"
#include "proc_maps.h"
#include "slave_dex.h"
#include "com.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>

void* slave_dex_task( void* task_args )
{
   // cast task arguments as Slave_dex_params type
   Slave_dex_params* slave_dex_params = (Slave_dex_params*)task_args;

   // get the output storage location
   std::string output_dir = getenv( "GPNC_OUTPUT_DIR" );

   int proc_id = slave_dex_params->proc_id;
   int task_id = slave_dex_params->task_id;

   int* data = (int*)slave_dex_params->workspace.reserve( 400 );

   Meta slave_meta;
   com::proc::Request request_meta;
   com::proc::Request request_data;

   int meta_tag = SLAVE_META + task_id;
   int data_tag = SLAVE_DATA + task_id;

   bool terminate = false;

   do {

     /************************************
      ** receive meta data from slave task
      ************************************/

      com::proc::Irecv(
            &slave_meta,
            1,           // count
            proc_id,     // proc id
            meta_tag,    // tag
            com::proc::Comm_world,
            &request_meta );

      // wait for meta data to be received
      com::proc::wait( &request_meta );

      // get the termination message
      terminate = slave_meta.terminate;

      // break when termination message is received
      if ( terminate ) break;

      /***********************************
       ** receive data from the slave task
       ***********************************/

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

      /*********************
       ** write data to disk
       *********************/

      std::cout << "writing data for proc id " <<
         proc_id << " and task id " <<
         task_id << std::endl;

      std::string output_filename = output_dir + "/slave_proc_";

      std::ostringstream str_id;

      str_id << proc_id;

      output_filename += str_id.str();

      str_id.clear();
      str_id.str("");
      str_id << task_id;

     output_filename += "_" + str_id.str();

      std::ofstream out_file;
      bool init = true;
      if (init) {
         out_file.open (output_filename.c_str());
         std::cout << "Writing data to " << output_filename << std::endl;
      } else {
         out_file.open (output_filename.c_str(), std::ios::app);
      }
      out_file << data[0] << "\n";
      out_file.close();

   } while( !terminate );

   // tell the main thread this task is complete
   com::tsk::barrier_wait( slave_dex_params->barrier );
}
