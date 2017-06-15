#include "com.h"
#include <mpi.h>

namespace com {

   int send( int* buf,
             int  count,
             int  dest_id )
   {
      // perform a non-blocking send
      return MPI_Isend ( buf, count, MPI_INT, dest_id, MPI_TAG_ANY, MPI_COMM_WORLD );
   }

   int send( float* buf,
             int    count,
             int    dest_id )
   {
      // perform a non-blocking send
      return MPI_Isend ( buf, count, MPI_FLOAT, dest_id, MPI_TAG_ANY, MPI_COMM_WORLD );
   }

   int send( double* buf,
             int     count,
             int     dest_id )
   {
      // perform a non-blocking send
      return MPI_Isend ( buf, count, MPI_DOUBLE, dest_id, MPI_TAG_ANY, MPI_COMM_WORLD );
   }

   int recv( int* buf,
             int  count,
             int  src_id )
   {
      MPI_Status status;

      // perform a non-blocking receive
      return MPI_Irecv ( buf, count, MPI_INT, src_id, MPI_TAG_ANY, &status );
   }

   int recv( float* buf,
             int    count,
             int    src_id )
   {
      MPI_Status status;

      // perform a non-blocking receive
      return MPI_Irecv ( buf, count, MPI_FLOAT, src_id, MPI_TAG_ANY, &status );
   }

   int recv( double*     buf,
             int         count,
             int         src_id,
             MPI_Status* status )
   {
      MPI_Status status;

      // perform a non-blocking receive
      return MPI_Irecv ( buf, count, MPI_DOUBLE, src_id, MPI_TAG_ANY, status );
   }

}
