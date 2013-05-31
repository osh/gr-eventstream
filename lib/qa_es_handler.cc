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

#include <qa_es_handler.h>
#include <cppunit/TestAssert.h>

#include <stdio.h>
#include <es/es.h>

// Test event generation, queue insertion, handler binding, general non gr-runtime operation
void 
qa_es_handler::t1()
{
    printf("t1\n");
    es_queue_sptr q = es_make_queue();
    pmt_t arb = es_make_arbiter();
    es_source_sptr s = es_make_source(arb,q,sizeof(float));
    std::vector<float> const_ramp;
    const_ramp.push_back(1.0);
    const_ramp.push_back(0.75);
    const_ramp.push_back(0.5);
    const_ramp.push_back(0.25);
    es_event_sptr e1 = es_event_sptr(new event_gen_vector_f( arb, const_ramp ));
    e1->set_time(100);
    es_handler* h1 = new es_handler_print();
    es_handler_sptr h1s( h1 );
    q->register_event_type(e1->type());
    q->bind_handler( e1->type(), h1s );
    q->add_event(e1);
    q->print();
}



