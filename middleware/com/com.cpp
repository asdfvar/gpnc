#include "com.h"
#include <iostream>

namespace com {

   Com::Com( int argc, char* argv[] )
   {
      MPI_Init( &argc, &argv );
      MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
      MPI_Comm_rank( MPI_COMM_WORLD, &myid );

      std::cout << "creating communication object " << myid
                << " out of " << numprocs << " processes."
                << std::endl;
   }

   Com::~Com( void )
   {
      MPI_Finalize();
      std::cout << "deleting process " << myid << " communication object" << std::endl;
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

   int create_tsk( pthread_t* thread, void* (*start_routine)(void*), void* arg)
   {
      const pthread_attr_t* attr = NULL;

      return pthread_create(
            thread,
            attr,
            start_routine,
            arg);
   }

   int join_tsk( pthread_t thread)
   {
      void** value_ptr = NULL;
      return pthread_join( thread, value_ptr );
   }

   int tsk_barrier_wait( pthread_barrier_t* barrier)
   {
      return pthread_barrier_wait( barrier );
   }
}
