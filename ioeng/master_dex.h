// master_dex.h

#ifndef MASTER_DEX_H
#define MASTER_DEX_H

#include "com.h"
#include "mem.h"

void* master_dex_task( void* task_args );

typedef struct
{
   com::proc::Comm    master_comm;
   com::tsk::barrier* barrier;
   mem::Memory        workspace;
} Master_dex_params;

#endif
