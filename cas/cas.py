#!/usr/bin/python3
import copy

class Variable:
   def __init__ (self, term, coef = 1, exp = 1):
      self.term = term
      self.coef = coef
      self.exp  = exp

   def __eq__ (self, other):
      return self.term == other.term and self.exp == other.exp

   def __str__ (self):
      return str (self.term)

   def __add__ (self, other):
      ret = None
      if isinstance (self.term, type (other.term)):
         if isinstance (self.term, int) or isinstance (self.term, float) or isinstance (self.term, complex):
            ret = Variable (other.term + self.term)
         elif self.term == other.term and self.exp == other.exp:
            ret = Variable (self.term, self.coef + other.coef)
      return ret

   def __mult__ (self, other):
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

      # Sum all the Numbered terms of the list of terms
      sum_term = Variable (0)
      terms = copy.copy (self.terms)
      while len (terms) > 0:
         term = terms.pop ()
         if isinstance (term, Variable):
            sum_term += term

      # Remove all existing Numbers from the list of terms
      ind = 0
      while ind < len (self.terms):
         if isinstance (self.terms[ind], Variable):
            self.terms.pop (ind)
            ind -= 1
         ind += 1

      # Append the final summed Variable to the list of terms
      if not sum_term.is_addative_id ():
         self.terms.append (Variable (sum_term))

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

      # Collect all Variable terms in the list of terms
      variable_terms = list ()
      for term in self.terms:
         if isinstance (term, Variable):
            variable_terms.append (term)

      # Remove all existing Variables from the list of terms
      ind = 0
      while ind < len (self.terms):
         if isinstance (self.terms[ind], Variable):
            self.terms.pop (ind)
            ind -= 1
         ind += 1

      # Coalesce like variable terms from the list of Variables

      # Multiply all the Numbered terms of the list of terms
      prod_term = Variable (1)
      terms = copy.copy (self.terms)
      while len (terms) > 0:
         term = terms.pop ()
         if isinstance (term, Variable):
            prod_term *= term

      # Remove all existing Numbers from the list of terms
      ind = 0
      while ind < len (self.terms):
         if isinstance (self.terms[ind], Variable):
            self.terms.pop (ind)
            ind -= 1
         ind += 1

      # Prepend the final product Variable to the list of terms
      if not prod_term.is_multiplicative_id ():
         self.terms.insert (0, Variable (prod_term))

   def __str__ (self):
      return str (self.terms[0]) + " * " + str (self.terms[1])

if __name__ == "__main__":
   seven = Variable (7)
   five  = Variable (5)
   summ = seven + five
   print ("sum of numbers " + str (seven) + " and " + str (five) + " is " + str (summ))

   op = Addition (seven, five)
   print ("binary operand consists of " + str (op))
   op.evaluate ()
   print ("Which evaluates to " + str (op))
