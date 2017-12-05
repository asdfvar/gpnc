// slave.h

#ifndef SLAVE_H
#define SLAVE_H

#include "communicator.h"
#include "slave_comm.h"
#include "com.h"
#include "fio.h"
#include "mem.h"

void* slave_task( void* task_args );

typedef struct
{
   int                proc_id;
   int                task_id;
   mem::Memory        workspace;
   fio::Parameter*    parameters;
   com::tsk::barrier* barrier;
   int                num_procs;
   int                num_tasks;
} Slave_tsk_params;

#endif
