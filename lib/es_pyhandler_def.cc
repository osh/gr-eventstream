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

#include <Python.h>
#include <stdio.h>
#include <es/es_pyhandler_def.h>

#define DEBUG(X) 

const gr_vector_int emptyvec(0);


es_hook_args::es_hook_args( gr_vector_void_star *_bufs, const pmt::pmt_t _msg, es_pyhandler* _handler)
    : bufs(_bufs), msg(_msg), handler(_handler)
{
}


// default constructor for pyhandler
// setup pointers to not point to nowhere
//
es_pyhandler::es_pyhandler() :
    source( es_make_vector_source(emptyvec) ),
    sink( es_make_vector_sink( emptyvec ) ),
    tb( gr::make_top_block("default pyhandler top block") ),
    b2(NULL)
{
    DEBUG(printf("es_pyhandler constructor running (this=%x)\n", this);)

    // make sure python is ready for threading
    if( Py_IsInitialized() ){
        if(PyEval_ThreadsInitialized() != 1 ){
            PyEval_InitThreads();
        }
    } else {
        throw std::runtime_error("dont use es_pyhandler without python!");
    }


//    printf("source = %x\n", source.get() );
}

es_pyhandler::~es_pyhandler(){
    DEBUG(printf("es_pyhandler destructor running (this=%x)\n", this);)
    if(b2!=NULL){
        Py_DECREF(b2);
    }
}

void es_pyhandler::set_b2(PyObject* _b2){
    b2 = _b2;
    Py_INCREF(b2);
}

void es_pyhandler::set_vars(PyObject* _vars){
    vars = _vars;
    Py_INCREF(vars);
}


//
// default factory function handler
//
es_pyhandler* default_factory_cb( int index, void* _cb_data ){
    throw std::runtime_error("default_factory_cb called, please define one...");
    return NULL;
}

//
// default hook function handler
//
es_hook_rval* default_hook_cb( gr_vector_void_star bufs, const pmt::pmt_t &msg, void* _cb_data){
    DEBUG(printf("default_hook_cb called, noop\n");)
    return NULL;
}

//
// constructor
//
es_pyhandler_def::es_pyhandler_def() :
    d_index(0),
    pre_hook_cb( NULL ),
    pre_hook_data( NULL ),
    post_hook_cb( NULL ),
    post_hook_data( NULL )
{
    set_factory_cb(default_factory_cb, 0);
}


//
// setters for default handler callback functions 
// c++ versions which are proxied to python via swig
//
void es_pyhandler_def::set_factory_cb(FACTORYCBTYPE fp, void* _cb_data){
    DEBUG(printf("es_pyhandler_def::set_factory_cb(%x, %x)\n", fp, _cb_data);)
    factory_callback = fp;
    factory_cb_data = _cb_data;
}
void es_pyhandler_def::set_pre_hook_cb(HOOKCBTYPE fp, void* _cb_data){
    DEBUG(printf("es_pyhandler_def::set_pre_hook_cb(%x, %x)\n", fp, _cb_data);)
    pre_hook_cb = fp;
    pre_hook_data = _cb_data;
}
void es_pyhandler_def::set_post_hook_cb(HOOKCBTYPE fp, void* _cb_data){
    DEBUG(printf("es_pyhandler_def::set_post_hook_cb(%x, %x)\n", fp, _cb_data);)
    post_hook_cb = fp;
    post_hook_data = _cb_data;
}

es_pyhandler* es_pyhandler_def::run_factory(){
    return factory_callback( d_index++, factory_cb_data );
}

es_hook_rval* es_pyhandler_def::run_pre_hook(gr_vector_void_star &bufs, const pmt::pmt_t &msg, es_pyhandler* handler){
    struct es_hook_args hargs(&bufs, msg, handler);
    if(pre_hook_cb == NULL){ 
        return NULL;
    } else {
        return pre_hook_cb( &hargs, pre_hook_data );
    }
}

es_hook_rval* es_pyhandler_def::run_post_hook(gr_vector_void_star &bufs, const pmt::pmt_t &msg, es_pyhandler* handler){
    struct es_hook_args hargs(&bufs, msg, handler);
    if(post_hook_cb == NULL){ 
        return NULL;
    } else {
        return post_hook_cb( &hargs, post_hook_data );
    }
}



