// dex_comm.cpp

#include "communicator.h"
#include "dex_comm.h"
#include "com.h"
#include "proc_maps.h"

namespace dex {

   Comm_setup::Comm_setup( int argc, char* argv[] ) : Communicator( argc, argv )
   {
      // split comm world to generate local communication handle
      com::proc::split(
            DATA_EXTRACTION_GROUP,
            global_rank,
            &my_comm );

      // get local rank
      com::proc::rank( my_comm, &local_rank );

      // inter-communicator to data extraction
      com::proc::intercomm_create(
            my_comm,
            MASTER_GROUP,
            MASTER_DATA_EXT,
            &dex_comm );
   }

   com::proc::Comm Comm_setup::get_dex_comm( void )
   {
      return dex_comm;
   }

   void Comm_setup::finalize( void )
   {
      // free data extraction communication handle
      com::proc::free( &dex_comm );

      // finalize process communication
      Communicator::finalize( "data extraction" );
   }

   Comm_setup::~Comm_setup( void )
   {
      // null
   }
}
