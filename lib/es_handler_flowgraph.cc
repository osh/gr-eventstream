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

#include <es/es.h>
#include <es/es_handler_flowgraph.h>
#include <stdio.h>

#include <gnuradio/blocks/message_source.h>
#include <gnuradio/top_block.h>


es_handler_flowgraph::es_handler_flowgraph( es_pyhandler_def* _pyhd, int pool_size ) : 
    pyhd(_pyhd),
    d_free_pyhandlers(100)
{
    // generate some initial number of handlers
    int initial_handlers = pool_size;
    for(int i=0; i<initial_handlers; i++){
        es_pyhandler* h = pyhd->run_factory();
        d_pyhandlers.push_back( h );
        d_free_pyhandlers.push( h );
    }
}


es_handler_flowgraph::~es_handler_flowgraph(){
}
void es_handler_flowgraph::handler( pmt_t msg, gr_vector_void_star buf ){

    // locals
    es_pyhandler* handler;
    int num_items = event_length(msg);

    // grab a free handler to service this request
    d_free_pyhandlers.pop( handler );

    // set up the source vectors if neccusary (for es.sink handler)
    if(handler->source.get() != NULL && handler->source->shape().size() > 0){
        // TODO: type checking on source vs buf item sizes/io_sig?
        gr_vector_const_void_star constbuf( buf.size() );
        for(int i=0; i<buf.size(); i++){
            constbuf[i] = (const void*) buf[i];
        }
        handler->source->set_data( constbuf, num_items, false );
    }


    // clear the output sink
    handler->sink->clear();

    // execture the pre-hook callback
    es_hook_rval* rval = pyhd->run_pre_hook(buf, msg, handler);
    

    // execture the top block
    //printf("es_handler_flowgraph::handler(), calling tb->run() !\n");
    handler->tb->run();
    //printf("es_handler_flowgraph::handler(), tb->run() returned !\n");

    // execute the post-hook callback
    pyhd->run_post_hook(buf, msg, handler);

    // copy vector contents to output buffer (for es.source handler)
    if(handler->sink.get() != NULL && handler->sink->shape().size() > 0){
        printf("es_handler_flowgraph::handler() copying output buffer.\n");
        gr_vector_const_void_star data = handler->sink->data();
        for(int i=0; i<data.size(); i++){
            int item_size = handler->sink->shape()[i];
            printf("num_items = %d, itemsize = %d\n", num_items, item_size );
            printf("buf[i] = %p\n", buf[i]);
            printf("data[i] = %p, datalen = %d\n", data[i], handler->sink->data_len_items());
            if(handler->sink->data_len_items() > num_items){
                printf("(sink.length = %d > evt_len = %d)\n",handler->sink->data_len_items(), num_items);
                throw std::runtime_error("mismatch between flowgraph sink and evt length item counts");
            }
            int gen_items = handler->sink->data_len_items();
            printf("(es_handler_flowgraph) producing (sink len) --- %d items, padding by %d\n", gen_items, num_items-gen_items);
            memcpy( buf[i], data[i], gen_items * item_size );
            memset( (void*)((uint64_t)buf[i] + gen_items*item_size), 0x00, (num_items-gen_items)*item_size );
        }
    }

    // release the handler instance to free queue
    d_free_pyhandlers.push( handler );

}


es_handler_sptr es_make_handler_flowgraph(es_pyhandler_def* _pyhd, int pool_size){
    return es_handler_sptr(new es_handler_flowgraph(_pyhd,pool_size));
}

