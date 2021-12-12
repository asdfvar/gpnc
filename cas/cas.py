#!/usr/bin/python3
import math

def isValue (term):
   return isinstance (term, int) or isinstance (term, float) or isinstance (term, complex)

class OperationBi:

   def __init__ (self, term1, term2, Type):
      self.term1 = term1
      self.term2 = term2
      self.Type  = Type

   def expand (self):

      if isinstance (self.term1, type (self)):
         self.term1 = self.term1.expand ()
      if isinstance (self.term2, type (self)):
         self.term2 = self.term2.expand ()

      # If one of the terms is a known number, ensure that the first term is set to one of the
      # known numbers to reduce the set of possible permutations downstream
      if isValue (self.term2) and (self.Type == '+' or self.Type == '*'):
         self.term1, self.term2 = self.term2, self.term1

      # If both of the terms are variable terms, arrange them in alphabetical order
      if self.Type == '+' or self.Type == '*':
         if not isValue (self.term1) and not isValue (self.term2):
            if not isinstance (self.term1, type (self)) and \
               not isinstance (self.term2, type (self)):
               if self.term1 > self.term2:
                  self.term1, self.term2 = self.term2, self.term1

      # Apply the operation directly if the two terms are known values
      if isValue (self.term1) and isValue (self.term2):
         if self.Type == '+':
            return self.term1 + self.term2
         elif self.Type == '*':
            return self.term1 * self.term2
         elif self.Type == '/':
            return self.term1 / self.term2
         elif self.Type == '^':
            return self.term1 ** self.term2
         else:
            print ("Unrecognized type " + str (self.Type))
            return None

      # Handle a*x + b*x -> (a + b)*x for known numbers 'a' and 'b', and unknown 'x'
      if isinstance (self.term1, type (self)) and isinstance (self.term2, type (self)):
         if isValue (self.term1.term1) and isValue (self.term2.term1) and \
            self.term1.term2 == self.term2.term2:
            if self.Type == '+' and self.term1.type == '*' and self.term2.type == '*':
               return OperationBi (self.term1.term1 + self.term2.term1, self.term1.term2, '*')

      # Handle a + (b + x) -> (a + b) + x for known numbers 'a' and 'b', and unknown 'x'
      if isValue (self.term1) and isinstance (self.term2, type (self)):
         if isValue (self.term2.term1) and isinstance (self.term2.term2, type (str ())) and \
             self.term2.Type == '+':
            return OperationBi (self.term1 + self.term2.term1, self.term2.term2, '+')

      # Handle (x + y)^n -> (x + y) * (x + y)^(n-1) for integer 'n'
      if self.Type == '^':
         if isinstance (self.term1, type (self)):
            if isinstance (self.term2, int) and self.term2 > 1 and self.term1.Type == '+':
               # term1 = (x + y)
               # term2 = n
               term2 = OperationBi (self.term1, self.term2 - 1, '^')
               term2 = term2.expand ()
               term  = OperationBi (self.term1, term2, '*')
               term  = term.expand ()
               return term
            elif self.term2 == 1:
               return self.term1

      # Handle (a + b)*(x + y) -> a*x + a*y + b*x + b*y
      if self.Type == '*':
         if isinstance (self.term1, type (self)) and isinstance (self.term2, type (self)):
            if self.term1.Type == '+' and self.term2.Type == '+':
               # (a*x) + (a*y) + (b*x) + (b*y) correspond to parts
               #  (1),    (2),    (3), and (4) respectively
               part1 = OperationBi (self.term1.term1, self.term2.term1, '*')
               part1 = part1.expand ()
               part2 = OperationBi (self.term1.term1, self.term2.term2, '*')
               part2 = part2.expand ()
               part3 = OperationBi (self.term1.term2, self.term2.term1, '*')
               part3 = part3.expand ()
               part4 = OperationBi (self.term1.term2, self.term2.term2, '*')
               part4 = part4.expand ()
               # ((a*x) + (a*y)) + ((b*x) + (b*y)) correspond to parts
               #       (1)      and      (2)       respectively
               part1 = OperationBi (part1, part2, '+')
               part1 = part1.expand ()
               part2 = OperationBi (part3, part4, '+')
               part2 = part2.expand ()
               term1 = part1
               term2 = part2
               term = OperationBi (term1, term2, '+')
               term = term.expand ()
               return term

      # Handle x + x -> 2*x
      if self.Type == '+':
         if self.term1 == self.term2:
            term = OperationBi (2, self.term1, '*')
            term = term.expand ()
            return term

      # Handle x*x -> x^2
      if self.Type == '*':
         if self.term1 == self.term2:
            term = OperationBi (self.term1, 2, '^')
            term = term.expand ()
            return term

      return self

   def rationalize (self):

      if isinstance (self.term1, type (self)):
         self.term1 = self.term1.rationalize ()
      if isinstance (self.term2, type (self)):
         self.term2 = self.term2.rationalize ()

      def prime_factors (number):
         factor = 2
         factors = list ()
         while factor**2 <= number:
            if number % factor:
               factor += 1
            else:
               number //= factor
               factors.append (factor)
         if number > 1:
            factors.append (number)
         return factors

      if isinstance (self.term1, float):
         term = self.term1
         num = math.floor (term)
         den = 1
         while num != term:
            term *= 10
            num = math.floor (term)
            den *= 10

         num_factors = prime_factors (num)
         den_factors = prime_factors (den)
         for factor in den_factors:
            if factor in num_factors:
               num //= factor
               den //= factor

         self.term1 = OperationBi (num, den, '/')

      if isinstance (self.term2, float):
         term = self.term2
         num = math.floor (term)
         den = 1
         while num != term:
            term *= 10
            num = math.floor (term)
            den *= 10

         num_factors = prime_factors (num)
         den_factors = prime_factors (den)
         for factor in den_factors:
            if factor in num_factors:
               num //= factor
               den //= factor

         self.term2 = OperationBi (num, den, '/')

   def __eq__ (self, other):
      if isValue (other):
         return False
      else:
         return self.term1 == other.term1 and self.term2 == other.term2

   def __str__ (self):
      if self.Type == '+':
         return "(" + str (self.term1) + " + " + str (self.term2) + ")"
      if self.Type == '*':
         return "(" + str (self.term1) + "*" + str (self.term2) + ")"
      if self.Type == '/':
         return "(" + str (self.term1) + "/" + str (self.term2) + ")"
      if self.Type == '^':
         return "(" + str (self.term1) + "^" + str (self.term2) + ")"
      return None

if __name__ == "__main__":
   op = OperationBi (OperationBi ('x', 'y', '+'), 2, '^')
   print (str (op) + " --> " + str (op.expand ()))

   op = OperationBi ('x', 'x', '+')
   print (str (op) + " --> " + str (op.expand ()))

   op = OperationBi ('y', 'x', '+')
   print (str (op) + " --> " + str (op.expand ()))

   op = OperationBi (OperationBi ('y', 'x', '*'), OperationBi ('x', 'y', '*'), '+')
   print (str (op) + " --> " + str (op.expand ()))
