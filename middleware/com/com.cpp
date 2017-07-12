#include "com.h"
#include <iostream>

fck::fck( int n ) {}

void dope( int k )
{
   int t = k * 7;
}

namespace com {

   Com::Com( void ) {}
   Com::Com( int n ) {}

#if 1
   Com::Com( int argc, char* argv[] )
   {
      MPI_Init( &argc, &argv );
      MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
      MPI_Comm_rank( MPI_COMM_WORLD, &myid );
std::cout << "myid = " << myid << std::endl;
std::cout << "numprocs = " << numprocs << std::endl;
   }
#endif

   Com::~Com( void )
   {
      MPI_Finalize();
      std::cout << "deleting communication object" << std::endl;
   }

   void Com::write( const std::string& file,
                    float*             data )
   {
   }

   int Com::send( int*         buf,
                  int          count,
                  int          dest_id,
                  MPI_Request* request )
   {
      // perform a non-blocking send
      return MPI_Isend ( buf, count, MPI_INT, dest_id, MPI_ANY_TAG, MPI_COMM_WORLD, request );
   }

   int Com::send( float*       buf,
                  int          count,
                  int          dest_id,
                  MPI_Request* request )
   {
      // perform a non-blocking send
      return MPI_Isend ( buf, count, MPI_FLOAT, dest_id, 1, MPI_COMM_WORLD, request );
   }

   int Com::send( double*      buf,
                  int          count,
                  int          dest_id,
                  MPI_Request* request )
   {
      // perform a non-blocking send
      return MPI_Isend ( buf, count, MPI_DOUBLE, dest_id, MPI_ANY_TAG, MPI_COMM_WORLD, request );
   }

   int Com::recv( int*         buf,
                  int          count,
                  int          src_id,
                  MPI_Request* request )
   {
      // perform a non-blocking receive
      return MPI_Irecv ( buf, count, MPI_INT, src_id, MPI_ANY_TAG, MPI_COMM_WORLD, request );
   }

   int Com::recv( float*       buf,
                  int          count,
                  int          src_id,
                  MPI_Request* request )
   {
      // perform a non-blocking receive
      return MPI_Irecv ( buf, count, MPI_FLOAT, src_id, 1, MPI_COMM_WORLD, request );
   }

   int Com::recv( double*      buf,
                  int          count,
                  int          src_id,
                  MPI_Request* request )
   {
      // perform a non-blocking receive
      return MPI_Irecv ( buf, count, MPI_DOUBLE, src_id, MPI_ANY_TAG, MPI_COMM_WORLD, request );
   }

   int Com::wait( MPI_Request* request )
   {
      MPI_Status status;
      return MPI_Wait ( request, &status );
   }
}
