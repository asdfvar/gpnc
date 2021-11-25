#!/usr/bin/python3
import copy

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
         if isinstance (self.term, int) or isinstance (self.term, float) or isinstance (self.term, complex):
            ret = Variable (other.term + self.term)
         elif self.term == other.term and self.exp == other.exp:
            ret = Variable (self.term, self.coef + other.coef)
      else:
         ret = Addition (self, other)
      return ret

   def __mul__ (self, other):
      ret = None
      if isinstance (self.term, type (other.term)):
         if isinstance (self.term, int) or isinstance (self.term, float) or isinstance (self.term, complex):
            ret = Variable (other.term * self.term)
         elif self.term == other.term and self.exp == other.exp:
            ret = Variable (self.term, self.coef * other.coef)
      return ret

   def is_addative_id (self):
      if self.term == 0: return True
      return False

   def is_multiplicative_id (self):
      if self.term == 1: return True
      return False

   def __str__ (self):
      components = ""
      if self.coef != 1:
         components += str (self.coef) + "*"
      components += str (self.term)
      if self.exp != 1:
         components += "^" + str (self.exp)
      return components

class Addition:

   def __init__ (self, term1, term2):
      self.terms = list()
      if isinstance (term1, Addition):
         self.terms += term1.terms
      else:
         self.terms.append (term1)
      if isinstance (term2, Addition):
         self.terms += term2.terms
      else:
         self.terms.append (term2)

   def evaluate (self):
      inda = 0
      while inda < len (self.terms):
         indb = inda + 1
         if isinstance (self.terms[inda], Addition):
            self.terms[inda].evaluate ()
         elif isinstance (self.terms[inda], Multiplication):
            self.terms[inda].evaluate ()
         else:
            while indb < len (self.terms):
               while isinstance (self.terms[inda], Variable) and \
                     isinstance (self.terms[indb], Variable) and \
                     type (self.terms[inda].term) == type (self.terms[indb].term):
                  self.terms[inda] += self.terms[indb]
                  self.terms.pop (indb)
                  if indb >= len (self.terms): break
               indb += 1
         inda += 1

   def __str__ (self):
      components = None
      for term in self.terms:
         if components is None:
            components = str (term)
         else:
            components += " + " + str (term)

      if components is None:
         return ""
      else:
         return components

class Multiplication:

   def __init__ (self, term1, term2):
      self.terms = list()
      if isinstance (term1, Multiplication):
         self.terms += term1.terms
      else:
         self.terms.append (term1)
      if isinstance (term2, Multiplication):
         self.terms += term2.terms
      else:
         self.terms.append (term2)

   def evaluate (self):
      inda = 0
      while inda < len (self.terms):
         indb = inda + 1
         if isinstance (self.terms[inda], Addition):
            self.terms[inda].evaluate ()
         elif isinstance (self.terms[inda], Multiplication):
            self.terms[inda].evaluate ()
         else:
            while indb < len (self.terms):
               while isinstance (self.terms[inda], Variable) and \
                     isinstance (self.terms[indb], Variable) and \
                     type (self.terms[inda].term) == type (self.terms[indb].term):
                  self.terms[inda] *= self.terms[indb]
                  self.terms.pop (indb)
                  if indb >= len (self.terms): break
               indb += 1
         inda += 1

   def __str__ (self):
      components = None
      for term in self.terms:
         if components is None:
            components = str (term)
         else:
            components += " * " + str (term)

      if components is None:
         return ""
      else:
         return components

if __name__ == "__main__":
   op1 = Addition (Variable (7), Variable (5))
   op2 = Addition (Variable (11), Variable (100))
   op  = Addition (op1, op2)
   op  = Addition (op, Variable ('a'))
   op  = Addition (op, Variable (7))
   op  = Addition (op, Multiplication (Variable (4), Variable (5)))

   print ("binary operand consists of " + str (op))
   op.evaluate ()
   print ("Which evaluates to " + str (op))
