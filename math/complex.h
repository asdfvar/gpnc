#ifndef complex_H
#define complex_H

#include <iostream>
#include <typeinfo>

template <class dataType>
class complex
{
   public:
      complex (void)
      {
         if (typeid (dataType) == typeid (float)) {
            real = 0.0f;
            imag = 0.0f;
         }
         else if (typeid (dataType) == typeid (double)) {
            real = 0.0;
            imag = 0.0;
         }
         else if (typeid (dataType) == typeid (int)) {
            real = 0;
            imag = 0;
         }
      }

      complex (dataType Real, dataType Imag) {
         real = Real;
         imag = Imag;
      }

      void operator= (const complex& c)
      {
         real = c.real;
         imag = c.imag;
      }

      complex operator+ (const complex& c)
      {
         complex result;
         result.real = real + c.real;
         result.imag = imag + c.imag;
         return result;
      }

      complex operator- (const complex& c)
      {
         complex result;
         result.real = real - c.real;
         result.imag = imag - c.imag;
         return result;
      }

      complex operator* (const complex& c)
      {
         complex result;
         result.real = real * c.real - imag * c.imag;
         result.imag = real * c.imag + imag * c.real;
         return result;
      }

      complex operator/ (const complex& c)
      {
         complex result;
         result.real     = real * c.real + imag * c.imag;
         result.imag     = imag * c.real - real * c.imag;
         dataType denom  = c.real * c.real + c.imag * c.imag;
         result.real    /= denom;
         result.imag    /= denom;
         return result;
      }

      complex operator+ (const dataType& number)
      {
         complex result;
         result.real = real + number;
         result.imag = imag;
         return result;
      }

      complex operator- (const dataType& number)
      {
         complex result;
         result.real = real - number;
         result.imag = imag;
         return result;
      }

      complex operator* (const dataType& number)
      {
         complex result;
         result.real = real * number;
         result.imag = imag * number;
         return result;
      }

      complex operator/ (const dataType& number)
      {
         complex result;
         result.real = real / number;
         result.imag = imag / number;
         return result;
      }

      complex operator+= (const complex& c)
      {
         *this = *this + c;
      }

      complex operator-= (const complex& c)
      {
         *this = *this - c;
      }

      complex operator*= (const complex& c)
      {
         *this = *this * c;
      }

      complex operator/= (const complex& c)
      {
         *this = *this / c;
      }

      complex operator+= (const dataType& c)
      {
         *this = *this + c;
      }

      complex operator-= (const dataType& c)
      {
         *this = *this - c;
      }

      complex operator*= (const dataType& c)
      {
         *this = *this * c;
      }

      complex operator/= (const dataType& c)
      {
         *this = *this / c;
      }

      friend std::ostream& operator<< (std::ostream& output, const complex& number)
      {
         output << number.real << " + " << number.imag << "i";
      }

   private:

      dataType real;
      dataType imag;

};

#endif
