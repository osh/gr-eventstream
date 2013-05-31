#!/usr/bin/env python
#
# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of gr-eventstream
#
# gr-eventstream is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# gr-eventstream is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with gr-eventstream; see the file COPYING. If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

from gnuradio import gr, gr_unittest
import sys;
sys.path.append("../swig/");
sys.path.append("../swig/.libs/");
import es_swig as es;
import random;


class qa_es (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

#    def test_001_square_ff (self):
#        print "ok"
#        print dir(es);
    
#    def test_002_es_source (self):
#
#        tb = gr.top_block();
#
#        # create the arbiter shared memory space
#        arb = es.es_make_arbiter();
#
#        # create the source event queue
#        queue = es.queue();
#
#        # create the source block and set a max stream length on it
#        sig = gr.io_signature( 1,1,gr.sizeof_float );
#        src = es.source( arb, queue, sig );
#        src.set_max(20);
#       
#        # add a singular event
#        vec = es.pmt_float_vector([1,2,3,4]);
#        e1 = es.event_create_gen_vector_f( 3, vec );
#        h1 = es.es_handler_insert_vector();
#        h1p = es.make_handler_pmt( h1 );
#
#        queue.register_event_type( es.event_type( e1 ) );
#        queue.bind_handler( es.event_type( e1 ), h1p );
#        queue.add_event(e1);
#        
#        # set up a vector sink for printing
#        snk = gr.vector_sink_f();
#        tb.connect(src, snk);
#
#        # run the graph to completion and print output stream
#        tb.run();
#        print snk.data();


    
#    def test_003_es_flowgraph(self):
#        tb = gr.top_block();
#
#        data = [1,2,3,4];
#        src = gr.vector_source_c(data);
# 
#        e1 = es.event_create("Evt1", 2,1);
#        
#        hb = gr.hier_block2("blk", gr.io_signature(0,0,0), gr.io_signature(0,0,0));
#        h1 = es.es_handler_flowgraph( 1, hb);
 

    def test_004_es_loopback(self):
        tb = gr.top_block();
        arb = es.es_make_arbiter();
        queue = es.queue();
        queue_snk = es.queue();
        
        max_idx = 10000;

        sig = [gr.sizeof_float];
        src = es.source(arb, queue, sig );
        src.set_max(max_idx);
 
        count = 0;
        xloc = 0;
        firstiter = True;

        while xloc < max_idx:
            count = float(count + 1);
            xloc = xloc + random.randint(1,50);
            dvec  = [];
            for i in range(1,10):
                dvec.append(count);
            vec = es.pmt_float_vector( dvec );
            e1 = es.event_create_gen_vector_f( xloc, vec );
            queue.add_event( e1 );
    
        print "queue adding done"
           
        snk = gr.vector_sink_f();
        #snk = es.sink(arb, queue_snk, sig);      
        tb.connect(src,snk);
 
        print "Total Events = %d"%(count);

        tb.run();
        print snk.data();
           
            
            


if __name__ == '__main__':
    gr_unittest.main ()

