#ifndef COM_H
#define COM_H

#include <mpi.h>

namespace com {

   class Com {

    private:

      int* volume[1024];

      int send( int* buf,
                int  count, 
                int  dest_id );

      int send( float* buf,
                int    count, 
                int    dest_id );

      int send( double* buf,
                int     count, 
                int     dest_id );

      int recv( int* buf,
                int  count,
                int  dest_id );

      int recv( float* buf,
                int    count,
                int    dest_id );

      int recv( double* buf,
                int     count,
                int     dest_id );

      int wait( int* buf,
                int  count,
                int  src_id,
                MPI_Status* status );
   };
}

#endif
