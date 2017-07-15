#include <iostream>
#include "fio.h"
#include <string>

int main()
{
   fio::Text_file test_file( "file.txt" );
   std::cout << "parameter is " << test_file.get_int( "parameter" ) << std::endl;
   std::cout << "pi is " << test_file.get_real( "pi" ) << std::endl;

   return 0;
}
