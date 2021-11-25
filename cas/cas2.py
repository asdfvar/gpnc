#!/usr/bin/python3

class Variable:
   def __init__ (self, term, coef = 1, exp = 1):
      self.term = term
      self.coef = coef
      self.exp  = exp

   def __str__ (self):
      return str (self.term)

   def __add__ (self, other):
      ret = None
      if isinstance (self.term, type (other.term)):
         if isinstance (self.term, int) or \
            isinstance (self.term, float) or \
            isinstance (self.term, complex):
            ret = Variable (other.term + self.term)
         elif self.term == other.term and self.exp == other.exp:
            ret = Variable (self.term, self.coef + other.coef)
      else:
         ret = OperationBi (self, other, '+')
      return ret

   def __mul__ (self, other):
      ret = None
      if isinstance (self.term, type (other.term)):
         if isinstance (self.term, int) or \
            isinstance (self.term, float) or \
            isinstance (self.term, complex):
            ret = Variable (other.term * self.term)
         elif self.term == other.term:
            ret = Variable (self.term, self.coef * other.coef, self.exp + other.exp)
      else:
         ret = OperationBi (self, other, '*')
      return ret

   def __str__ (self):
      components = ""
      if self.coef != 1:
         components += str (self.coef) + "*"
      components += str (self.term)
      if self.exp != 1:
         components += "^" + str (self.exp)
      return components

class OperationBi:

   def __init__ (self, term1, term2, Type):
      self.term1 = term1
      self.term2 = term2
      self.Type  = Type

   def evaluate (self):

      if isinstance (self.term1, OperationBi):
         self.term1 = self.term1.evaluate ()
      if isinstance (self.term2, OperationBi):
         self.term2 = self.term2.evaluate ()

      if isinstance (self.term1, Variable) and isinstance (self.term2, Variable):
         if self.Type == '+':
            return self.term1 + self.term2
         if self.Type == '*':
            return self.term1 * self.term2

      return self

   def __str__ (self):
      if self.Type == '+':
         return "(" + str (self.term1) + " + " + str (self.term2) + ")"
      if self.Type == '*':
         return "(" + str (self.term1) + " * " + str (self.term2) + ")"
      return None

if __name__ == "__main__":
   op = OperationBi (Variable (7), Variable (5), '+')
   op = OperationBi (op, Variable (100), '+')
   op  = OperationBi (op, Variable ('a'), '+')
   op  = OperationBi (op, OperationBi (Variable (4), Variable (5), '*'), '+')
   op  = OperationBi (op, Variable (7), '+')

   print ("binary operand consists of " + str (op))
   result = op.evaluate ()
   print ("Which evaluates to " + str (result))