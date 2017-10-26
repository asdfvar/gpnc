#ifndef COMM_SETUP_H
#define COMM_SETUP_H

#include "com.h"

class Comm_setup {

   public:

      // constructor
      Comm_setup( int argc, char* argv[] );

      // get data extraction communicator handle
      com::proc::Comm get_dex_comm( void );

      // finalize communication setup
      void finalize( void );

      // destructor
      ~Comm_setup( void );

   private:

      // number of global processors
      int numprocs;

      // global rank
      int global_rank;

      // local rank
      int local_rank;

      // communication handle for local master
      com::proc::Comm my_comm;

      // communication handle to the data extraction driver
      com::proc::Comm dex_comm;
};

#endif
