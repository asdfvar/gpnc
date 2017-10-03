#include "com.h"
#include "io.h"

namespace io
{

   Write::Write ( int rank, int group_number )
   {
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
   }
}
