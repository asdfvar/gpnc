#ifndef IO_H
#define IO_H

#include "com.h"
#include <string>

#define MAX_DIM           10
#define MAX_GROUP_HANDLES 10
#define MAX_FILE_NAMES    100

namespace io
{

   typedef struct {
      unsigned int size;
      unsigned int dim[MAX_DIM];
      std::string  file_name;
   } META_DATA;

   class Write {

      public:
         Write( int rank, int group_number, com::proc::Comm myComm, int tag );
         ~Write( void );

         void create( int target_group, int tag );

      private:
         // intra-communicator of local sub-group
         com::proc::Comm my_group_handle;
         int             num_group_handles;
         com::proc::Comm group_handles[MAX_GROUP_HANDLES];
         int             num_file_names;
         std::string     all_file_names[MAX_FILE_NAMES];

   };
}

#endif
