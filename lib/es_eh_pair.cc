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

#include <es/es_eh_pair.hh>
#include <es/es_common.h>
#include <es/es_handler.h>
#include <stdio.h>

es_eh_pair::es_eh_pair(pmt_t _event, pmt_t _handler) :
    handler(_handler), 
    event(_event)
    {

}

void es_eh_pair::run(){
//    printf("es_eh_pair::run()\n");
//    handler->hanler( 

    // new style call
    es_handler_sptr h = boost::any_cast< es_handler_sptr >(pmt_any_ref(handler));
    h->handler_helper( event );

/*  // old way -- pmt msg acceptor ref
    boost::shared_ptr<gruel::msg_accepter> h = pmt_msg_accepter_ref( handler );
    h->post( pmt::PMT_NIL, event );
*/

}


unsigned long long es_eh_pair::time(){
    return event_time( event );
}

unsigned long long es_eh_pair::length(){
    return event_length( event );
}

es_eh_pair::~es_eh_pair(){
//    printf("es_eh_pair::destructor running.\n");
}


