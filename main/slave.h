// slave.h

#ifndef SLAVE_H
#define SLAVE_H

#include "comm_parent.h"
#include "com.h"
#include "fio.h"
#include "memory.h"

void* slave_task( void* task_args );

class Slave_comm : public Comm_parent {

   public:

      // constructor
      Slave_comm( int argc, char* argv[] );

      // get data extraction communicator handle
      com::proc::Comm get_dex_comm( void );

      // finalize communication setup
      void finalize( void );

      // destructor
      ~Slave_comm( void );

   private:

      // communication handle for local slave
      com::proc::Comm my_comm;

      // communication handle to the data extraction driver
      com::proc::Comm dex_comm;
};

typedef struct
{
   int                proc_id;
   int                task_id;
   mem::Memory        workspace;
   fio::Parameter*    parameters;
   com::tsk::barrier* barrier;
   Slave_comm*        slave_comm;
} Slave_tsk_params;

#endif
