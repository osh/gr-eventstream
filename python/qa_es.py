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

from gnuradio import gr, gr_unittest, blocks
import pmt
import sys;
sys.path.append("../swig/");
sys.path.append("../swig/.libs/");
import es_swig as es;
import random;
import numpy;

class qa_es (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_alloc_things(self):
        b = []
        b.append(es.sink([1,1], 8))
        b.append(es.source([1], 8))
        b.append(es.es_handler_file( es.es_handler_file.TYPE_F32, "/dev/null", "test"))
        b.append(es.es_handler_insert_vector())
        b.append(es.es_handler_print(es.es_handler_print.TYPE_C32))
        b.append(es.es_handler_pdu(es.es_handler_pdu.TYPE_C32))
        b.append(es.trigger_edge_f(0, 100, 0, 1, 0))
        b.append(es.trigger_sample_timer(1, 1000, 0, 0, 100))
    
    def test_002_es_source (self):

        tb = gr.top_block();
        src = es.source( [gr.sizeof_float], 1 );
        src.set_max(20);

        # set up a vector sink for printing
        snk = blocks.vector_sink_f();
        tb.connect(src, snk);

        # run the graph to completion and print output stream
        tb.run();
        print snk.data();
        print len(snk.data());
        self.assertEqual( len(snk.data()), 20);

    
    def test_003_es_sink (self):

        iv = [0,1,2,3,4,5,6,7,8,9];
        src = blocks.vector_source_f(iv, repeat=True);
        hd = blocks.head(gr.sizeof_float, 10000);
        snk = es.sink([gr.sizeof_float], 1);
        t = es.trigger_sample_timer(gr.sizeof_float, 10, 5, 10, 10);
        tb = gr.top_block();
        pduh = es.es_make_handler_pdu(es.es_handler_print.TYPE_F32);
        msgdb = blocks.message_debug()
        tb.connect(src, hd, t, snk);   
        tb.msg_connect( t, "which_stream", snk, "schedule_event" )
        tb.msg_connect( t, "sample_timer_event", pduh, "handle_event" )
        tb.msg_connect( pduh, "pdus_out", msgdb, "store" )
        tb.run();
 
        # expected output of each event in the periodic sequence   
        sv = numpy.array( iv[5:] + iv[:5], dtype=numpy.float );

        # verify each received message
        nm = msgdb.num_messages();
        print "nm = %d"%(nm);
        for i in range(0, nm):
            m = msgdb.get_message(i);
            mp = pmt.to_python(pmt.cdr(m));
            print mp;
            self.assertEqual( sv.all(), mp.all() );



if __name__ == '__main__':
    gr_unittest.main ()

