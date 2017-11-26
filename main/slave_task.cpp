// slave_task.cpp

#include "com.h"
#include "slave.h"
#include <iostream>
#include "extract_data.h"
#include "data_exchange.h"

// specify the scope used here
using namespace slave;

void* slave_task( void* task_args )
{
   Slave_tsk_params* slave_tsk_parameters = (Slave_tsk_params*)task_args;

   int proc_id   = slave_tsk_parameters->proc_id;
   int task_id   = slave_tsk_parameters->task_id;
   int num_procs = slave_tsk_parameters->num_slave_procs;
   int num_tasks = slave_tsk_parameters->num_slave_tasks;

// TODO:
//   int* data = (int*)slave_tsk_parameters->workspace.reserve( 10 );
   int* data = new int[10];

   data[0] = 120 + task_id;
   data[1] = 170 + task_id;
   data[2] = 110 + task_id;
   data[3] = 180 + task_id;


#if 1
// TODO:
//      slave_tsk_parameters.task_id = task_num;
   // extract data
   extract_data(
         data,       // source
         "slave_filename", // filename
         4,          // count
         proc_id,
         task_id,
         com::proc::Comm_world );
#endif

#if 1
if (proc_id == 2 && task_id == 0) {

   data[0] = 314;
   data[1] = 315;
   data[2] = 316;
   data[3] = 317;

      send_to_slave(
            data,
            4, //int   count,
            3, //int   proc_id_dst,
            0, //int   task_id_dst,
            num_procs, //int   num_procs,
            num_tasks ); //int   num_tasks )

} else if (proc_id == 3 && task_id == 0) {

      receive_from_slave(
            data,
            4, //int   count,
            2, //int   proc_id_src,
            0, //int   task_id_src,
            proc_id, //int   proc_id_dst,
            task_id, //int   task_id_dst,
            num_procs, //int   num_procs,
            num_tasks ); //int   num_tasks )

std::cout << "data = " << data[0] << ", " << data[1] << ", " << data[2] << ", " << data[3] << std::endl;
}
#endif

   /*
   ** End master-task processing
   */
   // TODO:
   delete[] data;

   // TODO:
   // terminate slave data extraction task
   finalize_extraction( com::proc::Comm_world, task_id );

   com::tsk::barrier_wait( slave_tsk_parameters->barrier );
}
