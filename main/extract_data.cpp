// extract_data.cpp

#include "extract_data.h"
#include <string>
#include "com.h"

template<class Type>
void extract_data(
      const std::string& filename,
      Type* src,
      int   dim )
{

   // send meta data
#if 0
   com::proc::Isend(
         filename,
         1,
         0,
         1,
#endif

   // send source data
}
