#!/usr/bin/env python

from gnuradio import gr;


class ofdm_burst_mod(gr.hier_block2):
    def __init__(self):
        gr.hier_block2.__init__(self, "ofdm_burst_mod", 
                    gr.io_signature(0,0,0),
                    gr.io_signature(1,1,gr.sizeof_gr_complex))
        print "init";
    
        




a = ofdm_burst_mod();
b = gr.vector_sink_c();
c = gr.top_block();
c.connect(a,b);

c.run();







