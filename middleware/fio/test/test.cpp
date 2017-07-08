#include <iostream>
#include "fio.h"
#include <string>

int main()
{
   std::cout << "test for fio" << std::endl;
   fio::Text_file test_file( "file.txt" );
   test_file.print_all();
   std::cout << "parameter is " << test_file.get_int( "parameter" ) << std::endl;

   return 0;
}
