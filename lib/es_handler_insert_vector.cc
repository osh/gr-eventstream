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

#include <es/es.h>
#include <stdio.h>
#include <string.h>


es_handler_sptr es_make_handler_insert_vector(){
    return es_handler_sptr(new es_handler_insert_vector());
}

es_handler_insert_vector::es_handler_insert_vector( ){
}

//void es_handler_insert_vector::handler( pmt_t msg, void* buf ){
void es_handler_insert_vector::handler( pmt_t msg, gr_vector_void_star buf ){

//    printf("INSERT VECTOR HANDLER RUNNING!\n");
    pmt_t vector = event_field( msg, pmt::intern("vector") );
 
    // AAA: GNU Radio Bug ? 
    // pmt_is_vector(vector) should return true!
    //assert( pmt_is_vector( vector)==true );

    size_t len = 0;

    // Insert a 32 bit Floating Point single vector
    if( event_type_compare( msg, es::event_type_gen_vector_f ) ){
        printf("handling vector_f event.\n");
        const float* data = pmt::f32vector_elements( vector, len );
        for(int j=0; j<buf.size(); j++){
            float* fbuf = (float*) buf[j];
            for(int i=0; i<len; i++){
                fbuf[i] = data[i];
            }
        }

    } else 

    // Insert a 32 bit Floating Point single COMPLEX vector
    if( event_type_compare( msg, es::event_type_gen_vector_c ) ){
        printf("handling vector_c event.\n");
        const gr_complex* data = pmt::c32vector_elements( vector, len );
        for(int j=0; j<buf.size(); j++){
            gr_complex* cbuf = (gr_complex*) buf[j];
            for(int i=0; i<len; i++){
                cbuf[i] = data[i];
            }
        }

    } else 

    if( event_type_compare( msg, es::event_type_gen_vector_b ) ){
        printf("handling vector b event.\n");
        const unsigned char* data = pmt::u8vector_elements( vector, len);
        for(int i=0; i<buf.size(); i++){
            memcpy( buf[i], data, len);
        }

    } else 

    /* 
    * this is the handler for n holdover bufs of different types
    * meanth for spanning multiple source buffers
    */
    if( event_type_compare( msg, es::event_type_gen_vector ) ){
        for(int i=0; i<buf.size(); i++){
            pmt_t buf_i = pmt::nth( i, vector );
            size_t blen = 0;
            const uint8_t * bufdatasrc = pmt::u8vector_elements( buf_i, blen );
            // we really should be making sure buffer lenghts line up here.
            //assert( blen == len * d_output_signature->sizeof_stream_item(i) );
            memcpy( buf[i], bufdatasrc, blen );
        }

    } else

    {
        throw std::runtime_error("insert_vector handler was called with a malformed message.");
    }

}




