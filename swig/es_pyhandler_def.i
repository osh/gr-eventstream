/* -*- c++ -*- */
/*
 * Copyright 2011 Free Software Foundation, Inc.
 * 
 * This file is part of gr-eventstream
 * 
 * gr-eventstream is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * gr-eventstream is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with gr-eventstream; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

%{
#include <es/es_pyhandler_def.h>
#include <gruel/pmt.h>
using namespace std;
using namespace pmt;
%}

%include "gnuradio.i"
%include "typemaps.i"
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

%include <swig/gr_top_block.i>
%include <swig/gr_hier_block2.i>
%include <swig/gr_basic_block.i>

%template(stdMapStringBlock) std::map<std::string, gr_basic_block_sptr>;
%template(stdMapStringString) std::map<std::string, std::string>;

class es_pyhandler {
    public:
        es_pyhandler();
        gr_top_block_sptr tb;
        es_vector_source_sptr source;
        es_vector_sink_sptr sink;
        std::map <std::string, gr_basic_block_sptr> blocks;
        PyObject* b2;
        void set_b2(PyObject* _b2);
        PyObject* vars;
        void set_vars(PyObject* _vars);
};

struct es_hook_rval {
    std::map<std::string, std::string> map;
};



class es_hook_args {
  private:
    es_hook_args() {};
  public:
    es_hook_args(gr_vector_void_star *bufs, const pmt::pmt_t msg, es_pyhandler* handler);
    gr_vector_void_star *bufs;
    const pmt_t msg;
    es_pyhandler* handler;
};


// Grab a Python function object as a Python object.
%typemap(python,in) PyObject *pyfunc {
  if (!PyCallable_Check($source)) {
      PyErr_SetString(PyExc_TypeError, "Need a callable object!");
      return NULL;
  }
  $target = $source;
}

// Type mapping for grabbing a FILE * from Python
%typemap(python,in) FILE * {
  if (!PyFile_Check($source)) {
      PyErr_SetString(PyExc_TypeError, "Need a file!");
      return NULL;
  }
  $target = PyFile_AsFile($source);
}


%{
es_pyhandler* PythonFactoryCallBack(int index, void *clientdata)
{
   PyObject *func, *arglist;
   PyObject *result;
   es_pyhandler* dres;
   
   func = (PyObject *) clientdata;               // Get Python function
   arglist = Py_BuildValue("(l)",index);             // Build argument list
   result = PyEval_CallObject(func,arglist);     // Call Python
   Py_DECREF(arglist);                           // Trash arglist
   if (result) {                                 // If no errors, return double
        // SWIG_POINTER_DISOWN keeps the destructor from running ...
        int rv = SWIG_ConvertPtr(result, (void**)&dres, SWIGTYPE_p_es_pyhandler, SWIG_POINTER_DISOWN);
   } else {
        printf("PyEval did not return result!\n");
        PyErr_Print();
        assert(0);
    }

   Py_XDECREF(result);
   return dres;
}
%}

%{
using namespace pmt;
es_hook_rval* PythonHookCallBack(struct es_hook_args* args, void *clientdata)
{
   // some locals
   PyObject *func, *arglist, *swigged_args;
   PyObject *result;
   es_hook_rval* dres;

   // make sure we have the global interpreter lock
   PyGILState_STATE state = PyGILState_Ensure();

   // if no hook is set just return
   func = (PyObject *) clientdata;               // Get Python function
   if(clientdata == Py_None){ return NULL; } 

   // get our args swig item ready to pass
   swigged_args = SWIG_NewPointerObj( (void*) args, SWIGTYPE_p_es_hook_args, 0 );
   if(swigged_args == NULL){ throw std::runtime_error("SWIG_NewPointerObj(swigged_args) failed in PythonHookCallBack"); }

   // build arglist
   arglist = Py_BuildValue("(N)",swigged_args);             // Build argument list
   if(arglist == NULL){ throw std::runtime_error("Py_BuildValue() failed in PythonHookCallBack"); }

   // perform function call
   result = PyEval_CallObject(func,arglist);     // Call Python

   // de-ref count our args
   Py_DECREF(arglist);                           // Trash arglist
   Py_DECREF(swigged_args);                           // Trash swigged_args

   // check return values
   if (result) {                                 // If no errors, return double
        // SWIG_POINTER_DISOWN keeps the destructor from running ...
        int rv = SWIG_ConvertPtr(result, (void**)&dres, SWIGTYPE_p_es_hook_rval, SWIG_POINTER_DISOWN);
   } else {
        printf("PyEval did not return result!\n");
        PyErr_Print();
        assert(0);
   }
   // de-ref count return val, we are done with it
   Py_XDECREF(result);

   // release the global interpreter lock
   PyGILState_Release(state);

   return dres;
}
%}


class es_pyhandler_def {
    public:
        es_pyhandler_def();
        es_pyhandler* run_factory();
        es_hook_rval*    run_pre_hook(gr_vector_void_star &bufs, const pmt_t &msg, es_pyhandler* handler);
        es_hook_rval*    run_post_hook(gr_vector_void_star &bufs, const pmt_t &msg, es_pyhandler* handler);
};



%extend es_pyhandler {
    void set_pyb2(PyObject *_b2){
        self->set_b2( (PyObject*) _b2 );
        Py_INCREF(_b2);
    }
    PyObject* pb2(){
        PyObject* foo = self->b2;
        Py_INCREF(foo);
        return foo;
    }
    
    void set_pyvars(PyObject *_vars){
        self->set_vars( (PyObject*) _vars );
        Py_INCREF(_vars);
    }
    PyObject* pvars(){
        PyObject* foo = self->vars;
        Py_INCREF(foo);
        return foo;
    }

}


/* %addmethods es_pyhandler_def { */
%extend es_pyhandler_def {

   void set_pyfactory_cb(PyObject *pyfunc) {
     self->set_factory_cb(PythonFactoryCallBack, (void *) pyfunc);
     Py_INCREF(pyfunc);
   }

   void set_pypre_hook_cb(PyObject *pyfunc) {
     self->set_pre_hook_cb(PythonHookCallBack, (void *) pyfunc);
     Py_INCREF(pyfunc);
   }

   void set_pypost_hook_cb(PyObject *pyfunc) {
     self->set_post_hook_cb(PythonHookCallBack, (void *) pyfunc);
     Py_INCREF(pyfunc);
   }
 
   gr_vector_void_star empty_vec(){
     return gr_vector_void_star();
   }

}




