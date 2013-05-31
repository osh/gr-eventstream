#!/usr/bin/env python
from gnuradio import gr;
import numpy;
class patterned_interleaver(gr.basic_block):
    def __init__(self,itemsize, in_ports, pattern):
        gr.basic_block.__init__(self, name="patterned_interleaver", 
            in_sig=in_ports*[itemsize],
            out_sig=[itemsize]);
        self.pattern = pattern;
        self.pat_len = len(pattern);
        self.in_ports = in_ports;
        self.ptr = 0;
        self.last_empty = 0;
        self.set_output_multiple(self.pat_len);

        self.input_count = list(numpy.zeros(self.in_ports,dtype=numpy.int32));
        for i in range(0,in_ports):
            for v in self.pattern:
                if( v == i ):
                    self.input_count[i] = self.input_count[i] + 1;

    def forecast(self, noutput_items, ninput_items_required):
        for i in range(0,len(ninput_items_required)):
            ninput_items_required[i] = int(self.input_count[i] * noutput_items/self.pat_len);
        return;

    def general_work(self, input_items, output_items):
        out = output_items[0];

        blocks = len(out)/self.pat_len;
        for i in range(0,self.in_ports):
            blocks = min(blocks, len(input_items[i])/self.input_count[i]);

    
        for i in range(0,blocks):
            inptrs = list(numpy.zeros(self.in_ports,dtype=numpy.int32));
            for j in range(0,self.pat_len):
                sel = self.pattern[j];
                out[i*self.pat_len + j] = input_items[sel][i*self.input_count[sel] + inptrs[sel]]
                inptrs[sel] = inptrs[sel] + 1;
          
        if blocks == 0:
            if(self.last_empty > 2):
                return -1;
            else:
                self.last_empty = self.last_empty + 1;
        else:
            self.last_empty = 0;
            for i in range(0, self.in_ports):
                self.consume(i, int(blocks * self.input_count[i]));

        return int(blocks*self.pat_len);

    def reset(self):
        #self.ptr = 0;
        self.last_empty = False;

if __name__ == '__main__':
    a = gr.vector_source_c([0,0,0,0,0]);
    b = gr.vector_source_c([1,1,1,1,1]);
    c = patterned_interleaver( numpy.complex64, 2, [0,1] );
    d = gr.vector_sink_c();
    tb = gr.top_block();
    tb.connect(a,c,d);
    tb.connect( (b,0), (c,1) );
    tb.run();
    print d.data();   


