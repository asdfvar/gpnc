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

   // define memory size for slave-task processing
   size_t mem_size = parameters.get_int( "memory_size_slave" );

   // declare and define workspace
   mem::Memory workspace( mem_size );

   // get the number of slave tasks
   int num_slave_procs = atoi(getenv( "GPNC_NUM_SLAVE_PROCS" ));

   // get the number of slave tasks
   int num_slave_tasks = atoi(getenv( "GPNC_NUM_SLAVE_TASKS" ));

   com::tsk::handler slave_tsk_handle;
   com::tsk::barrier slave_barrier;
   com::tsk::barrier_init( &slave_barrier, num_slave_tasks + 1 );

   // declare the slave-task parameters
   Slave_tsk_params* slave_tsk_parameters = new Slave_tsk_params[num_slave_tasks];

   for (int task_num = 0; task_num < num_slave_tasks; task_num++)
   {
      // populate the slave-task parameters
      slave_tsk_parameters[task_num].proc_id         = slave_comm.get_global_rank();
      slave_tsk_parameters[task_num].task_id         = task_num;
      slave_tsk_parameters[task_num].parameters      = &parameters;
      slave_tsk_parameters[task_num].barrier         = &slave_barrier;
      slave_tsk_parameters[task_num].slave_comm      = &slave_comm;
      slave_tsk_parameters[task_num].num_slave_procs = num_slave_procs;
      slave_tsk_parameters[task_num].num_slave_tasks = num_slave_tasks;

      // start the slave tasks
      com::tsk::create(
            &slave_tsk_handle,
            slave_task,
            (void*)&slave_tsk_parameters[task_num] );
   }

   // wait for slave task to finish
   com::tsk::barrier_wait( &slave_barrier );
   std::cout << "AFTER barrier" << std::endl;

   /**************************************************************************
   * finish processing
   ***************************************************************************/

   // wait for all processes to sync before closing down
   com::proc::Barrier( com::proc::Comm_world );

   delete[] slave_tsk_parameters;

   // destroy thread barrier
   com::tsk::barrier_destroy( &slave_barrier );

   // suspend execution of the slave task
   com::tsk::join( slave_tsk_handle );

   // finalize process communication
   slave_comm.finalize();

   // free workspace memory from heap
   workspace.finalize();

   return 0;
}
