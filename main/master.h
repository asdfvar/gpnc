// master.h

#ifndef MASTER_H
#define MASTER_H

#include "communicator.h"
#include "master_comm.h"
#include "com.h"
#include "fio.h"
#include "memory.h"

void* master_task( void* task_args );

typedef struct {
   mem::Memory        workspace;
   fio::Parameter*    parameters;
   com::tsk::barrier* barrier;
   com::proc::Comm    dex_comm;
} Master_task_params;

#endif
