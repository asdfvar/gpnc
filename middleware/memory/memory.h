#ifndef MEMORY_H
#define MEMORY_H

#include <cstdlib>

namespace mem {

class Memory {
   public:
      Memory( size_t );
     ~Memory( void );

      float* reserve( size_t );
      void   finalize( void );

   private:
      size_t size_words;
      float* buffer;
      float* ptr;
};

}

#endif
