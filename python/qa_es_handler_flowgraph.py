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

from gnuradio import gr;
import es;
from gruel import pmt;

# Create out flowgraph, define the event's sources and/or sinks
def factory_cb(index):
    print "FACTORY CALLED index = %d"%(index);
    r = es.es_pyhandler();
    src = gr.vector_source_b([10,20,30]);
    r.sink = es.vector_sink([1]);
    tb  = gr.top_block();
    tb.connect(src, r.sink);

    r.tb = tb.to_top_block();
    r.blocks["src"] = src.to_basic_block();
    return r;

# Create our hook callback to be run before and after event flowgraph execution
def hook_cb(args):
    print "TEST HOOK CB Called with %s :: %s"%(type(args), str(args));
    print args.bufs;
    print args.msg;
    print args.blocks.keys();
    r = es.es_hook_rval();
    return r;

ph = es.es_pyhandler_def();
ph.set_pyfactory_cb(factory_cb);
#ph.set_pypre_hook_cb(hook_cb);
#ph.set_pypost_hook_cb(hook_cb);

fgh = es.es_handler_flowgraph( ph, 1 );

arb = es.es_make_arbiter();
queue = es.queue();

tb = gr.top_block();
src = es.source( arb, queue, [1] );
sink = gr.vector_sink_b();

src.set_max(20);
tb.connect(src,sink);

e1 = es.event_create("fg-event", 2, 3);
e2 = es.event_create("fg-event", 8, 3);
e3 = es.event_create("fg-event", 12, 3);

queue.register_event_type( es.event_type( e1 ) );
queue.bind_handler( es.event_type( e1 ), es.make_handler_pmt(fgh) );

print "--------calling queue.add_event(e1)-------------";
queue.add_event( e1 );
queue.add_event( e2 );
queue.add_event( e3 );

print "--------calling tb.run()-------------";
tb.run();
print "python: tb.run() returned"
print "sink.data() = %s"%(str( sink.data() ));
