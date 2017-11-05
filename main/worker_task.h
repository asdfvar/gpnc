#ifndef SLAVE_TASK_H
#define SLAVE_TASK_H

#include "com.h"

void* worker_task( void* task_args );

typedef struct
{
   int                par_int;
   float              par_float;
   double             par_double;
   com::tsk::barrier* barrier;
} Worker_tsk_params;

#endif
