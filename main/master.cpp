#include "com.h"
#include "fio.h"
#include "parameters.h"
#include "memory.h"
#include <iostream>

int main( int argc, char* argv[] )
{

   com::Com comObj( argc, argv );

   fio::Text_file parameters( "../parameters/parameters.txt" );

   std::cout << std::endl;
   std::cout << "reading in parameter file contents:" << std::endl;
   parameters.print_all();
   std::cout << std::endl;

   Inp_params inp_parameters;

   inp_parameters.par_int    = parameters.get_int( "parameter_int" );
   inp_parameters.par_float  = parameters.get_real( "parameter_float" );
   inp_parameters.par_double = parameters.get_real( "parameter_double" );
   size_t mem_size           = parameters.get_int( "memory_size_master" );

   mem::Memory workspace( mem_size );

   float buf[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
   com::com_request request;

   std::cout << "about to send" << std::endl;

   comObj.send( (float*)buf, 4, 2, &request );

   std::cout << "sending" << std::endl;

   comObj.wait( &request );

   std::cout << "sent" << std::endl;

   // finalize
   workspace.finalize();

   return 0;
}
