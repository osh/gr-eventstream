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
#include <es/es_handler_pdu.h>
#include <stdio.h>

es_handler_sptr es_make_handler_pdu(es_handler_pdu::DATATYPE type){
    return es_handler_sptr(new es_handler_pdu(type));
}

es_handler_pdu::es_handler_pdu( DATATYPE type ) :
    gr::sync_block ("es_sink",
            gr::io_signature::make(0,0,0),
            gr::io_signature::make(0,0,0))
{ 
    message_port_register_out(pmt::mp("pdus_out"));
    d_type = type;
}

void es_handler_pdu::handler( pmt_t msg, gr_vector_void_star buf ){
    if(!is_event(msg))
        throw std::runtime_error("input to es_handler_pdu::handler(msg ...) msg must be an event!");
    pmt::pmt_t meta = pmt::tuple_ref(msg,1);
    switch(d_type){
        case TYPE_F32:
            {
            float* fbuf = (float*) buf[0];
            pmt::pmt_t vec = pmt::init_f32vector( event_length(msg), fbuf );
            pmt::pmt_t pdu = pmt::cons( meta, vec );
            message_port_pub(pmt::mp("pdus_out"), pdu);
            break;
            }
        case TYPE_C32:
            {
            gr_complex* cbuf = (gr_complex*) buf[0];
            pmt::pmt_t vec = pmt::init_c32vector( event_length(msg), cbuf );
            pmt::pmt_t pdu = pmt::cons( meta, vec );
            message_port_pub(pmt::mp("pdus_out"), pdu);
            break;
            }
        default:
            printf("unknown vector content type.\n");
        }
}




