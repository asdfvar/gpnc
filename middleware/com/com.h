#ifndef COM_H
#define COM_H

#include <mpi.h>
#include <string>
#include <iostream>
#include <pthread.h>
#include <typeinfo>

namespace com {

   // MPI interface
   namespace proc {

      static void start (
            int   argc,
            char* argv[],
            int*  numprocs,
            int*  myid )
      {
         MPI_Init( &argc, &argv );
         MPI_Comm_size( MPI_COMM_WORLD, numprocs );
         MPI_Comm_rank( MPI_COMM_WORLD, myid );
      }

      static void init( int argc, char* argv[] )
      {
         MPI_Init( &argc, &argv );
      }

      static void size( MPI_Comm comm, int* numprocs )
      {
         MPI_Comm_size( comm, numprocs );
      }

      static void rank( MPI_Comm comm, int* rank )
      {
         MPI_Comm_rank( comm, rank );
      }

      static void finalize( void )
      {
         MPI_Finalize();
      }

      static void free ( MPI_Comm* com_handle )
      {
         MPI_Comm_free( com_handle );
      }

      static void split (
            int       color,
            int       rank,
            MPI_Comm* group_handle )
      {
         MPI_Comm_split( MPI_COMM_WORLD, color, rank, group_handle );
      }

      static void intercomm_create(
            MPI_Comm  local_comm,
            int       remote_leader,
            int       tag,
            MPI_Comm* new_intercomm )
      {
         MPI_Intercomm_create( local_comm, 0, MPI_COMM_WORLD, remote_leader, tag, new_intercomm );
      }

      template <class Type>
         static int Isend(
               Type*        buf,
               int          count, 
               int          dest_id,
               int          tag,
               MPI_Comm     comm,
               MPI_Request* request )
         {
            // perform a non-blocking send
            if( typeid(Type) == typeid(int) )
               return MPI_Isend ( buf, count, MPI_INT, dest_id, tag, comm, request );
            else if( typeid(Type) == typeid(float) )
               return MPI_Isend ( buf, count, MPI_FLOAT, dest_id, tag, comm, request );
            else if( typeid(Type) == typeid(double) )
               return MPI_Isend ( buf, count, MPI_DOUBLE, dest_id, tag, comm, request );
            else {
               return MPI_Isend ( (void*)buf, count * sizeof(Type),
                                  MPI_BYTE, dest_id, tag, comm, request );
            }
         }

      template <class Type>
         static int Irecv(
               Type*        buf,
               int          count,
               int          src_id,
               int          tag,
               MPI_Comm     comm,
               MPI_Request* request )
         {
            // perform a non-blocking receive
            if( typeid(Type) == typeid(int) )
               return MPI_Irecv ( buf, count, MPI_INT, src_id, tag, comm, request );
            else if( typeid(Type) == typeid(float) )
               return MPI_Irecv ( buf, count, MPI_FLOAT, src_id, tag, comm, request );
            else if( typeid(Type) == typeid(double) )
               return MPI_Irecv ( buf, count, MPI_DOUBLE, src_id, tag, comm, request );
            else {
               return MPI_Irecv ( (void*)buf, count * sizeof(Type),
                                  MPI_BYTE, src_id, tag, comm, request );
            }
         }

      static int wait( MPI_Request* request )
      {
         MPI_Status status;
         return MPI_Wait ( request, &status );
      }

      static int Barrier(
            MPI_Comm comm )
      {
         return MPI_Barrier( comm );
      }

      typedef MPI_Request         Request;
      typedef MPI_Comm            Comm;
      const   MPI_Comm Comm_world MPI_COMM_WORLD;
   }

   // POSIX threading interface
   namespace tsk {

      static int create(
            pthread_t* thread,
            void*      (*start_routine)(void*),
            void*      arg)
      {
         const pthread_attr_t* attr = NULL;

         return pthread_create(
               thread,
               attr,
               start_routine,
               arg);
      }

      static int join  ( pthread_t thread )
      {
         void** value_ptr = NULL;
         return pthread_join( thread, value_ptr );
      }

      static int barrier_init(
            pthread_barrier_t* barrier,
            unsigned int       count )
      {
         const pthread_barrierattr_t* barrier_attr = NULL;
         return pthread_barrier_init( barrier, barrier_attr, count );
      }

      static int barrier_wait( pthread_barrier_t* barrier )
      {
         return pthread_barrier_wait( barrier );
      }

      static int barrier_destroy( pthread_barrier_t* barrier)
      {
         return pthread_barrier_destroy( barrier );
      }

      typedef pthread_barrier_t barrier;
      typedef pthread_t         handler;
   }
}

#endif
