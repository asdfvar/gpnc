#ifndef FIO_H
#define FIO_H

#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <string>

namespace fio {

   class Parameter {

      public:
         Parameter ( std::string in_file );
        ~Parameter ( void );

         int    get_int ( const std::string& );
         double get_real( const std::string& );

         void   print_all( void );

      private:
         std::string contents;
   };

}

#endif
