#ifndef COMPLEX_H
#define COMPLEX_H

template <class dataType>
class complex
{
   complex::complex (void);

   void complex::operator= (const complex& c)
   {
      this.real = c.real;
      this.imag = c.imag;
   }

   complex complex::operator+ (const complex& c)
   {
      complex result;
      result.real = this.real + c.real;
      result.imag = this.imag + c.imag;
      return result;
   }

   complex complex::operator- (const complex& c)
   {
      complex result;
      result.real = this.real - c.real;
      result.imag = this.imag - c.imag;
      return result;
   }

   complex complex::operator* (const complex& c)
   {
      complex result;
      result.real = this.real * c.real - this.imag * c.imag;
      result.imag = this.real * c.imag + this.imag * c.real;
      return result;
   }

   // TODO: division

   private:

      dataType real;
      dataType imag;

}

#endif
