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
//#include <es/es_handler_print.h>
#include <stdio.h>

es_handler_sptr es_make_handler_print(es_handler_print::DATATYPE type){
    return es_handler_sptr(new es_handler_print(type));
}

es_handler_print::es_handler_print( DATATYPE type ){
    printf("new print handler %p\n", this);
    d_type = type;
}

//void es_handler_print::handler( pmt_t msg, void* buf ){
void es_handler_print::handler( pmt_t msg, gr_vector_void_star buf ){
    event_print(msg);
    switch(d_type){
        case TYPE_F32:
            {
            printf(" vector_contents = [");
            float* fbuf = (float*) buf[0];
            for(int i=0; i<event_length(msg); i++){
                printf("%f, ", fbuf[i]);
                }
            printf("]\n");
            break;
            }
        case TYPE_C32:
            {
            printf(" vector_contents = [");
            gr_complex* fbuf = (gr_complex*) buf[0];
            for(int i=0; i<event_length(msg); i++){
                printf("%f+%fj, ", fbuf[i].real(), fbuf[i].imag());
                }
            printf("]\n");
            break;
            }
        default:
            printf("unknown vector content type.\n");
        }

}




