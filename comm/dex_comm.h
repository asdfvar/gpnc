// dex_comm.h

#ifndef DEX_COMM_H
#define DEX_COMM_H

#include "communicator.h"
#include "com.h"
#include "mem.h"

namespace dex {

   class Comm_setup : public Communicator {

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

         // communication handle for local data extraction
         com::proc::Comm my_comm;

         // communication handle to the data extraction driver
         com::proc::Comm dex_comm;
   };
}

#endif
