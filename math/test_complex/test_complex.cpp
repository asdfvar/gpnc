#include "complex.h"
#include <iostream>

int main ( void )
{

   complex<float> c(3.0f, 2.2f);

   c += complex<float> (7.0f, 3.0f);

   std::cout << c << std::endl;
   std::cout << c + 5.0f << std::endl;
   std::cout << c - 5.0f << std::endl;
   std::cout << c * 5.0f << std::endl;
   std::cout << c / 5.0f << std::endl;

   return 0;
}
