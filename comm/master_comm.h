// master_comm.h

#ifndef MASTER_COMM_H
#define MASTER_COMM_H

#include "communicator.h"
#include "com.h"
#include "memory.h"

class Master_comm : public Communicator {

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

#endif
