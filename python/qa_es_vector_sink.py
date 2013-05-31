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
import numpy;


src = gr.vector_source_b([0,1,2,3,4,5,6,7,8,9]);
s2v = gr.stream_to_vector(1,2);
sink = es.vector_sink( [1, 2] );

tb = gr.top_block();

tb.connect(src, (sink,0));
tb.connect(src, s2v, (sink,1));

tb.run();

data = sink.strvec();


types = [ numpy.byte, numpy.int16 ];
for i in range(0, len(data)):
    d = data[i];
    t = types[i];
    v = numpy.fromstring( d, dtype=t );
    print v;
    print len(v);

