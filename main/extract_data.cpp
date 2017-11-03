// extract_data.cpp

#include "extract_data.h"
#include <string>
#include "com.h"
#include "proc_maps.h"

void extract_data( com::proc::Comm dex_comm )
{

   Meta meta_data;

   meta_data.test     = 314159;
   meta_data.finished = false;

   com::proc::Request request;

float data = 3.14159f;

std::cout << "ABOUT TO SEND DATA" << std::endl;
   // send meta data
   com::proc::Isend(
#if 0
        &meta_data,   // buf
#else
        &data,
#endif
         1,           // count
         0,           // dest_id
         MASTER_META, // tag
         dex_comm,    // comm handle
        &request );

   com::proc::wait( &request );
std::cout << "DATA SENT" << std::endl;

   // send source data
}
