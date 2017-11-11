// slave.h

#ifndef SLAVE_H
#define SLAVE_H

#include "comm_parent.h"
#include "com.h"
#include "fio.h"

void* slave_task( void* task_args );

typedef struct
{
   int                rank;
   int                task_id;
   fio::Parameter*    parameters;
   com::tsk::barrier* barrier;
} Slave_tsk_params;

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

#endif
