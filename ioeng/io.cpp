#include "com.h"
#include "io.h"
#include "groups.h"

namespace io
{

   Write::Write ( int rank, int group_number, com::proc::Comm myComm, int tag )
   {

      // initiate number of group handles to zero
      num_group_handles = 0;

      // initiate number of file names
      num_file_names = 0;

      // build intra-communicator for local sub-group
//      com::proc::split( group_number, rank, &my_group_handle );

#if 0
      com::proc::intercomm_create(
            myComm,
            group_number,
            tag,
            &group_handles[num_group_handles++]);
#endif
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
#if 0
      // free communicators
      while (num_group_handles > 0)
         com::proc::free( &group_handles[--num_group_handles] );
#endif
   }
}
