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
#include <es/es_handler_passthrough.h>
#include <stdio.h>

es_handler_sptr es_make_handler_passthrough(){
    return es_handler_sptr(new es_handler_passthrough());
}

es_handler_passthrough::es_handler_passthrough() :
    gr::sync_block ("es_handler_passthrough",
            gr::io_signature::make(0,0,0),
            gr::io_signature::make(0,0,0))
{ 
    message_port_register_out(pmt::mp("events_out"));
}

void es_handler_passthrough::handler( pmt_t msg, gr_vector_void_star buf ){
    message_port_pub(pmt::mp("events_out"), msg);
}




