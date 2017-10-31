// slave_dex.h

#ifndef SLAVE_DEX_H
#define SLAVE_DEX_H

#include "com.h"

void* slave_dex_task( void* task_args );

typedef struct
{
   com::tsk::barrier* barrier;
} Slave_dex_params;

#endif
