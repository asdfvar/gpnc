#ifndef COM_H
#define COM_H

#include <mpi.h>

namespace com {

   class Com {

    private:

      int* volume[1024];

      int send( int*         buf,
                int          count, 
                int          dest_id,
                MPI_Request* request );

      int send( float*       buf,
                int          count, 
                int          dest_id,
                MPI_Request* request );

      int send( double*      buf,
                int          count, 
                int          dest_id,
                MPI_Request* request );

      int recv( int*         buf,
                int          count,
                int          dest_id,
                MPI_Request* request );

      int recv( float*       buf,
                int          count,
                int          dest_id,
                MPI_Request* request );

      int recv( double*      buf,
                int          count,
                int          dest_id,
                MPI_Request* request );

      int wait( MPI_Request* request );
   };
}

#endif
