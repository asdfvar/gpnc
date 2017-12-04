// slave_comm.cpp

#include "communicator.h"
#include "slave.h"
#include "com.h"
#include "proc_maps.h"

Slave_comm::Slave_comm( int argc, char* argv[] ) : Communicator( argc, argv )
{
   // split comm world to generate local communication handle
   com::proc::split(
         SLAVE_GROUP,
         global_rank,
         &my_comm );

   // get local rank
   com::proc::rank( my_comm, &local_rank );
}

com::proc::Comm Slave_comm::get_dex_comm( void )
{
   return dex_comm;
}

void Slave_comm::finalize( void )
{
   // free data extraction communication handle
//   com::proc::free( &dex_comm );

   // finalize process communication
   Communicator::finalize( "slave" );
}

Slave_comm::~Slave_comm( void )
{
   // null
}
