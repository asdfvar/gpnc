// extract_data.cpp

#include "extract_data.h"
#include <string>
#include "com.h"
#include "proc_maps.h"

namespace master {
   void finalize_extraction( com::proc::Comm dex_comm )
   {
      Meta meta_data;

      com::proc::Request request;

      // send a finalization notice to the master DEX task
      // to let it know it can finish
      meta_data.terminate = true;

      // send meta data
      com::proc::Isend(
            &meta_data,  // buf
            1,           // count
            0,           // dest_id
            MASTER_META, // tag
            dex_comm,    // comm handle
            &request );

      // wait for notice to be sent
      com::proc::wait( &request );
   }
}

namespace slave {
   void finalize_extraction( com::proc::Comm dex_comm, int task_id )
   {
      Meta meta_data;

      com::proc::Request request;

      // send a finalization notice to the master DEX task
      // to let it know it can finish
      meta_data.terminate = true;

      int meta_tag = SLAVE_META + task_id;

      // send meta data
      com::proc::Isend(
            &meta_data,  // buf
            1,           // count
            0,           // dest_id
            meta_tag,    // tag
            dex_comm,    // comm handle
            &request );

      // wait for notice to be sent
      com::proc::wait( &request );
   }
}
