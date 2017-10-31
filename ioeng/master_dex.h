// master_dex.h

#ifndef MASTER_DEX_H
#define MASTER_DEX_H

#include "com.h"

void* master_dex_task( void* task_args );

typedef struct
{
   com::tsk::barrier* barrier;
} Master_dex_params;

#endif
