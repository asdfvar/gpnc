#ifndef IO_H
#define IO_H

#include "com.h"

#define MAX_DIM           10
#define MAX_GROUP_HANDLES 10

namespace io
{

   typedef struct {
      unsigned int size;
      unsigned int dim[MAX_DIM];
   } META_DATA;

   class Write {

      public:
         Write( int rank, int group_number );
         ~Write( void );

         void create( int target_group, int tag );

      private:
         // intra-communicator of local sub-group
         com::proc::Comm my_group_handle;
         com::proc::Comm group_handles[MAX_GROUP_HANDLES];
         int             num_group_handles;
   };
}

#endif
