#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "com.h"

typedef struct
{
   int    par_int;
   float  par_float;
   double par_double;
} Inp_params;

typedef struct
{
   int               par_int;
   float             par_float;
   double            par_double;
   com::tsk_barrier* barrier;
} Worker_tsk_params;

#endif
