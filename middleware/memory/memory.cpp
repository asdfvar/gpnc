#include "memory.h"
#include <iostream>

namespace mem {

   // Memory constructor
   Memory::Memory( void )
   {
      // NULL
   }

   // Memory constructor
   Memory::Memory( size_t num_words )
   {
      size_words = num_words;
      buffer     = new float[num_words];
      ptr        = buffer;
   }

   // Memory destructor
   Memory::~Memory( void )
   {
      // Null
   }

   // reserve function from Memory class
   float* Memory::reserve( size_t num_words )
   {
      float* r = ptr;

      ptr += num_words;

      if (ptr - buffer > size_words)
      {
         std::cout << "out of memory" << std::endl;
         return NULL;
      }

      return r;
   }

   // finalize function from Memory class
   void Memory::finalize( void )
   {
      std::cout << "clearing memory buffer object at " << buffer << std::endl;
      delete[] buffer;
   }

}
