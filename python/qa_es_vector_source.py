#!/usr/bin/env python


from gnuradio import gr;
import es;
import numpy;


src = es.vector_source( [1,2] );

sink1 = gr.vector_sink_b();
sink2 = gr.vector_sink_s();

tb = gr.top_block();

tb.connect( (src,0), (sink1,0) );
tb.connect( (src,1), (sink2,0) );

tb.run();

d1 = numpy.arange(0,10,1,numpy.byte);
d2 = numpy.arange(0,10,1,numpy.int16);

s1 = d1.tostring();
s2 = d2.tostring();

print s1;
print s2;

src.set_data( [s1, s2], 10 );


