#include "com.h"
#include "io.h"

namespace io
{

   Write::Write ( int argc, char** argv, int group_number )
   {
      int rank;
      int numprocs;

      // initiate MPI communication
      com::proc::start( argc, argv, &numprocs, &rank );

      // build intra-communicator for local sub-group
      com::proc::split( group_number, rank, &my_group_handle );

      // initiate number of group handles to zero
      num_group_handles = 0;
   }

   // create new communication handle to specified group
   void Write::create (
         int target_group,
         int tag )
   {
      com::proc::intercomm_create (
            my_group_handle,
            target_group,
            tag,
            &group_handles[num_group_handles++] );
   }

   Write::~Write ( void )
   {
      // free communicators
      while (num_group_handles > 0)
         com::proc::free( &group_handles[--num_group_handles] );

      // finalize MPI
      com::proc::finalize();
   }
}
