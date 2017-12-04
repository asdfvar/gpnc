// slave_comm.h

#ifndef SLAVE_COMM_H
#define SLAVE_COMM_H

class Slave_comm : public Communicator {

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
