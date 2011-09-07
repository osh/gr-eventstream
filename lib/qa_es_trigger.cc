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

#include <qa_es_trigger.h>
#include <cppunit/TestAssert.h>

#include <stdio.h>
#include <es/es.h>


#include <gnuradio/gr_top_block.h>
#include <gnuradio/gr_vector_source_f.h>

// Test gr-runtime operation of single event item
void
qa_es_trigger::t1()
{
    printf(" *** BEGIN QA_ES_TRIGGER_T1\n");
 
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
    pmt_t arb = es_make_arb();
    es_queue_sptr queue = es_make_queue();
    
    // build gnu radio blocks
    gr_top_block_sptr tb = gr_make_top_block("qa_es_trigger_t1_top");
    gr_vector_source_f_sptr src = gr_make_vector_source_f(vec);

    gr_vector_int insig(1);
    insig[0] = sizeof(float);
    es_sink_sptr snk = es_make_sink( arb, queue, insig, 1 );
//    es_sink_sptr snk = es_make_sink( arb, queue, sizeof(float) );

    es_trigger_edge_f_sptr trig = es_make_trigger_edge_f(arb, queue, 0.1, 4, 1, sizeof(char),1, "EDGE_TRIGGER_EVENT");

    // connect traditional flowgraph portion
    tb->connect( src, 0, trig, 0 );
    tb->connect( trig, 0, snk, 0 );
    
    // create the event handler, just a print handler in this case   
    //pmt_t h1 = make_handler_pmt( new es_handler_print(es_handler_print::TYPE_F32) );
    es_handler_sptr h1 = es_make_handler_print(es_handler_print::TYPE_F32);
    
    // register the trigger blocks event type as a known event type handled by this queue
    queue->register_event_type( trig->event_type(0) );

    // bind a handler to the trigger block's first output event type
    queue->bind_handler( trig->event_type(0), h1 );

    // run the waveform
    tb->run();

    printf(" *** END QA_ES_TRIGGER_T1\n");
}


