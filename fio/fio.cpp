#include "fio.h"
#include "string_utils.h"

namespace fio {

   /*
   ** constructor name: Text_file
   */
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

   /*
   ** destructor name: Text_file
   */
   Text_file::~Text_file( void )
   {
      std::cout << "clearing text-file contents" << std::endl;
   }

   /*
   ** function name: print_all from Text_file
   */
   void Text_file::print_all( void )
   {
      std::cout << contents;
   }

   /*
   ** function name: get_int from Text_file
   */
   int Text_file::get_int( const std::string& str_val )
   {
      std::string::size_type loc = contents.find( str_val );
      std::string::size_type eq = 0;

      if (loc == std::string::npos)
      {
         std::cout << str_val << " parameter not found" << std::endl;
         return 0;
      } else {

         eq = contents.find( '=', loc );
         int ind;

         // advance past white space
         for( ind = eq + 1; contents[ind] == ' ' && ind < contents.length(); ind++) {}

         // build the number string
         std::string str_num = "";
         for( ; contents[ind] != ' ' && ind < contents.length(); ind++)
         {
            str_num += contents[ind];
         }

         // convert the number string to an integer
         return atoi( str_num.c_str() );
      }
      return 0;
   }
}  
