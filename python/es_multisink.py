#!/usr/bin/env python

from gnuradio import gr;
import es, pmt, uuid;

class multisink(gr.hier_block2):
    def __init__(self, io_sig=1*[gr.sizeof_gr_complex], nthreads=8, samplehist=1000, eb=0, ss=2, tg=None, nsinks=2):
        # parent constructor
        gr.hier_block2.__init__(self,
            "es_multisink",
            gr.io_signature(len(io_sig), len(io_sig), io_sig[0]),
            gr.io_signature(0,0,0))
        if(tg == None):
            tg = uuid.uuid4();

        # set up ports
        self.message_port_register_hier_in("schedule_event");
        self.message_port_register_hier_out("nconsumed");
        self.message_port_register_hier_out("pdu_event");

        # create sinks
        self.sinks = [];
        for i in range(0, nsinks):
            self.sinks.append(
                es.sink(io_sig, nthreads,samplehist,eb,ss,tg)
                )

        # Set up hier connections
        for sink in self.sinks:
            print "Internal connect: ",sink, self
            # TODO: message connections are currently breaking this block
#            self.msg_connect( (self, "schedule_event"), (sink, "schedule_event") )       
#            self.msg_connect( sink, "nconsumed", self, "nconsumed" )       
#            self.msg_connect( sink, "pdu_event", self, "pdu_event" )       
            self.connect( (self,0), (sink,0) )

