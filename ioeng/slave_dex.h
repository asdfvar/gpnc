// slave_dex.h

#ifndef SLAVE_DEX_H
#define SLAVE_DEX_H

#include "com.h"
#include "memory.h"

void* slave_dex_task( void* task_args );

typedef struct
{
   int                proc_id;
   int                task_id;
   com::tsk::barrier* barrier;
   com::proc::Comm    slave_comm;
   mem::Memory        workspace;
} Slave_dex_params;

#endif
