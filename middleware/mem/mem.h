#ifndef MEMORY_H
#define MEMORY_H

#include <cstdlib>
#include <string>

namespace mem {

   class Memory {
      public:
         Memory( void );
         Memory( size_t, const std::string& name );
         ~Memory( void );

         float* reserve( size_t );
         void   finalize( void );

      private:
         size_t size_words;
         float* buffer;
         float* ptr;
         char   name[64];
   };

}

#endif
