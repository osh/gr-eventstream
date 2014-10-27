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

#include <qa_es_source.h>
#include <cppunit/TestAssert.h>

#include <stdio.h>
#include <es/es.h>


// Test event generation, queue insertion, general non gr-runtime operation
void 
qa_es_source::t1()
{
    printf("t1\n");

    gr_vector_int outsig(1);
    outsig[0] = sizeof(float);
    es_source_sptr s = es_make_source(outsig);
 
    pmt_t e1 = event_create( es::event_type_1, 1, 4 );
    pmt_t e2 = event_create( es::event_type_1, 10, 4 );
    pmt_t e3 = event_create( es::event_type_1, 20, 4 );
 
    //q->add_event(e1);
    //q->add_event(e2);
    //q->add_event(e3);
 
    //q->print_queue();
     
}


#include <gnuradio/top_block.h>
#include <gnuradio/blocks/vector_sink_f.h>
#include <gnuradio/blocks/vector_sink_c.h>

// Test gr-runtime operation of single event item
void
qa_es_source::t2()
{

    printf("QA_ES_SOURCE::t2\n");

    gr_vector_int outsig(1);
    outsig[0] = sizeof(float);
    es_source_sptr s = es_make_source(outsig);

    s->set_max(10);
 
    std::vector<float> const_ramp;
    const_ramp.push_back(1.0);
    const_ramp.push_back(0.75);
    const_ramp.push_back(0.5);
    const_ramp.push_back(0.25);
    
    // set up the event
    pmt_t e1_vector = pmt::init_f32vector( const_ramp.size(), &const_ramp[0] );
    pmt_t e1 = event_create_gen_vector_f( (uint64_t)3, e1_vector );   
    
    
    printf("QA_ES_SOURCE::t200CMT BARRIER\n");

    gr::top_block_sptr tb = gr::make_top_block("t2 graph");
    gr::blocks::vector_sink_f::sptr vs = gr::blocks::vector_sink_f::make();
 
    tb->connect( s, 0, vs, 0 );
    printf("running flowgraph.\n");
    tb->run();
 
    std::vector<float> out_data = vs->data();
    printf("total_output_vector=[");
    for(int i=0; i<out_data.size(); i++){
        printf("%f,", out_data[i]);
    }
    printf("]\n");
    
}


// Test gr-runtime operation of single event item
void
qa_es_source::t3()
{

    printf("QA_ES_SOURCE::t3\n");

    gr_vector_int outsig(1);
    outsig[0] = sizeof(gr_complex);
    es_source_sptr s = es_make_source(outsig);

    s->set_max(100);

    std::vector<gr_complex> const_ramp;
    const_ramp.push_back(gr_complex(1.0,0));
    const_ramp.push_back(gr_complex(0.75,0.5));
    const_ramp.push_back(gr_complex(0.5,1));
    const_ramp.push_back(gr_complex(0.25,2));

    // set up the event
    pmt_t e1_vector = pmt::init_c32vector( const_ramp.size(), &const_ramp[0] );
    pmt_t e1 = event_create( pmt::mp("pdu_event"), 0, const_ramp.size() );
    e1 = event_args_add(e1, pmt::mp("vector"), e1_vector);

    gr::top_block_sptr tb = gr::make_top_block("t3 graph");
    gr::blocks::vector_sink_c::sptr vs = gr::blocks::vector_sink_c::make();
    s->schedule_event(e1);

    tb->connect( s, 0, vs, 0 );
    printf("running flowgraph.\n");
    tb->run();

    std::vector<gr_complex> out_data = vs->data();
    printf("total_output_vector=[");
    for(int i=0; i<out_data.size(); i++){
        printf("(%f,%f),", out_data[i].real(), out_data[i].imag());
    }
    printf("]\n");

}


