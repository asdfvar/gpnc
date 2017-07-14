#ifndef COM_H
#define COM_H

#include <mpi.h>
#include <string>
#include <iostream>

class fck {
   public:
   fck( int n );
};

void dope( int k );

namespace com {

   class Com {

    public:

      Com( int argc, char* argv[] );
     ~Com( void );

      void write( const std::string& file,
                  float*             data );

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

    private:

      int myid;
      int numprocs;

      std::string  file_list[1024];

      MPI_Request request[1024];

   };
}

#endif
