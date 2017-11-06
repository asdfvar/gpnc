// slave_comm.cpp

#include "comm_parent.h"
#include "slave.h"
#include "com.h"
#include "proc_maps.h"

Slave_comm::Slave_comm( int argc, char* argv[] ) : Comm_parent( argc, argv )
{
   // split comm world to generate local communication handle
   com::proc::split( SLAVE_GROUP, global_rank, &my_comm );

   // get local rank
   com::proc::rank( my_comm, &local_rank );

   // inter-communicator to data extraction
   com::proc::intercomm_create( my_comm, DATA_EXTRACTION_GROUP, SLAVE_DATA_EXT, &dex_comm );
}

com::proc::Comm Slave_comm::get_dex_comm( void )
{
   return dex_comm;
}

void Slave_comm::finalize( void )
{
   // free data extraction communication handle
   com::proc::free( &dex_comm );

   // finalize process communication
   Comm_parent::finalize( "slave" );
}

Slave_comm::~Slave_comm( void )
{
   // null
}
