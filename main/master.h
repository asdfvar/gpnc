// master.h

#ifndef MASTER_H
#define MASTER_H

#include "comm_parent.h"
#include "com.h"
#include "fio.h"
#include "memory.h"

class Master_comm : public Comm_parent {

   public:

      // constructor
      Master_comm( int argc, char* argv[] );

      // get data extraction communicator handle
      com::proc::Comm get_dex_comm( void );

      // finalize communication setup
      void finalize( void );

      // destructor
      ~Master_comm( void );

   private:

      // communication handle for local master
      com::proc::Comm my_comm;

      // communication handle to the data extraction driver
      com::proc::Comm dex_comm;
};

void* master_task( void* task_args );

typedef struct {
   mem::Memory        workspace;
   fio::Parameter*    parameters;
   com::tsk::barrier* barrier;
   Master_comm*       master_comm;
} Master_task_params;

#endif
