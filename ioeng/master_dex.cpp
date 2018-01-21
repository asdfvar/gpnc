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

   int num_files = 0;
   std::string filenames[200];

   // cast task arguments as Master_dex_params type
   Master_dex_params* dex_params = (Master_dex_params*)task_args;

   // alias to workspace
   mem::Memory workspace = dex_params->workspace;

   // get the output storage location
   std::string output_dir = getenv( "GPNC_OUTPUT_DIR" );

   // reserve memory space based on how much was allocated
   std::string str_mem_size_bytes = getenv( "GPNC_MASTER_MEM" );
   int mem_size_words = atoi( str_mem_size_bytes.c_str() ) / 4;
   int* buffer = (int*)workspace.reserve( mem_size_words );

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

      bool initialized = false;

      // determine if the filename is new
      for (int ind = 0; ind < num_files; ind++)
      {
         if( filenames[ind] == std::string( meta.filename ))
         {
            initialized = true;
         }
      }

      if ( !initialized )
      {
         filenames[num_files++] = std::string( meta.filename );
      }

      std::string output_filename = output_dir + "/" +
                                    std::string( meta.filename );
      std::ofstream out_file;

      // a new file is created if this is the first instance of
      // this data being written to disk. Otherwise, the data
      // is appended to the existing file

      if (!initialized) {
         out_file.open (output_filename.c_str(), std::ios::binary);
         std::cout << "writing data to " << output_filename << std::endl;
      } else {
         out_file.open (output_filename.c_str(), std::ios::app);
      }
      out_file.write( (char*)buffer, meta.count * sizeof(int) );
      out_file.close();

      std::string meta_filename = output_filename + ".meta";
      std::ofstream meta_file;

      meta_file.open( meta_filename.c_str(), std::ios::binary);
      meta_file.write( (char*)&meta, sizeof(meta));

      meta_file.close();

   } while( !terminate );

   // tell the main thread this task is complete
   com::tsk::barrier_wait( dex_params->barrier );
}
