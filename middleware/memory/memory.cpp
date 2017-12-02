#include "memory.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

namespace mem {

   // Memory constructor in mem namespace
   Memory::Memory( void )
   {
      // NULL
   }

   // Memory constructor in mem namespace
   Memory::Memory( size_t num_words, const std::string& input_name )
   {
      size_words  = num_words;
      buffer      = new float[num_words];
      ptr         = buffer;
      strncpy(name, input_name.c_str(), sizeof(name));
   }

   // reserve function from Memory class in mem namespace
   float* Memory::reserve( size_t num_words )
   {
      float* r = ptr;

      ptr += num_words;

      if (ptr - buffer > size_words)
      {
         std::cout << "out of memory for " << name << " process" << std::endl;
         return NULL;
      }

      return r;
   }

   // finalize function from Memory class in mem namespace
   void Memory::finalize( void )
   {
      std::cout << "clearing " << name << " memory buffer object" << std::endl;
      delete[] buffer;
   }

   // Memory destructor in mem namespace
   Memory::~Memory( void )
   {
      // Null
   }

}
