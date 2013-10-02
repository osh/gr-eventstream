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

#include <qa_es_sink.h>
#include <cppunit/TestAssert.h>

#include <stdio.h>
#include <es/es.h>


#include <gnuradio/top_block.h>
#include <gnuradio/blocks/vector_source_f.h>

// Test gr-runtime operation of single event item
void
qa_es_sink::t1()
{
    printf(" *** BEGIN QA_ES_SINK_T1\n");
 
    // build input test vector
    std::vector<float> vec;
    vec.push_back(0);
    vec.push_back(0);
    vec.push_back(0);
    vec.push_back(4);
    vec.push_back(3);
    vec.push_back(2);
    vec.push_back(1);
    vec.push_back(0);
    vec.push_back(0);
    vec.push_back(0);
    vec.push_back(0);
    
    // create eventstream control objects
    
    // build gnu radio blocks
    gr::top_block_sptr tb = gr::make_top_block("qa_es_sink_t1_top");
    gr::blocks::vector_source_f::sptr src = gr::blocks::vector_source_f::make(vec);
    
    gr_vector_int insig(1);
    insig[0] = sizeof(float);
    es_sink_sptr snk = es_make_sink( insig, 1 );
    
    // connect traditional flowgraph portion
    tb->connect( src, 0, snk, 0 );
    
    pmt_t evt1 = event_create("NEW_EVT_TYPE_3_4", 3, 4);
    pmt_t evt2 = event_create("NEW_EVT_TYPE_5_4", 5, 4);

    //pmt_t h1 = make_handler_pmt( new es_handler_print(es_handler_print::TYPE_F32) );
    es_handler_sptr h1 = es_make_handler_print(es_handler_print::TYPE_F32);
    
    // register the NEW_EVT_TYPE as a known event type handled by this queue
//    queue->register_event_type( event_type( evt1 ) );

    // bind a handler to the NEW_EVT_TYPE
//    queue->bind_handler( event_type( evt1 ), h1 );

    // to define this event type and not discard it run ...
//    queue->register_event_type( event_type( evt2 ) );
//    queue->bind_handler( event_type( evt2 ), h1 );

    // add the two events to the queue
//    queue->add_event(evt1);
//    queue->add_event(evt2);
 
    printf("tb->run()\n");
      
    // run the waveform
    tb->run();
    
    printf(" *** END QA_ES_SINK_T1\n");
}


