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
#include <stdio.h>
#include <es/es.h>

pmt_t pmt_float_vector( std::vector< float > vec ){
    return pmt::init_f32vector(vec.size(), &vec[0]);
}

pmt_t pmt_complex_vector( std::vector< gr_complex > vec ){
    return pmt::init_c32vector(vec.size(), &vec[0]);
}

es_handler::es_handler()
    : gr::basic_block("handler", 
        gr::io_signature::make(0,0,0),
        gr::io_signature::make(0,0,0))
{
}

es_handler::es_handler(std::string name)
    : gr::basic_block(name, 
        gr::io_signature::make(0,0,0),
        gr::io_signature::make(0,0,0))
{
}

void es_handler::handler_helper( pmt_t msg ){
    handler( msg, get_buffer_ptr( event_field( msg, es::event_buffer ) ) );
}

es_handler::~es_handler(){
//    printf("Handler Base Class destructing (%x)!\n",this);
}

gr_vector_void_star es_handler::get_buffer_ptr(pmt_t buffer_arg){
    int nvec = pmt::length(buffer_arg);
    gr_vector_void_star outvec(nvec);
    for(int i=0; i<nvec; i++){
        pmt_t list_nth = pmt::nth(i,buffer_arg);
        if(pmt::is_any(list_nth)){
            boost::any ptr = pmt::any_ref( list_nth );
            outvec[i] = boost::any_cast<void*>(ptr);
        } else if(pmt::is_u8vector( list_nth ) ) {
            size_t buf_len = 0; // this is set by return, input val not used?
            const uint8_t* dataptr = pmt::u8vector_elements(list_nth, buf_len);
            outvec[i] = (void*) dataptr;
        } else if(pmt::is_blob( list_nth) ){
            //outvec[i] = pmt_blob_rw_data(list_nth);
            outvec[i] = (void*)pmt::blob_data(list_nth);
        } else {
            throw std::runtime_error("unknown pmt type in event buffer!");
        }
    }
    return outvec;
}

/*
*  default implementation of handler() should never be called,
*  maybe we should make this an abstract/pure-virtual method...
*/
void es_handler::handler(pmt_t msg, gr_vector_void_star buf){
    printf("es_handler::handler() this=%p (base class handler called!! not good)\n",this);
    throw std::runtime_error("base class handler called!! not good");
}   




