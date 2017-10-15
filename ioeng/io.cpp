#include "com.h"
#include "io.h"
#include "groups.h"

namespace io
{

   Write::Write ( int rank, int group_number, int tag )
   {

      // initiate number of group handles to zero
      num_group_handles = 0;

      // build intra-communicator for local sub-group
//      com::proc::split( group_number, rank, &my_group_handle );

      com::proc::intercomm_create(
            my_group_handle,
            group_number,
            tag,
            &group_handles[num_group_handles++]);
   }

   // TODO: make this obsolete
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
