#ifndef FIO_H
#define FIO_H

#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <string>

namespace fio {

   class Text_file {

      public:
         Text_file ( std::string in_file );
        ~Text_file ( void );

         void print_all( void );

      private:
         std::string contents;
   };

}

#endif
