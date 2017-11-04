// extract_data.h

#ifndef EXTRACT_DATA_H
#define EXTRACT_DATA_H

#include "com.h"
#include <string>
#include "proc_maps.h"

template<class Data_type> void extract_data(
      Data_type*      src,
      int             count,
      com::proc::Comm dex_comm )
{

   Meta meta_data;

   meta_data.test      = 314159;
   meta_data.type_size = sizeof(Data_type);
   meta_data.count     = count;

   // data extraction does not terminate the data extraction task
   meta_data.terminate = false;

   com::proc::Request request;

   // send meta data
   com::proc::Isend(
        &meta_data,   // buffer
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

#endif
