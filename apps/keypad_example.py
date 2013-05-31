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

import sys;
from gnuradio import gr, audio;
import es;
import pmt;

fs = 48e3;
evt_len = int(100e-3*fs);
key_sym = pmt.pmt_intern("key");

# Create our handler flowgraph, define the event's sources and/or sinks
def key_factory(index):
    print "FACTORY CALLED index = %d"%(index);
    r = es.es_pyhandler();
    src_L = gr.sig_source_f( 48e3, gr.GR_SIN_WAVE, 1440.0, 1.0, 0 );
    src_R = gr.sig_source_f( 48e3, gr.GR_SIN_WAVE, 480.0, 1.0, 0 );
    head_L = gr.head( gr.sizeof_float, evt_len );
    head_R = gr.head( gr.sizeof_float, evt_len );

    r.sink = es.vector_sink([gr.sizeof_float, gr.sizeof_float]);
    tb  = gr.top_block();
    tb.connect( (src_L,0), (head_L,0), (r.sink,0) );
    tb.connect( (src_R,0), (head_R,0), (r.sink,1) );

    # store blocks where we can get to them
    blocks = {};
    blocks["src_L"] = src_L;
    blocks["src_R"] = src_R;
    blocks["head_L"] = head_L;
    blocks["head_R"] = head_R;
    r.set_pyb2(blocks);
    
    r.tb = tb.to_top_block();
    return r;

# Create our handler's hook callback to be run before and after event flowgraph execution
def pre_hook(val):
    key_pmt = es.event_field( val.msg, key_sym );   
    key = pmt.pmt_symbol_to_string( key_pmt );
    print key;
    if(key == "*"):
        key = "10";
    elif(key == "#"):
        key = "12";
    elif(key=="0"):
        ival = 11;
    ival = int(key);
    
    row = (ival - 1)/3;
    col = (ival - 1)%3;
    rowfreqs = [697.0, 770.0, 852.0, 941.0];
    colfreqs = [1209.0, 1336.0, 1477.0, 1633.0];
    assert(row < len(rowfreqs));
    assert(col < len(colfreqs));

    blocks = val.handler.pb2();
    blocks["src_L"].set_frequency(rowfreqs[row]);
    blocks["src_R"].set_frequency(colfreqs[col]);
    print "set freq %s"%( str((rowfreqs[row], colfreqs[col])) );
    r = es.es_hook_rval();
    return r;

# post hook, reset the head blocks' counters
def post_hook(val):
    blocks = val.handler.pb2();
    blocks["head_L"].reset();
    blocks["head_R"].reset();
    r = es.es_hook_rval();
    return r;

# define our new event handler
key_handler = es.es_pyhandler_def();
key_handler.set_pyfactory_cb(key_factory);
key_handler.set_pypre_hook_cb(pre_hook);
key_handler.set_pypost_hook_cb(post_hook);
key_handler_graph = es.es_handler_flowgraph( key_handler, 2 );

# set up some common es components
arb = es.es_make_arbiter();
queue = es.queue();
queue.set_early_behavior(1);

# set up the main flow graph
tb = gr.top_block();
src = es.source( arb, queue, [gr.sizeof_float, gr.sizeof_float] );
sink = audio.sink(int(fs));
tb.connect((src,0),(sink,0));
tb.connect((src,1),(sink,1));

fsnk = gr.file_sink(gr.sizeof_gr_complex, "keypad.out");
f2c = gr.float_to_complex();
tb.connect( (src,0), (f2c,0) , fsnk);
tb.connect( (src,1), (f2c,1) );

# create initial event, set up event bindings, handlers
e1 = es.event_create("key-press", 10, evt_len );
tmp_pmt = pmt.pmt_intern("1");
print tmp_pmt;
e1 = es.event_args_add( e1, key_sym, tmp_pmt );
queue.register_event_type( es.event_type( e1 ) );
queue.bind_handler( es.event_type( e1 ), es.make_handler_pmt(key_handler_graph) );
#queue.add_event( e1 );

# start the main flowgraph
tb.start();


import Tkinter as tk;
import sys;

def keypress(event):
    print "evt running"
    if(event.keysym == 'Escape'):
        root.destroy();
    x = event.char
    print x;
    key_evt = es.event_create("key-press", 0, int(100e-3 * fs) );
    key_evt = es.event_args_add( key_evt, key_sym, pmt.pmt_intern(str(x)) );
    queue.add_event( key_evt );
           
    
root = tk.Tk();
print "binding"
frame = tk.Frame(root, width=100, height=100)
frame.bind("<Key>", keypress);
frame.pack();
frame.focus_set();
print "bound"
#root.withdraw();
print "withdrew"
root.mainloop();
print "loopex"

tb.stop();
tb.wait();
