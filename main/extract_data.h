// extract_data.h

#ifndef EXTRACT_DATA_H
#define EXTRACT_DATA_H

#include <string>

template<class Type>
void extract_data(
      const std::string& filename,
      Type* src,
      int   dim );

#endif
