#ifndef COM_H
#define COM_H

#include <mpi.h>
#include <string>

namespace com {

   typedef struct {
      int connections[10];
      int dims[10];
      int n_connections;
   } Topology;

   class Com {

    public:

      Com( size_t vol_size );
     ~Com( void );

      void write( const std::string& file,
                  float*             data,
                  Topology*          topology );

    private:

      int*    volume;
      size_t  vol_size;
      string  file_list[1024];

      MPI_Request request[1024];

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
