// slave.cpp

#include "com.h"
#include "fio.h"
#include "memory.h"
#include "slave.h"
#include "proc_maps.h"
#include <iostream>
#include <stdlib.h>

int main( int argc, char* argv[] )
{

   // setup process communications
   Slave_comm slave_comm( argc, argv );

   // read parameter file
   fio::Parameter parameters( getenv( "GPNC_PARAMS" ) );

   // get the number of slave tasks
   int num_procs = atoi(getenv( "GPNC_NUM_SLAVE_PROCS" ));

   // get the number of slave tasks
   int num_tasks = atoi(getenv( "GPNC_NUM_SLAVE_TASKS" ));

   com::tsk::handler slave_tsk_handle;
   com::tsk::barrier slave_barrier;
   com::tsk::barrier_init( &slave_barrier, num_tasks + 1 );

   // declare the slave-task parameters
   Slave_tsk_params* slave_tsk_parameters = new Slave_tsk_params[num_tasks];

   // define memory size for slave-task processing
   std::string str_mem_size = getenv( "GPNC_SLAVE_MEM" );
   int mem_size = atoi( str_mem_size.c_str() );
   int mem_size_words = (mem_size + 4) / 4;

   for (int task = 0; task < num_tasks; task++)
   {

      // populate the slave-task parameters
      slave_tsk_parameters[task].proc_id     = slave_comm.get_global_rank();
      slave_tsk_parameters[task].task_id     = task;
      slave_tsk_parameters[task].parameters  = &parameters;
      slave_tsk_parameters[task].barrier     = &slave_barrier;
      slave_tsk_parameters[task].slave_comm  = &slave_comm;
      slave_tsk_parameters[task].num_procs   = num_procs;
      slave_tsk_parameters[task].num_tasks   = num_tasks;
      slave_tsk_parameters[task].workspace   = mem::Memory( mem_size_words, "slave" );

      // start the slave tasks
      com::tsk::create(
            &slave_tsk_handle,
            slave_task,
            (void*)&slave_tsk_parameters[task] );
   }

   // wait for slave task to finish
   com::tsk::barrier_wait( &slave_barrier );

   /**************************************************************************
   * finish processing
   ***************************************************************************/

   // wait for all processes to sync before closing down
   com::proc::Barrier( com::proc::Comm_world );

   // destroy thread barrier
   com::tsk::barrier_destroy( &slave_barrier );

   // suspend execution of the slave task
   com::tsk::join( slave_tsk_handle );

   // free workspace memory
   for (int task = 0; task < num_tasks; task++)
   {
      slave_tsk_parameters[task].workspace.finalize();
   }

   // free slave task parameters
   delete[] slave_tsk_parameters;

   // finalize process communication
   slave_comm.finalize();

   return 0;
}
