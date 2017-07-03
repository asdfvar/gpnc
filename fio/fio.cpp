#include "fio.h"
#include "string_utils.h"

namespace fio {

   Text_file::Text_file( std::string in_file )
   {

      std::ifstream input;
      input.open( in_file.c_str(), std::ios::in );

      // get length of file
      input.seekg( 0, input.end );
      int length = input.tellg();
      input.seekg( 0, input.beg );

      // allocate memory
      char* buffer = new char [length];

      // read all the data
      input.read( buffer, length );
      contents = char_to_string( buffer, length );
      delete[] buffer;

      // close file
      input.close();

   }  

   Text_file::~Text_file( void )
   {
      std::cout << "clearing text-file contents" << std::endl;
   }

   void Text_file::print_all( void )
   {
      std::cout << contents;
   }

}  
