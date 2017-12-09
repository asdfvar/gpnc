// data_extraction.cpp

#include "com.h"
#include "fio.h"
#include "mem.h"
#include "proc_maps.h"
#include "dex_comm.h"
#include <iostream>
#include <stdlib.h>
#include "master_dex.h"
#include "slave_dex.h"

using namespace dex;

int main( int argc, char* argv[] )
{

   // get global rank and number of procs
   int global_rank;
   int numprocs;

   // initialize proc-to-proc communication
   Comm_setup dex_comm( argc, argv );

   // get the memory size allocation from the environment variables
   std::string str_mem_size = getenv( "GPNC_IOENG_MEM" );
   int mem_size = atoi( str_mem_size.c_str() );
   int mem_size_words = (mem_size + 4) / 4;

   // declare the master DEX task handle
   com::tsk::handler master_dex_handle;

   // declare the master DEX barrier
   com::tsk::barrier master_dex_barrier;

   // declare the master DEX parameters
   Master_dex_params master_dex_params;

   // initialize the master DEX barrier
   com::tsk::barrier_init( &master_dex_barrier, 2 );

   master_dex_params.master_comm = dex_comm.get_dex_comm();
   master_dex_params.workspace   = mem::Memory( mem_size_words, "IO engine" );
   master_dex_params.barrier     = &master_dex_barrier;

   // start master data extraction task
   com::tsk::create(
         &master_dex_handle,
         master_dex_task,
         (void*)&master_dex_params );

   // declare the slave DEX task handle
   com::tsk::handler slave_dex_handle;

   // declare the slave DEX barrier
   com::tsk::barrier slave_dex_barrier;

   // get the number of slave processes from the environment variables
   std::string str_num_slave_procs = getenv( "GPNC_NUM_SLAVE_PROCS" );
   int num_slave_procs = atoi( str_num_slave_procs.c_str() );

   // get the number of slave tasks per process from the environment variables
   std::string str_num_slave_tasks = getenv( "GPNC_NUM_SLAVE_TASKS" );
   int num_slave_tasks = atoi( str_num_slave_tasks.c_str() );

   // declare the slave DEX parameters
   Slave_dex_params* slave_dex_params = new Slave_dex_params[num_slave_procs * num_slave_tasks];

   // initialize the slave DEX barrier
   com::tsk::barrier_init( &slave_dex_barrier, num_slave_procs * num_slave_tasks + 1 );

   // create a new task to correspond to each slave task that exists
   for (int slave_proc = 0; slave_proc < num_slave_procs; slave_proc++)
   {

      for (int slave_task = 0; slave_task < num_slave_tasks; slave_task++)
      {

         int index = slave_task + slave_proc * num_slave_tasks;
         slave_dex_params[index].barrier   = &slave_dex_barrier;
         slave_dex_params[index].proc_id   = slave_proc + SLAVE_GROUP;
         slave_dex_params[index].task_id   = slave_task;
         slave_dex_params[index].workspace = mem::Memory( mem_size_words, "IO engine" );

         // start slave data extraction task
         com::tsk::create(
               &slave_dex_handle,
               slave_dex_task,
               (void*)&slave_dex_params[index] );
      }
   }

   // wait for the master DEX task to finish
   com::tsk::barrier_wait( &master_dex_barrier );
   std::cout << "master DEX task processing complete" << std::endl;

   // wait for the slave DEX task to finish
   com::tsk::barrier_wait( &slave_dex_barrier );
   std::cout << "slave DEX task processing complete" << std::endl;

   /********************************************
    ** close down and finalize DEX processing **
    *******************************************/

   // wait for all processes to sync before closing down
   com::proc::Barrier( com::proc::Comm_world );

   // destroy master DEX barrier
   com::tsk::barrier_destroy( &master_dex_barrier );

   // destroy slave DEX barrier
   com::tsk::barrier_destroy( &slave_dex_barrier );

   // free workspace memory from master DEX processing
   master_dex_params.workspace.finalize();

   // free workspace memory from slave DEX processing
   for (int task = 0; task < num_slave_procs * num_slave_tasks; task++)
   {
         slave_dex_params[task].workspace.finalize();
   }

   // finalize process communication
   dex_comm.finalize();

   return 0;
}

#if 0
  /*
   * Function NAME: write_val
   */
  void write_val(float val, std::string out_file_path, bool init)
  {

    std::ofstream out_file;
    if (init) {
       out_file.open (out_file_path.c_str());
       std::cout << "Writing data to " << out_file_path << std::endl;
    } else {
       out_file.open (out_file_path.c_str(), std::ios::app);
    }
    out_file << val << "\n";
    out_file.close();

  }
#endif
