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
   int num_files = 0;
   std::string filenames[200];

   // cast task arguments as Slave_dex_params type
   Slave_dex_params* dex_params = (Slave_dex_params*)task_args;

   // alias to workspace
   mem::Memory workspace = dex_params->workspace;

   // get the output storage location
   std::string output_dir = getenv( "GPNC_OUTPUT_DIR" );

   int proc_id = dex_params->proc_id;
   int task_id = dex_params->task_id;

   int* buffer = (int*)workspace.reserve( 400 );

   // persistent request handles for meta and data
   com::proc::Request request_meta;
   com::proc::Request request_data;

   // local object to hold meta data
   Meta meta;

   int meta_tag = SLAVE_META + task_id;
   int data_tag = SLAVE_DATA + task_id;

   bool terminate = false;

   unsigned int loop_count = 0;

   // continue data extraction until termination message is received
   do {

     /************************************
      ** receive meta data from slave task
      ************************************/

      com::proc::Irecv(
            &meta,
            1,           // count
            proc_id,     // proc id
            meta_tag,    // tag
            com::proc::Comm_world,
            &request_meta );

      // wait for meta data to be received
      com::proc::wait( &request_meta );

      // get the termination message
      terminate = meta.terminate;

      // break when termination message is received
      if ( terminate ) break;

      /***********************************
       ** receive data from the slave task
       ***********************************/

      char* dst       = (char*)buffer;
      int   count     = meta.count;
      int   type_size = meta.type_size;

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

      bool initialized = false;

      // determine if the filename is new
      for (int ind = 0; ind < num_files; ind++)
      {
         if( filenames[ind] == std::string( meta.filename ))
         {
            initialized = true;
         }
      }

      if (!initialized)
      {
         filenames[num_files++] = std::string( meta.filename);
      }

      std::string output_filename = output_dir                   +
                                    "/"                          +
                                    std::string( meta.filename ) +
                                    "_";
      std::ostringstream str_id;
      str_id << proc_id;
      output_filename += str_id.str();
      str_id.clear();
      str_id.str("");
      str_id << task_id;
      output_filename += "_" + str_id.str();
      std::ofstream out_file;

      if ( !initialized ) {

         std::string meta_filename = output_filename + ".meta";
         std::ofstream meta_file;

         meta_file.open( meta_filename.c_str(), std::ios::binary);
         meta_file.write( (char*)&meta, sizeof(meta));

         meta_file.close();

         out_file.open (output_filename.c_str());
         std::cout << "Writing data to " << output_filename << std::endl;

      } else {

         out_file.open (output_filename.c_str(), std::ios::app);
      }

      out_file << buffer[0] << "\n";
      out_file.close();

   } while( !terminate );

   // tell the main thread this task is complete
   com::tsk::barrier_wait( dex_params->barrier );
}
