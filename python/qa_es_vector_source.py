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


src = es.vector_source( [1,2] );

sink1 = gr.vector_sink_b();
sink2 = gr.vector_sink_s();

tb = gr.top_block();

tb.connect( (src,0), (sink1,0) );
tb.connect( (src,1), (sink2,0) );

tb.run();
print sink1.data();
print sink2.data();

d1 = numpy.arange(0,30,3,numpy.byte);
d2 = numpy.arange(0,20,2,numpy.int16);

s1 = d1.tostring();
s2 = d2.tostring();

src.set_data( es.StrVector( [s1,s2] ) , 10 );

tb.run();
print sink1.data();
print sink2.data();

