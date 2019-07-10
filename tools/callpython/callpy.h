#ifndef CALLPY_H
#define CALLPY_H

#include <Python.h>
#include <string>
#include <vector>
#include <iostream>

class CallPy {

   public:

      // constructor name: CallPy
      CallPy (const std::string py_path,
               const std::string module_name,
               const std::string function_name,
               const int num_args)
      {

         // set the python path to the requested location
         setenv ("PYTHONPATH", py_path.c_str (), 1);

         // initialize the python interpreter
         if (Py_IsInitialized() == 0) Py_Initialize ();

         PyObject *pName = PyString_FromString (module_name.c_str ());
         pyModule         = PyImport_Import (pName);
         Py_DECREF (pName);

         if (pyModule != nullptr)
         {
            pyFunc = PyObject_GetAttrString (pyModule, function_name.c_str ());
         }

         // python tuple to be passed as the argument list to the specified python function
         pyArgs     = PyTuple_New (num_args);
         argument   = 0;
      }

      // destructor name: CallPy
      ~CallPy (void)
      {
         // dereference the python type
         if (pyResult == Py_None) Py_XDECREF (pyResult);

         // dereference the python function
         Py_XDECREF (pyFunc);

         // dereference the python module
         Py_XDECREF (pyModule);

         // dereference the python function
         Py_XDECREF (pyFunc);

         // dereference the python arguments
         Py_XDECREF (pyArgs);

         // close the python interpreter
         Py_Finalize ();
      }

      // function name: set_arg from CallPy
      template <typename type>
         int set_arg (type src)
         {
            int stat = 0;

            PyObject *pyValue;
            if (std::is_same <type, int>::value)
            {
               pyValue = PyInt_FromLong (src);
            }

            // set the python value as the argument
            stat |= PyTuple_SetItem (pyArgs, argument++, pyValue);

            // dereference the python value
            if (pyValue != nullptr) Py_DECREF (pyValue);

            return stat;
         }

      // function name: set_arg from CallPy
      template <typename type>
         int set_arg (type *src, int num_el)
         {
            int stat = 0;

            // instantiate a new python list
            PyObject *pyList = PyList_New (num_el);

            for (int ind = 0; ind < num_el; ind++)
            {
               if (std::is_same <type, int>::value)
                  pyType = PyInt_FromLong ((long)src[ind]);
               else if (std::is_same <type, float>::value)
                  pyType = PyFloat_FromDouble ((double)src[ind]);
               else
                  pyType = nullptr;

               // set each integer value to the python list
               PyList_SetItem (pyList, ind, pyType);
            }

            PyObject *list_as_tuple = PyTuple_New (num_el);

            list_as_tuple = PyList_AsTuple (pyList);

            // set the argument value
            stat |= PyTuple_SetItem (pyArgs, argument++, list_as_tuple);

            // dereference the python list
            if (pyList != nullptr) Py_DECREF (pyList);

            return stat;
         }

      // function name: execute from CallPy
      void execute (void)
      {
         if (pyFunc && PyCallable_Check (pyFunc))
         {
            // call the python function
            pyResult = PyObject_CallObject (pyFunc, pyArgs);
         }
      }

      template <typename type>
         type read_result (void)
         {
            if (PyTuple_Check (pyResult)) return 0;

            type result;
            if (PyInt_Check (pyResult))
            {
               result = static_cast<int>(PyInt_AsLong (pyResult));
            }
            else if (PyFloat_Check (pyResult))
            {
               result = static_cast<float>(PyFloat_AsDouble (pyResult));
            }

            return result;
         }

      template <typename type>
         void read_result (type *dst)
         {
            if (!PyTuple_Check (pyResult)) return;

            Py_ssize_t tuple_size = PyTuple_Size (pyResult);

            for (int ind = 0; ind < tuple_size; ind++)
            {
               pyType = PyTuple_GetItem (pyResult, ind);

               if (PyInt_Check (pyType))
               {
                  dst[ind] = static_cast<int>(PyInt_AsLong (pyType));
               }
               else if (PyFloat_Check (pyType))
               {
                  dst[ind] = static_cast<float>(PyFloat_AsDouble (pyType));
               }
            }
         }

   private:

      PyObject *pyModule;
      PyObject *pyFunc;
      PyObject *pyArgs;
      PyObject *pyResult;
      PyObject *pyType;

      int argument;
};

#endif

#if 0
+#if 0
+PyTypeObject *type = subElement->ob_type;
+const char* p = type->tp_name;
+std::cout << "My type is " << p << std::endl;
+#endif
#endif
