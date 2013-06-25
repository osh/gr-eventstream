#!/usr/bin/env python
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

import random;
import time;
import sys;
from gnuradio import gr, audio, digital, filter;
import es;
import pmt;
import numpy;

SNR_db = 13;
sig_amp = 1.0;
noise_amp = 10**(-20.0/SNR_db);
print sig_amp, noise_amp;
fs = 1e5;
key_sym = pmt.intern("key");

# Create our handler flowgraph, define the event's sources and/or sinks
def key_factory(index):
    print "FACTORY CALLED index = %d"%(index);
    
    r = es.es_pyhandler();

    excess_bw = 0.5;
    sps = 4;
    amplitude = sig_amp;
    taps = gr.firdes.root_raised_cosine(sps*amplitude,  # Gain
                        sps,        # Sampling rate
                        1.0,        # Symbol rate
                        excess_bw,      # Roll-off factor
                        11*sps)         # Number of taps

    blocks = {};
    blocks["src"] = es.vector_source([1]) 
    blocks["scrambler"] = digital.scrambler_bb(0x8A, 0x7F, 7);
    blocks["mapper"] = digital.chunks_to_symbols_bc( [-1+0j, 0+1j, 1+0j, 0-1j] );
    blocks["rrc"] = filter.interp_fir_filter_ccf(sps, taps);
    r.sink = es.vector_sink([gr.sizeof_gr_complex]);
    r.set_pyb2(blocks);

    tb  = gr.top_block();
    tb.connect( blocks["src"], blocks["scrambler"], blocks["mapper"], blocks["rrc"], r.sink );

    r.tb = tb.to_top_block();
    return r;

# Create our handler's hook callback to be run before and after event flowgraph execution
def pre_hook(val):
    payload = es.event_field( val.msg, key_sym );   
    payload = pmt.symbol_to_string( payload );
    v = numpy.fromstring( payload, dtype=numpy.byte );
    blocks = val.handler.pb2();
    blocks["src"].set_data( es.StrVector([payload]), len(v) );
    r = es.es_hook_rval();
    return r;

# post hook, reset the head blocks' counters
def post_hook(val):
#    val.handler.sink.clear();
    r = es.es_hook_rval();
    return r;
    


# define our new event handler
key_handler = es.es_pyhandler_def();
key_handler.set_pyfactory_cb(key_factory);
key_handler.set_pypre_hook_cb(pre_hook);
#key_handler.set_pypost_hook_cb(post_hook);
key_handler_graph = es.es_handler_flowgraph( key_handler,2 );

# set up some common es components
arb = es.es_make_arbiter();
queue = es.queue();

# most packets will got out as scheduled
# but if we are ever later just send it as soon as possible
queue.set_early_behavior(1);
#queue.set_early_behavior(0);

# set up the main flow graph
tb = gr.top_block();
src = es.source( arb, queue, [gr.sizeof_gr_complex] );
#sink = gr.file_sink(gr.sizeof_gr_complex , "outfile.dat" );
sink = gr.udp_sink( gr.sizeof_gr_complex, "localhost", 12345 );
summer = gr.add_cc();
noise = gr.noise_source_c( gr.GR_GAUSSIAN, noise_amp);
throttle = gr.throttle(gr.sizeof_gr_complex, fs);
tb.connect(src, summer, sink);
tb.connect(noise, throttle, (summer,1));

# create initial event, set up event bindings, handlers
e1 = es.event_create("burst_transmit", 10, 1000 );
e1 = es.event_args_add( e1, key_sym, pmt.intern("1") );
queue.register_event_type( es.event_type( e1 ) );
queue.bind_handler( es.event_type( e1 ), es.make_handler_pmt(key_handler_graph) );

# start the main flowgraph
tb.start();


import Tkinter as tk;
import sys;
import subprocess;
 
while(True):
    time.sleep(0.2*random.random());  

    print "queue length = %d"%( queue.length() );
    # if we have a backlog waiting to go out go back to sleep
    if(queue.length() > 4):
        continue;
    
    try:
        quote = subprocess.check_output(["fortune"])
    except:
        print "ERROR: fortune not found, please install fortune!"
        sys.exit(-1);       


    # transmit 
    timenow = src.time();
    txtime = timenow + random.randint(0,1e4);

    key_evt = es.event_create("burst_transmit", txtime, int(100e2) );
    key_evt = es.event_args_add( key_evt, key_sym, pmt.intern(str(quote)) );
    es.event_print(key_evt);
    queue.add_event( key_evt );
    
    
  

#tb.stop();
#tb.wait();
