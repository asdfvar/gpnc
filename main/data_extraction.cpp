// data_extraction.cpp

#include "com.h"
#include "data_extraction.h"
#include "proc_maps.h"

Data_extraction::Data_extraction (
      int rank,
      int group_number,
      com::proc::Comm myComm,
      int tag )
{

   // initiate number of group handles to zero
   num_group_handles = 0;

   // initiate number of file names
   num_file_names = 0;

   // build intra-communicator for local sub-group
   //      com::proc::split( group_number, rank, &my_group_handle );

}

Data_extraction::~Data_extraction( void )
{
   // null
}
