#include "com.h"
#include <iostream>

namespace com {

   Com::Com( size_t vol_size_in )
   {
      vol_size = vol_size_in;
      volume   = new int[vol_size];
   }

   Com::~Com( void )
   {
      delete[] volume;
      std::cout << "deleting communication object" << std::endl;
   }

   void Com::write( const std::string& file,
                    float*             data,
                    Topology*          topology )
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
      return MPI_Isend ( buf, count, MPI_FLOAT, dest_id, MPI_ANY_TAG, MPI_COMM_WORLD, request );
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
      return MPI_Irecv ( buf, count, MPI_FLOAT, src_id, MPI_ANY_TAG, MPI_COMM_WORLD, request );
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
