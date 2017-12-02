// slave_task.cpp

#include "com.h"
#include "slave.h"
#include <iostream>
#include "data_exchange.h"
#include "topology.h"

// specify the scope used here
using namespace slave;

static void slave_alg( fio::Parameter& parameters,
                       mem::Memory     workspace,
                       TOPOLOGY        topology,
                       com::proc::Comm communicator )
{

#if 1
   int* data = (int*)workspace.reserve( 10 );

   data[0] = 120 + topology.task_id;
   data[1] = 170 + topology.task_id;
   data[2] = 110 + topology.task_id;
   data[3] = 180 + topology.task_id;

   Message extracting;

   // extract data
   extracting.extract_data(
         data,       // source
         "slave_filename", // filename
         4,          // count
         topology.proc_id,
         topology.task_id,
         communicator );

   if (topology.proc_id == 2 && topology.task_id == 0) {

      data[0] = 314;
      data[1] = 315;
      data[2] = 316;
      data[3] = 317;

      Message sending;
      sending.send_to_slave(
            data,
            4, //int   count,
            3, //int   proc_id_dst,
            0, //int   task_id_dst,
            topology.num_procs, //int   num_procs,
            topology.num_tasks ); //int   num_tasks )

      sending.wait();

   } else if (topology.proc_id == 3 && topology.task_id == 0) {

      Message receiving;
      receiving.receive_from_slave(
            data,
            4, //int   count,
            2, //int   proc_id_src,
            0, //int   task_id_src,
            topology.proc_id, //int   proc_id_dst,
            topology.task_id, //int   task_id_dst,
            topology.num_procs, //int   num_procs,
            topology.num_tasks ); //int   num_tasks )

      receiving.wait();

      std::cout << "data = " << data[0] << ", "
         << data[1] << ", "
         << data[2] << ", "
         << data[3] << std::endl;
   }
#endif
}

// slave_task function
void* slave_task( void* task_args )
{
   Slave_tsk_params* slave_tsk_parameters = (Slave_tsk_params*)task_args;

   int proc_id   = slave_tsk_parameters->proc_id;
   int task_id   = slave_tsk_parameters->task_id;
   int num_procs = slave_tsk_parameters->num_procs;
   int num_tasks = slave_tsk_parameters->num_tasks;

   TOPOLOGY topology;

   topology.type      = 0;
   topology.proc_id   = slave_tsk_parameters->proc_id;
   topology.task_id   = slave_tsk_parameters->task_id;
   topology.num_procs = slave_tsk_parameters->num_procs;
   topology.num_tasks = slave_tsk_parameters->num_tasks;

   fio::Parameter* parameters = slave_tsk_parameters->parameters;
   mem::Memory     workspace  = slave_tsk_parameters->workspace;

   slave_alg( *parameters,
               workspace,
               topology,
               com::proc::Comm_world );

   /*
   ** End slave-task processing
   */

   // TODO:
   // terminate slave data extraction task
   finalize_extraction( com::proc::Comm_world, task_id );

   com::tsk::barrier_wait( slave_tsk_parameters->barrier );
}
