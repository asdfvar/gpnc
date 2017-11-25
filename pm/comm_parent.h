// comm_parent.h

// abstract class to handle communication setup

#ifndef COMM_PARENT_H
#define COMM_PARENT_H

#include "com.h"
#include "iostream"
#include "string"

class Comm_parent {

   public:

      // constructor
      Comm_parent( int argc, char* argv[] )
      {
         // initiate process
         com::proc::init( argc, argv );

         // get number of processors
         com::proc::size( com::proc::Comm_world, &numprocs );

         // get global rank
         com::proc::rank( com::proc::Comm_world, &global_rank );
      }

      int get_local_rank( void )
      {
         return local_rank;
      }

      int get_global_rank( void )
      {
         return global_rank;
      }

      // finalize communication setup
      virtual void finalize( const std::string& proc_name )
      {
         std::cout << "finalizing process " + proc_name << std::endl;

         // finalize process communication
         com::proc::finalize();
      }

   protected:

      // number of global processes
      int numprocs;

      // global rank
      int global_rank;

      // local rank
      int local_rank;

};

#endif
