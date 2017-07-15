#include "memory.h"
#include <iostream>

namespace mem {
/*
** constructor name: Memory
*/
Memory::Memory( size_t num_words )
{
   size_words = num_words;
   buffer     = new float[num_words];
   ptr        = buffer;
}

/*
** destructor name: ~Memory
*/
Memory::~Memory( void )
{
   // Null
}

/*
** function name: reserve from: Memory
*/
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

/*
** function name: finalize from: Memory
*/
void Memory::finalize( void )
{
   std::cout << "clearing memory buffer object at " << buffer << std::endl;
   delete[] buffer;
}

}
