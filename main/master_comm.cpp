// master_comm.cpp

#include "communicator.h"
#include "master.h"
#include "com.h"
#include "proc_maps.h"

Master_comm::Master_comm( int argc, char* argv[] ) : Communicator( argc, argv )
{
   // split comm world to generate local communication handle
   com::proc::split(
         MASTER_GROUP,
         global_rank,
         &my_comm );

   // get local rank
   com::proc::rank( my_comm, &local_rank );

   // inter-communicator to data extraction
   com::proc::intercomm_create(
         my_comm,
         DATA_EXTRACTION_GROUP,
         MASTER_DATA_EXT,
         &dex_comm );
}

com::proc::Comm Master_comm::get_dex_comm( void )
{
   return dex_comm;
}

void Master_comm::finalize( void )
{
   // free data extraction communication handle
   com::proc::free( &dex_comm );

   // finalize process communication
   Communicator::finalize( "master" );
}

Master_comm::~Master_comm( void )
{
   // null
}
