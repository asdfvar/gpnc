#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

std::string trim ( std::string );

std::string parse_string (
                  std::string input_string,
                  std::string delimiter,
                  unsigned int index );

std::string char_to_string( char* src,
                            int   length );

#endif
