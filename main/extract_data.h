// extract_data.h

#ifndef EXTRACT_DATA_H
#define EXTRACT_DATA_H

#include "com.h"
#include <string>
#include "proc_maps.h"

namespace master {
   template<class Data_type> void extract_data(
         Data_type*            src,
         const std::string&    filename,
         int                   count,
         com::proc::Comm       dex_comm )
   {

      Meta meta_data;

      meta_data.type_size = sizeof(Data_type);
      meta_data.count     = count;

      // data extraction does not terminate the data extraction task
      meta_data.terminate = false;

      com::proc::Request request;

      // send meta data
      com::proc::Isend(
            &meta_data,  // buffer
            1,           // count
            0,           // destination id
            MASTER_META, // tag
            dex_comm,    // comm handle
            &request );

      // wait for meta data to be sent
      com::proc::wait( &request );

      // send source data
      com::proc::Isend(
            src,         // buffer
            count,       // count
            0,           // destination id
            MASTER_DATA, // tag
            dex_comm,    // comm handle
            &request );

      // wait for data to be sent
      com::proc::wait( &request );

   }

   void finalize_extraction( com::proc::Comm dex_comm );
}

namespace slave {
   template<class Data_type> void extract_data(
         Data_type*            src,
         const std::string&    filename,
         int                   count,
         int                   proc_id,
         int                   task_id,
         com::proc::Comm       dex_comm )
   {

      Meta meta_data;

      meta_data.type_size = sizeof(Data_type);
      meta_data.count     = count;

      // data extraction does not terminate the data extraction task
      meta_data.terminate = false;

      com::proc::Request request;

      // send meta data
      com::proc::Isend(
            &meta_data,  // buffer
            1,           // count
            0,           // destination id
            SLAVE_META + 1000 + task_id,  // tag
//            dex_comm,    // comm handle
            com::proc::Comm_world,
            &request );

      // wait for meta data to be sent
      com::proc::wait( &request );

#if 1
      // send source data
      com::proc::Isend(
            src,         // buffer
            count,       // count
            0,           // destination id
            SLAVE_DATA + 2000 + task_id,  // tag
            dex_comm,    // comm handle
            &request );

      // wait for data to be sent
      com::proc::wait( &request );
#endif

   }

   void finalize_extraction( com::proc::Comm dex_comm, int task_id );
}


#endif
