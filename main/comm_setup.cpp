#include "comm_setup.h"
#include "com.h"
#include "proc_maps.h"

Comm_setup::Comm_setup( int argc, char* argv[] )
{

   // initiate process
   com::proc::init( argc, argv );

   // get number of processors
   com::proc::size( com::proc::Comm_world, &numprocs );

   // get global rank
   com::proc::rank( com::proc::Comm_world, &global_rank );

   // split comm world
   com::proc::split( MASTER_GROUP, global_rank, &my_comm );

   // get local rank
   com::proc::rank( my_comm, &local_rank );

   // inter-communicator to data extraction
   com::proc::intercomm_create( my_comm, DATA_EXTRACTION_GROUP, DATA_EXT, &dex_comm );

}

com::proc::Comm Comm_setup::get_dex_comm( void )
{
   return dex_comm;
}

void Comm_setup::finalize( void )
{
   // free data extraction communication handle
   com::proc::free( &dex_comm );

   // finalize processor communication
   com::proc::finalize();
}

Comm_setup::~Comm_setup( void )
{
   // null
}
