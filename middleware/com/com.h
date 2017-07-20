#ifndef COM_H
#define COM_H

#include <mpi.h>
#include <string>
#include <iostream>
#include <pthread.h>

namespace com {

   // MPI interface
   class Com {

    public:

      Com( int argc, char* argv[] );
     ~Com( void );

      void write( const std::string& file,
                  float*             data );

      int send( int*         buf,
                int          count, 
                int          dest_id,
                int          tag,
                MPI_Request* request );

      int send( float*       buf,
                int          count, 
                int          dest_id,
                int          tag,
                MPI_Request* request );

      int send( double*      buf,
                int          count, 
                int          dest_id,
                int          tag,
                MPI_Request* request );

      int recv( int*         buf,
                int          count,
                int          dest_id,
                int          tag,
                MPI_Request* request );

      int recv( float*       buf,
                int          count,
                int          dest_id,
                int          tag,
                MPI_Request* request );

      int recv( double*      buf,
                int          count,
                int          dest_id,
                int          tag,
                MPI_Request* request );

      int wait( MPI_Request* request );

    private:

      int myid;
      int numprocs;

   };

   typedef MPI_Request proc_request;

   // POSIX threading interface
   int create_tsk( pthread_t* thread,
                   void*      (*start_routine)(void*),
                   void*      arg);

   int join_tsk  ( pthread_t thread );

   int tsk_barrier_init( pthread_barrier_t*,
                         unsigned int count );

   int tsk_barrier_wait( pthread_barrier_t* barrier );

   int tsk_barrier_destroy( pthread_barrier_t* barrier);

   typedef pthread_barrier_t tsk_barrier;
   typedef pthread_t         tsk_handler;
}

#endif
