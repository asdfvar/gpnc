// extract_data.cpp

#include "extract_data.h"
#include <string>
#include "com.h"
#include "proc_maps.h"

#if 0
#if 1
template<class Data_type> void extract_data<Data_type>(
      Data_type*      src,
#else
typedef int Data_type;
void extract_data(
      int*      src,
#endif
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
#endif

void finalize_extraction( com::proc::Comm dex_comm )
{
   Meta meta_data;

   com::proc::Request request;

   // send a finalization notice to the master DEX task
   // to let it know it can finish
   meta_data.terminate = true;

   // send meta data
   com::proc::Isend(
        &meta_data,   // buf
         1,           // count
         0,           // dest_id
         MASTER_META, // tag
         dex_comm,    // comm handle
        &request );

   // wait for notice to be sent
   com::proc::wait( &request );
}
