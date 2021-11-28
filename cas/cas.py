#!/usr/bin/python3
import math

def isValue (term):
   return isinstance (term, int) or isinstance (term, float) or isinstance (term, complex)

class OperationBi:

   def __init__ (self, term1, term2, Type):
      self.term1 = term1
      self.term2 = term2
      self.Type  = Type

   def reduce (self):

      if isinstance (self.term1, type (self)):
         self.term1 = self.term1.reduce ()
      if isinstance (self.term2, type (self)):
         self.term2 = self.term2.reduce ()

      # If one of the terms is a known number, ensure that the first term is set to one of the
      # known numbers to reduce the set of possible permutations downstream
      if isValue (self.term2):
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

      return self

   def rationalize (self):

      if isinstance (self.term1, type (self)):
         self.term1 = self.term1.rationalize ()
      if isinstance (self.term2, type (self)):
         self.term2 = self.term2.rationalize ()

      if isinstance (self.term1, float):
         term = self.term1
         num = math.floor (term)
         den = 1
         while num != term:
            term *= 10
            num = math.floor (term)
            den *= 10
         self.term1 = OperationBi (num, den, '/')

      if isinstance (self.term2, float):
         term = self.term2
         num = math.floor (term)
         den = 1
         while num != term:
            term *= 10
            num = math.floor (term)
            den *= 10
         self.term2 = OperationBi (num, den, '/')

   def __str__ (self):
      if self.Type == '+':
         return "(" + str (self.term1) + " + " + str (self.term2) + ")"
      if self.Type == '*':
         return "(" + str (self.term1) + " * " + str (self.term2) + ")"
      if self.Type == '/':
         return "(" + str (self.term1) + " / " + str (self.term2) + ")"
      if self.Type == '^':
         return "(" + str (self.term1) + " ^ " + str (self.term2) + ")"
      return None

if __name__ == "__main__":
   op = OperationBi (7, 5, '+')
   op = OperationBi (op, 100, '+')
   op  = OperationBi (op, 'a', '+')
   op1 = OperationBi (4.3, 5, '*')
   op2 = OperationBi (op1, 5, '+')
   op  = OperationBi (op, op2, '+')

   print ("binary operand consists of " + str (op))
   result = op.reduce ()
   print ("Which evaluates to " + str (result))
   result.rationalize ()
   print ("Which rationalizes to " + str (result))
