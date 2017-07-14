#include "com.h"
#include "fio.h"
#include "parameters.h"
#include <iostream>

int main( int argc, char* argv[] )
{

   com::Com comObj( argc, argv );

   fio::Text_file parameters( "../parameters/parameters.txt" );

   Inp_params inp_parameters;

   inp_parameters.par_int    = parameters.get_int( "parameter_int" );
   inp_parameters.par_float  = parameters.get_real( "parameter_float" );
   inp_parameters.par_double = parameters.get_real( "parameter_double" );

   std::cout << "parameters = " << inp_parameters.par_int   << ", "
                                << inp_parameters.par_float << ", "
                                << inp_parameters.par_double << std::endl;
   return 0;
}
