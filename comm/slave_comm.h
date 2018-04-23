// slave_comm.h

#ifndef SLAVE_COMM_H
#define SLAVE_COMM_H

namespace slave {

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

         // communication handle for local slave
         com::proc::Comm my_comm;

         // communication handle to the data extraction driver
         com::proc::Comm dex_comm;
   };
}

#endif
